#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>

struct Edge {
    int destination;
    int capacity;
    int reverseIndex;
};

class FlowNetwork {
public:
    FlowNetwork(int numNodes) : numNodes(numNodes), graph(numNodes) {}

    void addEdge(int source, int destination, int capacity) {
        if (source < 0 || source >= numNodes || destination < 0 || destination >= numNodes) {
            throw std::invalid_argument("Invalid node indices");
        }
        if (capacity < 0) {
            throw std::invalid_argument("Capacity must be non-negative");
        }

        // Create forward edge
        Edge forwardEdge{destination, capacity, static_cast<int>(graph[destination].size())};
        
        // Create reverse edge with 0 capacity
        Edge reverseEdge{source, 0, static_cast<int>(graph[source].size())};
        
        // Add edges to the graph
        graph[source].push_back(forwardEdge);
        graph[destination].push_back(reverseEdge);
    }

    bool findAugmentingPath(int source, int sink, std::unordered_map<int, int>& parent) {
        std::vector<bool> visited(numNodes, false);
        std::queue<int> q;
        
        q.push(source);
        visited[source] = true;
        parent[source] = -1;
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            
            // Explore all adjacent nodes
            for (int i = 0; i < graph[current].size(); i++) {
                int next = graph[current][i].destination;
                
                // If not visited and has remaining capacity
                if (!visited[next] && graph[current][i].capacity > 0) {
                    parent[next] = current;
                    parent[numNodes + next] = i; // Store edge index
                    
                    if (next == sink) {
                        return true;
                    }
                    
                    visited[next] = true;
                    q.push(next);
                }
            }
        }
        
        return false;
    }

    int edmondsKarp(int source, int sink) {
        if (source < 0 || source >= numNodes || sink < 0 || sink >= numNodes) {
            throw std::invalid_argument("Invalid source or sink node");
        }

        int maxFlow = 0;
        std::unordered_map<int, int> parent;
        
        // While there exists an augmenting path
        while (findAugmentingPath(source, sink, parent)) {
            // Find minimum residual capacity along the path
            int pathFlow = INT_MAX;
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                int edgeIndex = parent[numNodes + v];
                pathFlow = std::min(pathFlow, graph[u][edgeIndex].capacity);
            }
            
            // Update residual capacities
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                int edgeIndex = parent[numNodes + v];
                
                // Update forward edge
                graph[u][edgeIndex].capacity -= pathFlow;
                
                // Update reverse edge
                int reverseEdgeIndex = graph[u][edgeIndex].reverseIndex;
                graph[v][reverseEdgeIndex].capacity += pathFlow;
            }
            
            maxFlow += pathFlow;
        }
        
        return maxFlow;
    }

    void visualizeGraph() {
        for (int i = 0; i < numNodes; i++) {
            std::cout << "Node " << i << " connections:\n";
            for (const auto& edge : graph[i]) {
                std::cout << "  -> Node " << edge.destination 
                         << " (Capacity: " << edge.capacity << ")\n";
            }
        }
    }

private:
    int numNodes;
    std::vector<std::vector<Edge>> graph;
};
