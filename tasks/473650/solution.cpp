#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <stdexcept>

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
        if (source < 0 || source >= numNodes || destination < 0 || destination >= numNodes) {
            throw std::invalid_argument("Invalid node indices");
        }
        if (capacity < 0) {
            throw std::invalid_argument("Capacity cannot be negative");
        }
        graph[source].push_back({ destination, capacity, static_cast<int>(graph[destination].size()) });
        graph[destination].push_back({ source, 0, static_cast<int>(graph[source].size()) - 1 }); // Reverse edge
    }

    // Function to find an augmenting path using BFS
    bool findAugmentingPath(int source, int sink, std::unordered_map<int, int>& parent) {
        std::vector<bool> visited(numNodes, false);
        std::queue<int> q;
        q.push(source);
        visited[source] = true;
        parent.clear();
        parent[source] = -1;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (size_t i = 0; i < graph[current].size(); ++i) {
                const Edge& edge = graph[current][i];
                if (!visited[edge.destination] && edge.capacity > 0) {
                    q.push(edge.destination);
                    visited[edge.destination] = true;
                    parent[edge.destination] = current;

                    if (edge.destination == sink) {
                        return true; // Path to sink found
                    }
                }
            }
        }
        return false; // No augmenting path found
    }

    // Function to calculate the maximum flow using the Ford-Fulkerson method
    int edmondsKarp(int source, int sink) {
        if (source < 0 || source >= numNodes || sink < 0 || sink >= numNodes) {
            throw std::invalid_argument("Invalid source or sink indices");
        }

        int maxFlow = 0;
        std::unordered_map<int, int> parent;

        while (findAugmentingPath(source, sink, parent)) {
            int pathFlow = INT_MAX;

            // Find the minimum capacity in the augmenting path
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                for (const auto& edge : graph[u]) {
                    if (edge.destination == v && edge.capacity > 0) {
                        pathFlow = std::min(pathFlow, edge.capacity);
                        break;
                    }
                }
            }

            // Update capacities of the edges and reverse edges
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                for (auto& edge : graph[u]) {
                    if (edge.destination == v) {
                        edge.capacity -= pathFlow;
                        break;
                    }
                }
                for (auto& edge : graph[v]) {
                    if (edge.destination == u) {
                        edge.capacity += pathFlow;
                        break;
                    }
                }
            }

            maxFlow += pathFlow;
        }
        return maxFlow;
    }

    // Function to visualize the graph
    void visualizeGraph() {
        for (int i = 0; i < numNodes; ++i) {
            std::cout << "Node " << i << ":\n";
            for (const auto& edge : graph[i]) {
                std::cout << "  -> Node " << edge.destination << " (Capacity: " << edge.capacity << ")\n";
            }
        }
    }

private:
    int numNodes;                          // Number of nodes in the graph
    std::vector<std::vector<Edge>> graph; // Adjacency list representation of the graph
};