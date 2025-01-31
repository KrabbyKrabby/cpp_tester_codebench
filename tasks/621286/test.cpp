#include <gtest/gtest.h>
#include "solution.cpp"

TEST(DijkstraTestDijkstraTest, MixedGraphTest)
{
    // Number of nodes in the graph
    int n = 6;

    // Graph represented as an adjacency list
    std::vector<std::vector<Pair>> graph(n);

    // Add edges (node1, node2, weight)
    graph[0].push_back({1, -4});
    graph[0].push_back({2, 4});
    graph[1].push_back({2, -2});
    graph[1].push_back({3, 5});
    graph[2].push_back({1, -2});
    graph[2].push_back({3, -8});
    graph[2].push_back({4, 10});
    graph[3].push_back({5, 6});
    graph[4].push_back({5, -2});

    vector<int> distances = dijkstra(0, graph);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 2);
    EXPECT_EQ(distances[2], 4);
    EXPECT_EQ(distances[3], 1);
    EXPECT_EQ(distances[4], 14);
    EXPECT_EQ(distances[5], 7);
}
TEST(DijkstraTestDijkstraTest, PositiveGraphTest)
{
    // Number of nodes in the graph
    int n = 6;

    // Graph represented as an adjacency list
    std::vector<std::vector<Pair>> graph(n);

    // Add edges (node1, node2, weight)
    graph[0].push_back({1, 4});
    graph[0].push_back({2, 4});
    graph[1].push_back({2, 2});
    graph[1].push_back({3, 5});
    graph[2].push_back({3, 8});
    graph[2].push_back({4, 10});
    graph[3].push_back({5, 6});
    graph[4].push_back({5, 2});

    vector<int> distances = dijkstra(0, graph);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 4);
    EXPECT_EQ(distances[2], 4);
    EXPECT_EQ(distances[3], 9);
    EXPECT_EQ(distances[4], 14);
    EXPECT_EQ(distances[5], 15);
}

TEST(DijkstraTestDijkstraTest, HandlesPositiveEdges) {
    vector<vector<Pair>> graph(4);
    graph[0].push_back({1, 4});
    graph[0].push_back({2, 1});
    graph[1].push_back({2, 2});
    graph[1].push_back({3, 5});
    graph[2].push_back({3, 1});

    vector<int> distances;
    distances = dijkstra(0, graph);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 4);
    EXPECT_EQ(distances[2], 1);
    EXPECT_EQ(distances[3], 2);
}

TEST(DijkstraTestDijkstraTest, HandlesNegativeEdgesClosestToZero) {
    vector<vector<Pair>> graph(4);
    graph[0].push_back({1, -4});
    graph[0].push_back({2, 3});
    graph[1].push_back({2, -2});
    graph[1].push_back({3, 7});
    graph[2].push_back({3, 1});

    vector<int> distances;
    distances = dijkstra(0, graph);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], -4);
    EXPECT_EQ(distances[2], 3);
    EXPECT_EQ(distances[3], 3);
}

TEST(DijkstraTestDijkstraTest, HandlesDisconnectedGraph) {
    vector<vector<Pair>> graph(5);
    graph[0].push_back({1, 2});
    graph[1].push_back({2, 3});

    vector<int> distances;
    distances = dijkstra(0, graph);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 2);
    EXPECT_EQ(distances[2], 5);
    EXPECT_EQ(distances[3], numeric_limits<int>::max());
    EXPECT_EQ(distances[4], numeric_limits<int>::max());
}

TEST(DijkstraTestDijkstraTest, HandlesNegativeCyclePrevention) {
    vector<vector<Pair>> graph(3);
    graph[0].push_back({1, -1});
    graph[1].push_back({2, -1});
    graph[2].push_back({0, -1});

    vector<int> distances;
    distances = dijkstra(0, graph);

    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], -1);
    EXPECT_EQ(distances[2], -2);
}

TEST(DijkstraTestDijkstraTest, EmptyGraphTest)
{
    vector<vector<Pair>> graph; // Empty graph

    vector<int> distances;
    ASSERT_NO_THROW(dijkstra(0, graph)); // Ensure no exceptions are thrown
    EXPECT_TRUE(distances.empty()); // Distances should remain empty and all distances equals to max value
}

int main(int argc, char* argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    return 0;
}