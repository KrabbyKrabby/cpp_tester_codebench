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

    // Initialize distances and visited array
    distances.assign(n, numeric_limits<int>::max());
    vector<bool> inNegativeCycle(n, false);
    distances[start] = 0;

    // First phase: Bellman-Ford for n-1 iterations
    for (int i = 0; i < n - 1; ++i) {
        bool relaxed = false;
        for (int u = 0; u < n; ++u) {
            if (distances[u] == numeric_limits<int>::max()) continue;
            
            for (const Pair& edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    relaxed = true;
                }
            }
        }
        if (!relaxed) break; // Early termination if no relaxation occurred
    }

    // Second phase: Detect and handle negative cycles
    vector<bool> visited(n, false);
    for (int i = 0; i < n; ++i) {
        for (int u = 0; u < n; ++u) {
            if (distances[u] == numeric_limits<int>::max()) continue;
            
            for (const Pair& edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (distances[u] + weight < distances[v]) {
                    distances[v] = 0;
                    inNegativeCycle[v] = true;
                    
                    // Propagate the negative cycle effect
                    queue<int> q;
                    q.push(v);
                    visited[v] = true;

                    while (!q.empty()) {
                        int current = q.front();
                        q.pop();

                        for (const Pair& nextEdge : graph[current]) {
                            int next = nextEdge.first;
                            if (!visited[next]) {
                                distances[next] = 0;
                                inNegativeCycle[next] = true;
                                visited[next] = true;
                                q.push(next);
                            }
                        }
                    }
                }
            }
        }
    }
}
