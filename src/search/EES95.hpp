#pragma once
#include "BoundedSuboptimalBase.hpp"

using namespace std;

template<class Domain, class Node>
class EES95 : public BoundedSuboptimalBase<Domain, Node>
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

    enum class Qtype
    {
        undefined,
        focal,
        open,
        cleanup,
        openAndCleanup
    };

public:
    EES95(Domain& domain_, const string& sorting_)
        : BoundedSuboptimalBase<Domain, Node>(domain_, sorting_)
        , fhatminVar(100)
        , fhatminSum(0)
        , fhatminSumSq(0)
        , fhatminCounter(0)
    {}

    ~EES95()
    {
        // delete all of the nodes from the last expansion phase
        for (typename unordered_map<State, Node*, Hash>::iterator it =
               closed.begin();
             it != closed.end(); it++)
            delete it->second;

        closed.clear();
    }

    double run(SearchResultContainer& res)
    {
        open.swapComparator(Node::compareNodesFHat);
        focal.swapComparator(Node::compareNodesDHat);
        cleanup.swapComparator(Node::compareNodesF);

        auto inith = this->domain.heuristic(this->domain.getStartState());
        auto initD = this->domain.distance(this->domain.getStartState());

        // Get the start node
        Node* initNode = new Node(
          0, inith, initD, this->domain.epsilonHGlobal(),
          this->domain.epsilonDGlobal(), this->domain.epsilonHVarGlobal(),
          this->domain.getStartState(), NULL);

        Node* weightedInitNode =
          new Node(Node::weight * initNode->getGValue(),
                   Node::weight * initNode->getHValue(),
                   Node::weight * initNode->getDValue(),
                   this->domain.epsilonHGlobal(), this->domain.epsilonDGlobal(),
                   this->domain.epsilonHVarGlobal(), State(), NULL);

        fmin        = initNode->getFValue();
        fhatmin     = initNode->getFHatValue();
        fhatminNode = initNode;
        pushFhatmin();

        open.insert(initNode);
        cleanup.push(initNode);
        bool isIncreament;
        open.updateCursor(weightedInitNode, isIncreament);
        res.initialH = inith;

        // Expand until find the goal
        while (!open.empty()) {

            // debug ees
            // cout << "focal q: " << focal.size() << "\n";
            // cout << "open q: " << open.getSize() << "\n";
            // cout << "cleanup q: " << cleanup.size() << "\n";

            Qtype nodeFrom = Qtype::undefined;
            Node* cur      = selectNode(nodeFrom);

            /*            cerr << "{\"g\":" << cur->getGValue() << ", ";*/
            // cerr << "\"f\":" << cur->getFValue() << ", ";
            // cerr << "\"h\":" << cur->getHValue() << ", ";
            // cerr << "\"d\":" << cur->getDValue() << ", ";
            // cerr << "\"fhat\":" << cur->getFHatValue() << ", ";
            // cerr << "\"expansion\":" << res.nodesExpanded << ", ";
            // cerr << "\"fmin\":" << fmin << ", ";
            // cerr << "\"open size\":" << open.getSize() << ", ";
            // cerr << "\"focal size\":" << focal.size() << ", ";
            // cerr << "\"fhatmin\":" << fhatmin << "}\n";

            // cout << "{\"g\":" << cur->getGValue() << ", ";
            // cout << "\"f\":" << cur->getFValue() << ", ";
            // cout << "\"h\":" << cur->getHValue() << ", ";
            // cout << "\"d\":" << cur->getDValue() << ", ";
            // cout << "\"fhat\":" << cur->getFHatValue() << ", ";
            // cout << "\"expansion\":" << res.nodesExpanded << ", ";
            // cout << "\"fmin\":" << fmin << ", ";
            // cout << "\"open size\":" << open.getSize() << ", ";
            // cout << "\"focal size\":" << focal.size() << ", ";
            // cout << "\"fhatmin\":" << fhatmin << "}\n";

            // Check if current node is goal
            if (this->domain.isGoal(cur->getState())) {
                this->getSolutionPath(res, cur);
                return cur->getFValue();
            }

            res.nodesExpanded++;

            cur->close();

            vector<State> children = this->domain.successors(cur->getState());
            res.nodesGenerated += children.size();

            State bestFChildState;
            Cost  bestF = numeric_limits<double>::infinity();

            for (State child : children) {

                auto newG = cur->getGValue() + this->domain.getEdgeCost(child);
                auto newH = this->domain.heuristic(child);
                auto newD = this->domain.distance(child);

                Node* childNode =
                  new Node(newG, newH, newD, this->domain.epsilonHGlobal(),
                           this->domain.epsilonDGlobal(),
                           this->domain.epsilonHVarGlobal(), child, cur);

                // Duplicate detection
                bool dup = duplicateDetection(childNode);

                if (!dup && childNode->getFValue() < bestF) {
                    bestF           = childNode->getFValue();
                    bestFChildState = child;
                }

                if (!dup) {
                    childNode->computeExpectedEffortValue(fhatminNode,
                                                          fhatminVar);
                    open.insert(childNode);
                    cleanup.push(childNode);
                    if (childNode->getFHatValue() <= Node::weight * fhatmin &&
                        childNode->getDXESProbValue() > 0.95) {
                        focal.push(childNode);
                    }
                    closed[child] = childNode;
                } else
                    delete childNode;
            }

            // Learn one-step error
            if (bestF != numeric_limits<double>::infinity()) {
                Cost epsD = (1 + this->domain.distance(bestFChildState)) -
                            cur->getDValue();
                Cost epsH = (this->domain.getEdgeCost(bestFChildState) +
                             this->domain.heuristic(bestFChildState)) -
                            cur->getHValue();

                this->domain.pushEpsilonHGlobal(epsH);
                this->domain.pushEpsilonDGlobal(epsD);

                this->domain.updateEpsilons();
            }

            // update fmin
            // if (nodeFrom == Qtype::cleanup ||
            // nodeFrom == Qtype::openAndCleanup) {
            fmin = cleanup.top()->getFValue();
            //}

            // update fhatmin

            auto bestFHatNode = open.getMinItem();
            if (fhatmin != bestFHatNode->getFHatValue()) {

                fhatmin = bestFHatNode->getFHatValue();
                fhatminNode = bestFHatNode;

                Node* weightedFhatMinNode = new Node(
                  Node::weight * bestFHatNode->getGValue(),
                  Node::weight * bestFHatNode->getHValue(),
                  Node::weight * bestFHatNode->getDValue(),
                  this->domain.epsilonHGlobal(), this->domain.epsilonDGlobal(),
                  this->domain.epsilonHVarGlobal(), State(), NULL);
                // cout << "c1\n";
                // open.prettyPrint();
                // cout << "cur status " << open.getCursorStatus() << "\n";
                // cout << "cur value " << open.getCursorValue() << "\n";
                // cout << "new cur value " <<
                // weightedFhatMinNode->getFHatValue() << "\n"; cout << "max
                // tree node " << open.getMaxItem()->getFHatValue() << "\n";

                bool isIncrease;
                auto itemsNeedUpdate =
                  open.updateCursor(weightedFhatMinNode, isIncrease);

                // cout << "c2\n";
                for (auto item : itemsNeedUpdate) {
                    if (isIncrease)
                        focal.push(item);
                    else {
                        focal.remove(item);
                    }
                }
            }

            pushFhatmin();
        }

        return -1.0;
    }

