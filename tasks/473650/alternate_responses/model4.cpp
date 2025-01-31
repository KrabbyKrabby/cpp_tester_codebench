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
        // Create forward edge
        Edge forwardEdge{destination, capacity, 
                        static_cast<int>(graph[destination].size())};
        
        // Create reverse edge with 0 initial capacity
        Edge reverseEdge{source, 0, 
                        static_cast<int>(graph[source].size())};
        
        // Add edges to the graph
        graph[source].push_back(forwardEdge);
        graph[destination].push_back(reverseEdge);
    }

    bool findAugmentingPath(int source, int sink, 
                           std::unordered_map<int, int>& parent) {
        std::vector<bool> visited(numNodes, false);
        std::queue<int> q;
        
        q.push(source);
        visited[source] = true;
        
        while (!q.empty() && !visited[sink]) {
            int current = q.front();
            q.pop();
            
            // Explore all adjacent nodes
            for (int i = 0; i < graph[current].size(); i++) {
                Edge& edge = graph[current][i];
                int next = edge.destination;
                
                if (!visited[next] && edge.capacity > 0) {
                    visited[next] = true;
                    parent[next] = i;
                    q.push(next);
                }
            }
        }
        
        return visited[sink];
    }

    int edmondsKarp(int source, int sink) {
        int maxFlow = 0;
        std::unordered_map<int, int> parent;
        
        // While there exists an augmenting path
        while (findAugmentingPath(source, sink, parent)) {
            // Find minimum residual capacity along the path
            int pathFlow = INT_MAX;
            int current = sink;
            
            while (current != source) {
                int parentNode = -1;
                for (int i = 0; i < numNodes; i++) {
                    for (int j = 0; j < graph[i].size(); j++) {
                        if (graph[i][j].destination == current && 
                            graph[i][j].capacity > 0) {
                            parentNode = i;
                            break;
                        }
                    }
                    if (parentNode != -1) break;
                }
                
                Edge& edge = graph[parentNode][parent[current]];
                pathFlow = std::min(pathFlow, edge.capacity);
                current = parentNode;
            }
            
            // Update residual capacities
            current = sink;
            while (current != source) {
                int parentNode = -1;
                int edgeIndex = -1;
                for (int i = 0; i < numNodes; i++) {
                    for (int j = 0; j < graph[i].size(); j++) {
                        if (graph[i][j].destination == current && 
                            graph[i][j].capacity > 0) {
                            parentNode = i;
                            edgeIndex = j;
                            break;
                        }
                    }
                    if (parentNode != -1) break;
                }
                
                Edge& edge = graph[parentNode][edgeIndex];
                Edge& reverseEdge = graph[current][edge.reverseIndex];
                
                edge.capacity -= pathFlow;
                reverseEdge.capacity += pathFlow;
                
                current = parentNode;
            }
            
            maxFlow += pathFlow;
            parent.clear();
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
