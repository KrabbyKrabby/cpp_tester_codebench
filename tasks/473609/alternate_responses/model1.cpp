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

    // Initialize distances and predecessor tracking
    distances.assign(n, numeric_limits<int>::max());
    vector<bool> inNegativeCycle(n, false);
    vector<int> predecessor(n, -1);
    distances[start] = 0;

    // First phase: Regular Bellman-Ford iterations
    for (int i = 0; i < n - 1; ++i) {
        bool relaxed = false;
        for (int u = 0; u < n; ++u) {
            if (distances[u] == numeric_limits<int>::max()) continue;
            
            for (const Pair& edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    predecessor[v] = u;
                    relaxed = true;
                }
            }
        }
        if (!relaxed) break;  // Early termination if no relaxation occurred
    }

    // Second phase: Detect and handle negative cycles
    vector<bool> visited(n, false);
    for (int u = 0; u < n; ++u) {
        if (distances[u] == numeric_limits<int>::max()) continue;
        
        for (const Pair& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (distances[u] != numeric_limits<int>::max() && 
                distances[u] + weight < distances[v]) {
                
                // Mark all nodes in the negative cycle
                int current = v;
                while (current != -1 && !visited[current]) {
                    visited[current] = true;
                    inNegativeCycle[current] = true;
                    current = predecessor[current];
                }
            }
        }
    }

    // Set distances for nodes in negative cycles to 0
    for (int i = 0; i < n; ++i) {
        if (inNegativeCycle[i]) {
            distances[i] = 0;
        }
    }
}