private:
    Node* selectNode(Qtype& nodeFrom)
    {
        Node* cur;

        if (!focal.empty() &&

            focal.top()->getFHatValue() <= Node::weight * fmin) {

            // cout << "pop from focal\n";
            cur = focal.top();

            /*if (open.getSize() == 118) {*/
            // cout << "cur " << cur << "\n";
            // cout << "before delete "
            //<< "\n";
            // open.prettyPrint();
            /*}*/

            nodeFrom = Qtype::focal;

            auto isOpenTop    = cur == open.getMinItem();
            auto isCleanupTop = cur == cleanup.top();

            if (isOpenTop && isCleanupTop) {
                nodeFrom = Qtype::openAndCleanup;
            } else if (isOpenTop) {
                nodeFrom = Qtype::open;
            } else if (isCleanupTop) {
                nodeFrom = Qtype::cleanup;
            }

            focal.pop();

            /*if (open.getSize() == 2568) {*/
            // cout << "cur " << cur << "\n";
            // open.prettyPrint();
            // cout << "TNULL " << open.getTNULL() << "\n";
            //// exit(1);
            /*}*/
            // cout << "open size " << open.getSize() << "\n";

            open.deleteNode(cur);
            // open.checkTreePropertyRedKidsAreRed();
            cleanup.remove(cur);
            return cur;
        }

        cur = open.getMinItem();
        if (cur->getFHatValue() <= Node::weight * fmin) {
            // cout << "pop from open\n";
            focal.remove(cur);

            nodeFrom = Qtype::open;

            if (cur == cleanup.top()) {
                nodeFrom = Qtype::openAndCleanup;
            }

            open.deleteNode(cur);
            cleanup.remove(cur);
            return cur;
        }

        // cout << "pop from cleanup\n";
        cur = cleanup.top();
        focal.remove(cur);
        open.deleteNode(cur);
        cleanup.remove(cur);
        nodeFrom = Qtype::cleanup;
        return cur;
    }

    bool duplicateDetection(Node* node)
    {
        // Check if this state exists
        typename unordered_map<State, Node*, Hash>::iterator it =
          closed.find(node->getState());

        if (it != closed.end()) {
            /*cout << "dup found \n";*/
            // cout << "new " << node->getState();
            // cout << "old " << it->second->getState();

            // if the new node is better, update it on close
            if (node->getGValue() < it->second->getGValue()) {

                // This state has been generated before,
                // check if its node is on OPEN
                if (it->second->onOpen()) {
                    // This node is on OPEN and cleanup, keep the better g-value
                    // cout << "dup on open " << it->second << "\n";
                    open.deleteNode(it->second);

                    it->second->setGValue(node->getGValue());
                    it->second->setParent(node->getParent());
                    it->second->setHValue(node->getHValue());
                    it->second->setDValue(node->getDValue());
                    it->second->setEpsilonH(node->getEpsilonH());
                    it->second->setEpsilonHVar(node->getEpsilonHVar());
                    it->second->setEpsilonD(node->getEpsilonD());
                    it->second->setState(node->getState());

                    open.insert(it->second);
                    cleanup.update(it->second);
                    focal.update(it->second);
                } else {
                    it->second->reopen();

                    it->second->setGValue(node->getGValue());
                    it->second->setParent(node->getParent());
                    it->second->setHValue(node->getHValue());
                    it->second->setDValue(node->getDValue());
                    it->second->setEpsilonH(node->getEpsilonH());
                    it->second->setEpsilonHVar(node->getEpsilonHVar());
                    it->second->setEpsilonD(node->getEpsilonD());
                    it->second->setState(node->getState());

                    open.insert(it->second);
                    cleanup.push(it->second);
                    if (it->second->getFHatValue() <= Node::weight * fhatmin) {
                        focal.push(it->second);
                    }
                }
            }
            return true;
        }
        return false;
    }

    void pushFhatmin()
    {
        fhatminSum += fhatmin;
        fhatminSumSq += fhatmin * fhatmin;
        fhatminCounter++;

        if (fhatminCounter < 2) {
            fhatminVar = 100;
            return;
        }

        /*    if (fhatminCounter < 100) {*/
        // fhatminVar = 100;
        // return;
        //}

        fhatminVar =
          (fhatminSumSq - (fhatminSum * fhatminSum) / fhatminCounter) /
          (fhatminCounter - 1.0);
    }

    RBTree<Node*>                     open;
    PriorityQueue<Node*>              focal;
    PriorityQueue<Node*>              cleanup;
    Cost                              fmin;
    Cost                              fhatmin;
    Node*                             fhatminNode;
    unordered_map<State, Node*, Hash> closed;

    double fhatminVar;
    double fhatminSum;
    double fhatminSumSq;
    double fhatminCounter;
};
