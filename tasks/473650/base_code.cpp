#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>

// Structure to represent an edge in the graph
struct Edge {
    int destination;   // Destination node index
    int capacity;      // Edge capacity
    int reverseIndex;  // Index of the reverse edge
};

// Class to represent the flow network
class FlowNetwork {
public:
    FlowNetwork(int numNodes) : numNodes(numNodes), graph(numNodes) {}

    // Function to add an edge to the graph
    void addEdge(int source, int destination, int capacity) {
      //ToDo: Implementation
    }

    // Function to find an augmenting path using BFS
    bool findAugmentingPath(int source, int sink, std::unordered_map<int, int>& parent) {       
       //ToDO: Implement findAugmentingPath algorithm
    }

    // Function to calculate the maximum flow using the Ford-Fulkerson method
    int edmondsKarp(int source, int sink) {
    //TODO:  Implement edmondsKarp algorithm
    }

    // Function to visualize the graph
    void visualizeGraph() {
    //TODO: Implement  visualizeGraph function
    }

private:
    int numNodes;                          // Number of nodes in the graph
    std::vector<std::vector<Edge>> graph; // Adjacency list representation of the graph
};