#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>

using namespace std;
typedef pair<int, int> Pair;

void dijkstra(int start, const vector<vector<Pair>>& graph, vector<int>& distances) {
    int n = graph.size();
    
    // Handle empty graph scenario
    if (n == 0) {
        throw invalid_argument("Graph is empty.");
    }
    
    // Check for invalid start node index
    if (start < 0 || start >= n) {
        throw out_of_range("Start node is out of range.");
    }

    // Initialize distances and relaxation count
    distances.assign(n, numeric_limits<int>::max());
    vector<int> relaxCount(n, 0);
    distances[start] = 0;

    // First phase: Regular edge relaxation (modified Bellman-Ford)
    bool hasChange;
    for (int i = 0; i < n; ++i) {
        hasChange = false;
        for (int u = 0; u < n; ++u) {
            if (distances[u] == numeric_limits<int>::max()) continue;
            
            for (const Pair& edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;
                
                // Prevent integer overflow
                if (distances[u] == numeric_limits<int>::max()) continue;
                
                long long newDist = static_cast<long long>(distances[u]) + weight;
                if (newDist < numeric_limits<int>::min()) newDist = numeric_limits<int>::min();
                if (newDist > numeric_limits<int>::max()) continue;
                
                if (newDist < distances[v]) {
                    distances[v] = static_cast<int>(newDist);
                    relaxCount[v]++;
                    hasChange = true;
                    
                    // If a vertex is relaxed more than n-1 times, it's affected by a negative cycle
                    if (relaxCount[v] >= n) {
                        distances[v] = 0;
                    }
                }
            }
        }
        
        // Early termination if no changes occurred
        if (!hasChange) break;
    }

    // Second phase: Mark all nodes reachable from negative cycles
    vector<bool> inNegativeCycle(n, false);
    for (int u = 0; u < n; ++u) {
        if (relaxCount[u] >= n) {
            inNegativeCycle[u] = true;
        }
    }

    // Propagate the effect of negative cycles
    for (int i = 0; i < n; ++i) {
        for (int u = 0; u < n; ++u) {
            for (const Pair& edge : graph[u]) {
                int v = edge.first;
                if (inNegativeCycle[u]) {
                    inNegativeCycle[v] = true;
                    distances[v] = 0;
                }
            }
        }
    }
}
