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

        // Add forward edge
        Edge forwardEdge{destination, capacity, static_cast<int>(graph[destination].size())};
        // Add reverse edge with 0 capacity
        Edge reverseEdge{source, 0, static_cast<int>(graph[source].size())};

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

            for (int i = 0; i < graph[current].size(); i++) {
                int next = graph[current][i].destination;
                if (!visited[next] && graph[current][i].capacity > 0) {
                    parent[next] = i;
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
        if (source < 0 || source >= numNodes || sink < 0 || sink >= numNodes) {
            throw std::invalid_argument("Invalid source or sink node");
        }

        int maxFlow = 0;
        std::unordered_map<int, int> parent;

        while (findAugmentingPath(source, sink, parent)) {
            // Find minimum residual capacity along the augmenting path
            int pathFlow = INT_MAX;
            int current = sink;
            int currentParent = parent[current];

            while (current != source) {
                int prev = graph[current][currentParent].destination;
                int edgeIndex = graph[current][currentParent].reverseIndex;
                pathFlow = std::min(pathFlow, graph[prev][edgeIndex].capacity);
                current = prev;
                currentParent = parent[current];
            }

            // Update residual capacities
            current = sink;
            currentParent = parent[current];

            while (current != source) {
                int prev = graph[current][currentParent].destination;
                int edgeIndex = graph[current][currentParent].reverseIndex;
                int reverseIndex = graph[prev][edgeIndex].reverseIndex;

                graph[prev][edgeIndex].capacity -= pathFlow;
                graph[current][currentParent].capacity += pathFlow;

                current = prev;
                currentParent = parent[current];
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    void visualizeGraph() {
        std::cout << "Network Graph Visualization:\n";
        for (int i = 0; i < numNodes; i++) {
            std::cout << "Node " << i << " connections:\n";
            for (const Edge& edge : graph[i]) {
                std::cout << "  -> Node " << edge.destination 
                         << " (Capacity: " << edge.capacity << ")\n";
            }
        }
        std::cout << std::endl;
    }

private:
    int numNodes;
    std::vector<std::vector<Edge>> graph;
};