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
    int flow;          // Current flow on the edge
};

// Class to represent the flow network
class FlowNetwork {
public:
    FlowNetwork(int numNodes) : numNodes(numNodes), graph(numNodes) {}

    // Function to add an edge to the graph
    void addEdge(int source, int destination, int capacity) {
        Edge forwardEdge = {destination, capacity, (int)graph[destination].size(), 0};
        Edge backwardEdge = {source, 0, (int)graph[source].size(), 0}; // Initialize reverse edge with 0 capacity

        graph[source].push_back(forwardEdge);
        graph[destination].push_back(backwardEdge);
    }

    // Function to find an augmenting path using BFS
    bool findAugmentingPath(int source, int sink, std::unordered_map<int, int>& parent) {
        std::vector<bool> visited(numNodes, false);
        std::queue<int> queue;

        queue.push(source);
        visited[source] = true;

        while (!queue.empty()) {
            int currentNode = queue.front();
            queue.pop();

            for (int edgeIndex = 0; edgeIndex < graph[currentNode].size(); ++edgeIndex) {
                Edge& edge = graph[currentNode][edgeIndex];
                int destination = edge.destination;

                if (!visited[destination] && edge.capacity > edge.flow) {
                    queue.push(destination);
                    visited[destination] = true;
                    parent[destination] = currentNode;
                }
            }
        }

        return visited[sink];
    }

    // Function to calculate the maximum flow using the Ford-Fulkerson method
    int edmondsKarp(int source, int sink) {
        int maxFlow = 0;
        std::unordered_map<int, int> parent;

        while (findAugmentingPath(source, sink, parent)) {
            int pathFlow = INT_MAX;
            int currentNode = sink;

            while (currentNode != source) {
                int previousNode = parent[currentNode];
                int edgeIndex = -1;

                // Find the edge from previousNode to currentNode
                for (int i = 0; i < graph[previousNode].size(); ++i) {
                    if (graph[previousNode][i].destination == currentNode) {
                        edgeIndex = i;
                        break;
                    }
                }

                if (edgeIndex != -1) {
                    pathFlow = std::min(pathFlow, graph[previousNode][edgeIndex].capacity - graph[previousNode][edgeIndex].flow);
                }

                currentNode = previousNode;
            }

            currentNode = sink;
            while (currentNode != source) {
                int previousNode = parent[currentNode];
                int edgeIndex = -1;

                // Find the edge from previousNode to currentNode
                for (int i = 0; i < graph[previousNode].size(); ++i) {
                    if (graph[previousNode][i].destination == currentNode) {
                        edgeIndex = i;
                        break;
                    }
                }

                if (edgeIndex != -1) {
                    graph[previousNode][edgeIndex].flow += pathFlow;
                    graph[currentNode][graph[previousNode][edgeIndex].reverseIndex].flow -= pathFlow;
                }

                currentNode = previousNode;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    // Function to visualize the graph
    void visualizeGraph() {
        for (int node = 0; node < numNodes; ++node) {
            std::cout << "Node " << node << " is connected to: ";
            for (const Edge& edge : graph[node]) {
                std::cout << edge.destination << " (Capacity: " << edge.capacity << ", Flow: " << edge.flow << ") ";
            }
            std::cout << std::endl;
        }
    }

private:
    int numNodes;                          // Number of nodes in the graph
    std::vector<std::vector<Edge>> graph; // Adjacency list representation of the graph
};