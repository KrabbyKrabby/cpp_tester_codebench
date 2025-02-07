#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <limits>
#include "solution.cpp"

using namespace std;
typedef pair<int, int> Pair;

// Test for positive case where there are no negative weights
TEST(DijkstraTest, PositiveNoNegativeEdges) {
    vector<vector<Pair>> graph = {
        {{1, 1}, {2, 4}},
        {{2, 2}, {3, 5}},
        {{3, 1}},
        {}
    };

    vector<int> distances;
    dijkstra(0, graph, distances);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 1);
    EXPECT_EQ(distances[2], 3);
    EXPECT_EQ(distances[3], 4);
}

// Test for case with negative edge weights but no negative cycles
TEST(DijkstraTest, NegativeEdgeWeightsNoCycle) {
    vector<vector<Pair>> graph = {
        {{1, 2}, {2, 5}},
        {{2, -3}},
        {{3, 1}},
        {}
    };

    vector<int> distances;
    dijkstra(0, graph, distances);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 2);
    EXPECT_EQ(distances[2], -1);
    EXPECT_EQ(distances[3], 0);
}

// Test for negative cycle detection
TEST(DijkstraTest, NegativeCycleDetection) {
    vector<vector<Pair>> graph = {
        {{1, 2}},
        {{2, -3}},
        {{0, -4}}
    };

    vector<int> distances;
    dijkstra(0, graph, distances);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 0);
    EXPECT_EQ(distances[2], 0);
}

// Test for unreachable nodes
TEST(DijkstraTest, UnreachableNodes) {
    vector<vector<Pair>> graph = {
        {{1, 2}},
        {},
        {{3, 1}},
        {}
    };

    vector<int> distances;
    dijkstra(0, graph, distances);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 2);
    EXPECT_EQ(distances[2], std::numeric_limits<int>::max());
    EXPECT_EQ(distances[3], std::numeric_limits<int>::max());
}

// Test for edge case with empty graph
TEST(DijkstraTest, EmptyGraph) {
    vector<vector<Pair>> graph = {};

    vector<int> distances;
    EXPECT_THROW(dijkstra(0, graph, distances), std::invalid_argument);
}

// Test for wrong input
TEST(DijkstraTest, WrongInput) {
    vector<vector<Pair>> graph = {
        {{1, 2}},
        {},
        {{3, 1}},
        {}
    };

    vector<int> distances;
    EXPECT_THROW(dijkstra(15, graph, distances), std::out_of_range);

}