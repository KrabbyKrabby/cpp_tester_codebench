#include <iostream>
#include <vector>
#include <limits>

using namespace std;
typedef pair<int, int> Pair;

// Function to implement Bellman-Ford algorithm
void dijkstra(int start, const vector<vector<Pair>> &graph) {
    int n = graph.size();
    vector<int> distances(n, numeric_limits<int>::max()); 

    distances[start] = 0;

    // Relax edges repeatedly
    for (int i = 0; i < n - 1; ++i) {
        for (int u = 0; u < n; ++u) {
            for (const Pair &neighbor : graph[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (distances[u] != numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                }
            }
        }
    }

    // Check for negative-weight cycles
    for (int u = 0; u < n; ++u) {
        for (const Pair &neighbor : graph[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (distances[u] != numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                // Negative cycle detected
                distances[v] = numeric_limits<int>::min(); // Set to negative infinity
            }
        }
    }

    // Print the shortest distances from the start node
    cout << "Node\tDistance from start" << endl;
    for (int i = 0; i < n; ++i) {
        cout << i << "\t" << distances[i] << endl;
    }
}
