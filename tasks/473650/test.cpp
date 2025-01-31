#include <gtest/gtest.h>
#include "solution.cpp"

// Positive Test Case: Basic Maximum Flow
TEST(FlowNetworkTest, PositiveBasicMaxFlow) {
    FlowNetwork network(6);

    // Add edges
    network.addEdge(0, 1, 16);
    network.addEdge(0, 2, 13);
    network.addEdge(1, 2, 10);
    network.addEdge(1, 3, 12);
    network.addEdge(2, 1, 4);
    network.addEdge(2, 4, 14);
    network.addEdge(3, 2, 9);
    network.addEdge(3, 5, 20);
    network.addEdge(4, 3, 7);
    network.addEdge(4, 5, 4);

    // Calculate maximum flow
    int maxFlow = network.edmondsKarp(0, 5);
    EXPECT_EQ(maxFlow, 23); // Expected maximum flow is 23
}


// Edge Case: Disconnected Nodes
TEST(FlowNetworkTest, EdgeDisconnectedNodes) {
    FlowNetwork network(4);

    network.addEdge(0, 1, 10);
    network.addEdge(2, 3, 5);

    int maxFlow = network.edmondsKarp(0, 3);
    EXPECT_EQ(maxFlow, 0); // No path from source to sink
}

// Corner Case: Single Node Graph
TEST(FlowNetworkTest, CornerSingleNodeGraph) {
    FlowNetwork network(1);

    int maxFlow = network.edmondsKarp(0, 0);
    EXPECT_EQ(maxFlow, 0); // No flow in a single-node graph
}

// Edge Case: Graph with Zero Capacity Edges
TEST(FlowNetworkTest, EdgeZeroCapacityEdges) {
    FlowNetwork network(4);

    network.addEdge(0, 1, 0);
    network.addEdge(1, 2, 0);
    network.addEdge(2, 3, 0);

    int maxFlow = network.edmondsKarp(0, 3);
    EXPECT_EQ(maxFlow, 0); // No flow due to zero capacity
}

// Positive Test Case: Complex Flow
TEST(FlowNetworkTest, PositiveComplexFlow) {
    FlowNetwork network(5);

    // Add edges
    network.addEdge(0, 1, 10);
    network.addEdge(0, 2, 8);
    network.addEdge(1, 2, 2);
    network.addEdge(1, 3, 5);
    network.addEdge(2, 3, 10);
    network.addEdge(3, 4, 7);

    // Calculate maximum flow
    int maxFlow = network.edmondsKarp(0, 4);
    EXPECT_EQ(maxFlow, 7); // Expected maximum flow is 15
}

// Edge Case: Multiple Parallel Edges
TEST(FlowNetworkTest, EdgeMultipleParallelEdges) {
    FlowNetwork network(3);

    network.addEdge(0, 1, 10);
    network.addEdge(0, 1, 5); // Parallel edge
    network.addEdge(1, 2, 10);

    int maxFlow = network.edmondsKarp(0, 2);
    EXPECT_EQ(maxFlow, 10); // Sum of capacities of parallel edges
}

// Corner Case: No Edges in the Graph
TEST(FlowNetworkTest, CornerNoEdges) {
    FlowNetwork network(4);

    int maxFlow = network.edmondsKarp(0, 3);
    EXPECT_EQ(maxFlow, 0); // No edges, no flow
}

// Edge Case: Large Graph with Sparse Edges
TEST(FlowNetworkTest, EdgeLargeSparseGraph) {
    FlowNetwork network(1000);

    network.addEdge(0, 999, 20);

    int maxFlow = network.edmondsKarp(0, 999);
    EXPECT_EQ(maxFlow, 20); // Single edge connecting source to sink
}

// Edge Case: Circular Flow
TEST(FlowNetworkTest, EdgeCircularFlow) {
    FlowNetwork network(4);

    network.addEdge(0, 1, 10);
    network.addEdge(1, 2, 5);
    network.addEdge(2, 0, 15); // Circular edge
    network.addEdge(2, 3, 10);

    int maxFlow = network.edmondsKarp(0, 3);
    EXPECT_EQ(maxFlow, 5); // Flow to sink should not exceed bottleneck capacity
}