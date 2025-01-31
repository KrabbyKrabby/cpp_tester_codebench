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
        // Create forward edge
        Edge forwardEdge{destination, capacity, 
                        static_cast<int>(graph[destination].size())};
        
        // Create reverse edge (for residual graph)
        Edge reverseEdge{source, 0, 
                        static_cast<int>(graph[source].size())};

        // Add edges to the graph
        graph[source].push_back(forwardEdge);
        graph[destination].push_back(reverseEdge);
    }

    // Function to find an augmenting path using BFS
    bool findAugmentingPath(int source, int sink, 
                           std::unordered_map<int, int>& parent) {
        std::vector<bool> visited(numNodes, false);
        std::queue<int> q;
        
        // Start BFS from source
        q.push(source);
        visited[source] = true;
        parent[source] = -1;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            // Explore all adjacent nodes
            for (int i = 0; i < graph[current].size(); i++) {
                int next = graph[current][i].destination;
                
                // If not visited and has capacity
                if (!visited[next] && graph[current][i].capacity > 0) {
                    parent[next] = i;
                    visited[next] = true;
                    q.push(next);

                    if (next == sink) {
                        return true;  // Path found
                    }
                }
            }
        }
        return false;  // No path found
    }

    // Function to calculate the maximum flow using the Ford-Fulkerson method
    int edmondsKarp(int source, int sink) {
        int maxFlow = 0;
        std::unordered_map<int, int> parent;

        // While there exists an augmenting path
        while (findAugmentingPath(source, sink, parent)) {
            // Find minimum residual capacity along the path
            int pathFlow = INT_MAX;
            int current = sink;
            
            while (current != source) {
                int prev = graph[current][parent[current]].destination;
                int edgeIndex = graph[current][parent[current]].reverseIndex;
                pathFlow = std::min(pathFlow, 
                                  graph[prev][edgeIndex].capacity);
                current = prev;
            }

            // Update residual capacities
            current = sink;
            while (current != source) {
                int prev = graph[current][parent[current]].destination;
                int edgeIndex = graph[current][parent[current]].reverseIndex;
                int revEdgeIndex = parent[current];

                graph[prev][edgeIndex].capacity -= pathFlow;
                graph[current][revEdgeIndex].capacity += pathFlow;
                
                current = prev;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    // Function to visualize the graph
    void visualizeGraph() {
        for (int i = 0; i < numNodes; i++) {
            std::cout << "Node " << i << " connections:\n";
            for (const Edge& edge : graph[i]) {
                std::cout << "  -> Node " << edge.destination 
                         << " (Capacity: " << edge.capacity << ")\n";
            }
        }
    }

private:
    int numNodes;                          // Number of nodes in the graph
    std::vector<std::vector<Edge>> graph;  // Adjacency list representation
};
