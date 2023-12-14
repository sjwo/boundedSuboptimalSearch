load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_library(
    name = "SearchBase",
    srcs = ["SearchBase.hpp"],
    deps = [
        ":HeavyTilePuzzle",
        ":InverseTilePuzzle",
        ":PancakePuzzle",
        ":RaceTrack",
        ":ReverseTilePuzzle",
        ":SqrtTilePuzzle",
        ":VacuumWorld",
        ":EES",
        ":DXES",
        ":DXES95",
        ":DPS",
        ":DPSRoundRobin",
        ":BFSonP",
        ":EES95",
        ":RoundRobin",
        ":EESRoundRobin",
        ":WAstarSearch",
        ":SpeedySearch",
        ":EESDoylew",
        ":EESLi",
        ":DynamicBEES",
        ":EES95Doylew",
        ":SMHAstar",
        ":SMHAstar2",
    ]
)

cc_library(
    name = "SMHAstar2",
    srcs = ["search/SMHAstar2.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "SMHAstar",
    srcs = ["search/SMHAstar.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "DynamicBEES",
    srcs = ["search/DynamicBEES.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "EESLi",
    srcs = ["search/EESLi.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "EESDoylew",
    srcs = ["search/EESDoylew.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "EES95Doylew",
    srcs = ["search/EES95Doylew.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "SpeedySearch",
    srcs = ["search/SpeedySearch.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "WAstarSearch",
    srcs = ["search/WAstarSearch.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "RoundRobin",
    srcs = ["search/RoundRobin.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "EESRoundRobin",
    srcs = ["search/EESRoundRobin.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "BFSonP",
    srcs = ["search/BFSonP.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "DPSRoundRobin",
    srcs = ["search/DPSRoundRobin.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "DPS",
    srcs = ["search/DPS.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "DXES95",
    srcs = ["search/DXES95.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "DXES",
    srcs = ["search/DXES.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "EES95",
    srcs = ["search/EES95.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "EES",
    srcs = ["search/EES.hpp"],
    deps = [
        ":BoundedSuboptimalBase",
    ]
)

cc_library(
    name = "SearchResultContainer",
    srcs = ["utility/SearchResultContainer.h", "utility/ResultContainer.h"],
)

cc_library(
    name = "RBTree",
    srcs = ["utility/RBTree.h"],
)

cc_library(
    name = "PriorityQueue",
    srcs = ["utility/PriorityQueue.h"],
)

cc_library(
    name = "BoundedSuboptimalBase",
    srcs = ["search/BoundedSuboptimalBase.hpp"],
    deps = [
        ":PriorityQueue",
        ":RBTree",
        ":SearchResultContainer",
    ]
)

cc_library(
    name = "VacuumWorld",
    srcs = [
        "domain/vacuumWorld/VacuumWorld.h",
        "domain/vacuumWorld/mst.h",
        "domain/vacuumWorld/DisjointSet.h",
    ],
    # deps = [
    #     ":SlidingTilePuzzle",
    # ]
)

cc_library(
    name = "SqrtTilePuzzle",
    srcs = ["domain/SqrtTilePuzzle.h"],
    deps = [
        ":SlidingTilePuzzle",
    ]
)

cc_library(
    name = "ReverseTilePuzzle",
    srcs = ["domain/ReverseTilePuzzle.h"],
    deps = [
        ":SlidingTilePuzzle",
    ]
)

cc_library(
    name = "debug",
    srcs = ["utility/debug.h"],
)

cc_library(
    name = "PairHash",
    srcs = ["utility/PairHash.h"],
)

cc_library(
    name = "RaceTrack",
    srcs = ["domain/RaceTrack.h"],
    deps = [
        ":PairHash",
        ":debug",
    ]
)

cc_library(
    name = "PancakePuzzle",
    srcs = ["domain/PancakePuzzle.h"],
)

cc_library(
    name = "InverseTilePuzzle",
    srcs = ["domain/InverseTilePuzzle.h"],
    deps = [
        ":SlidingTilePuzzle",
    ]
)

cc_library(
    name = "SlidingWindow",
    srcs = ["utility/SlidingWindow.h"],
)

cc_library(
    name = "SlidingTilePuzzle",
    srcs = ["domain/SlidingTilePuzzle.h"],
    deps = [
        ":SlidingWindow",
    ]
)

cc_library(
    name = "HeavyTilePuzzle",
    srcs = ["domain/HeavyTilePuzzle.h"],
    deps = [
        ":SlidingTilePuzzle",
    ]
)


cc_library(
    name = "BoundedSuboptimalSearch",
    srcs = ["BoundedSuboptimalSearch.hpp"],
    deps = [
        "@boost//:math",
    ]
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        ":BoundedSuboptimalSearch",
        ":SearchBase",
    ],
)

