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
        Edge forwardEdge = {destination, capacity, graph[destination].size()};
        graph[source].push_back(forwardEdge);

        Edge backwardEdge = {source, 0, graph[source].size() - 1};
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

            for (const Edge& edge : graph[currentNode]) {
                if (!visited[edge.destination] && edge.capacity > 0) {
                    queue.push(edge.destination);
                    visited[edge.destination] = true;
                    parent[edge.destination] = currentNode;

                    if (edge.destination == sink) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    // Function to calculate the maximum flow using the Ford-Fulkerson method
    int edmondsKarp(int source, int sink) {
        int maxFlow = 0;
        std::unordered_map<int, int> parent;

        while (findAugmentingPath(source, sink, parent)) {
            int pathFlow = INT_MAX;

            // Find the minimum capacity along the path
            int currentNode = sink;
            while (currentNode != source) {
                int previousNode = parent[currentNode];
                for (const Edge& edge : graph[previousNode]) {
                    if (edge.destination == currentNode) {
                        pathFlow = std::min(pathFlow, edge.capacity);
                        break;
                    }
                }
                currentNode = previousNode;
            }

            // Update the capacities along the path
            currentNode = sink;
            while (currentNode != source) {
                int previousNode = parent[currentNode];
                for (Edge& edge : graph[previousNode]) {
                    if (edge.destination == currentNode) {
                        edge.capacity -= pathFlow;
                        graph[currentNode][edge.reverseIndex].capacity += pathFlow;
                        break;
                    }
                }
                currentNode = previousNode;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    // Function to visualize the graph
    void visualizeGraph() {
        for (int i = 0; i < numNodes; i++) {
            std::cout << "Node " << i << " is connected to: ";
            for (const Edge& edge : graph[i]) {
                std::cout << edge.destination << " with capacity " << edge.capacity << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    int numNodes;                          // Number of nodes in the graph
    std::vector<std::vector<Edge>> graph; // Adjacency list representation of the graph
};
