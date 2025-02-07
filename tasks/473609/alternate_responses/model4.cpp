#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>

using namespace std;
typedef pair<int, int> Pair;

void dijkstra(int start, const vector<vector<Pair>>& graph, vector<int>& distances) {
    int n = graph.size();
    
    // Handle empty graph
    if (n == 0) {
        throw invalid_argument("Graph is empty.");
    }
    
    // Validate start node
    if (start < 0 || start >= n) {
        throw out_of_range("Start node is out of range.");
    }

    // Initialize distances and visited nodes tracking
    distances.assign(n, numeric_limits<int>::max());
    vector<bool> inNegativeCycle(n, false);
    vector<int> relaxCount(n, 0);
    distances[start] = 0;

    // First phase: Bellman-Ford style relaxation
    for (int i = 0; i < n - 1; ++i) {
        bool anyUpdate = false;
        for (int u = 0; u < n; ++u) {
            if (distances[u] == numeric_limits<int>::max()) continue;
            
            for (const Pair& edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;
                
                // Prevent integer overflow
                if (distances[u] != numeric_limits<int>::max() &&
                    weight != numeric_limits<int>::max() &&
                    distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    anyUpdate = true;
                    relaxCount[v]++;
                }
            }
        }
        if (!anyUpdate) break;
    }

    // Second phase: Detect and handle negative cycles
    for (int i = 0; i < n; ++i) {
        for (int u = 0; u < n; ++u) {
            if (distances[u] == numeric_limits<int>::max()) continue;
            
            for (const Pair& edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;
                
                if (distances[u] != numeric_limits<int>::max() &&
                    distances[u] + weight < distances[v]) {
                    // Node is part of or reachable from a negative cycle
                    inNegativeCycle[v] = true;
                    distances[v] = 0;  // Set to closest to 0 as specified
                    
                    // Propagate the negative cycle information
                    queue<int> q;
                    q.push(v);
                    while (!q.empty()) {
                        int current = q.front();
                        q.pop();
                        
                        for (const Pair& nextEdge : graph[current]) {
                            int next = nextEdge.first;
                            if (!inNegativeCycle[next]) {
                                inNegativeCycle[next] = true;
                                distances[next] = 0;
                                q.push(next);
                            }
                        }
                    }
                }
            }
        }
    }
}
