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
        Edge forwardEdge = {destination, capacity, static_cast<int>(graph[destination].size())};
        Edge reverseEdge = {source, 0, static_cast<int>(graph[source].size())};

        graph[source].push_back(forwardEdge);
        graph[destination].push_back(reverseEdge);
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

            for (int i = 0; i < graph[currentNode].size(); ++i) {
                Edge& edge = graph[currentNode][i];

                if (!visited[edge.destination] && edge.capacity > 0) {
                    queue.push(edge.destination);
                    visited[edge.destination] = true;
                    parent[edge.destination] = currentNode;
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

            // Calculate the maximum flow that can be pushed through the path
            for (int currentNode = sink; currentNode != source; currentNode = parent[currentNode]) {
                int previousNode = parent[currentNode];

                for (int i = 0; i < graph[previousNode].size(); ++i) {
                    Edge& edge = graph[previousNode][i];

                    if (edge.destination == currentNode) {
                        pathFlow = std::min(pathFlow, edge.capacity);
                        break;
                    }
                }
            }

            // Update the capacities of the edges along the path
            for (int currentNode = sink; currentNode != source; currentNode = parent[currentNode]) {
                int previousNode = parent[currentNode];

                for (int i = 0; i < graph[previousNode].size(); ++i) {
                    Edge& edge = graph[previousNode][i];

                    if (edge.destination == currentNode) {
                        edge.capacity -= pathFlow;
                        graph[currentNode][edge.reverseIndex].capacity += pathFlow;
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
            std::cout << "Node " << i << " has edges to: ";

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

int main() {
    FlowNetwork network(6);

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

    std::cout << "Network Visualization:" << std::endl;
    network.visualizeGraph();

    int maxFlow = network.edmondsKarp(0, 5);
    std::cout << "Maximum Flow: " << maxFlow << std::endl;

    return 0;
}