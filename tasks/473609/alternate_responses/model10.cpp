#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>

using namespace std;
typedef pair<int, int> Pair;

// Function to implement Dijkstra's algorithm with negative edge weights and cycle detection
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

    // Initialize distances with max values
    distances.assign(n, numeric_limits<int>::max());
    distances[start] = 0;

    // Relax edges up to (n-1) times (Bellman-Ford approach)
    for (int i = 0; i < n - 1; ++i) {
        for (int u = 0; u < n; ++u) {
            for (const Pair& edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;

                // Relax the edge if a shorter path is found
                if (distances[u] != numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                }
            }
        }
    }

    // Detect negative cycles and set distances to closest to 0
    for (int u = 0; u < n; ++u) {
        for (const Pair& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;

            // If further relaxation is possible, a negative cycle exists
            if (distances[u] != numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                distances[v] = 0; // Set distance to closest to 0
            }
        }
    }
}
