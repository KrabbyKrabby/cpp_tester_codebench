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
        // Forward edge
        Edge forwardEdge{destination, capacity, 
                        static_cast<int>(graph[destination].size())};
        
        // Reverse edge (residual)
        Edge reverseEdge{source, 0, 
                        static_cast<int>(graph[source].size())};
        
        graph[source].push_back(forwardEdge);
        graph[destination].push_back(reverseEdge);
    }

    bool findAugmentingPath(int source, int sink, 
                           std::unordered_map<int, int>& parent) {
        std::vector<bool> visited(numNodes, false);
        std::queue<int> q;
        
        q.push(source);
        visited[source] = true;
        parent[source] = -1;
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            
            for (int i = 0; i < graph[current].size(); i++) {
                int next = graph[current][i].destination;
                if (!visited[next] && graph[current][i].capacity > 0) {
                    parent[next] = i;
                    parent[next + numNodes] = current;  // Store parent node
                    visited[next] = true;
                    q.push(next);
                    
                    if (next == sink) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    int edmondsKarp(int source, int sink) {
        int maxFlow = 0;
        std::unordered_map<int, int> parent;
        
        while (findAugmentingPath(source, sink, parent)) {
            int pathFlow = INT_MAX;
            
            // Find minimum residual capacity along the path
            for (int v = sink; v != source; 
                 v = parent[v + numNodes]) {
                int u = parent[v + numNodes];
                int edgeIndex = parent[v];
                pathFlow = std::min(pathFlow, 
                                  graph[u][edgeIndex].capacity);
            }
            
            // Update residual capacities
            for (int v = sink; v != source; 
                 v = parent[v + numNodes]) {
                int u = parent[v + numNodes];
                int edgeIndex = parent[v];
                
                graph[u][edgeIndex].capacity -= pathFlow;
                graph[v][graph[u][edgeIndex].reverseIndex].capacity += pathFlow;
            }
            
            maxFlow += pathFlow;
        }
        
        return maxFlow;
    }

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
    int numNodes;
    std::vector<std::vector<Edge>> graph;
};
