#include <iostream>
#include <vector>
#include <limits>
#include <stdexcept>
#include <queue>

using namespace std;
typedef pair<int, int> Pair;

// Function to implement Bellman-Ford algorithm
void dijkstra(int start, const vector<vector<Pair>> &graph, vector<int> &distances) {
    int n = graph.size();

    // Validate input
    if (n == 0) {
        throw std::invalid_argument("Graph is empty.");
    }
    if (start < 0 || start >= n) {
        throw std::out_of_range("Start node is out of range.");
    }
    

    // Initialize distances
    distances.resize(n, numeric_limits<int>::max());
    distances[start] = 0;

    // Step 1: Relax all edges (n-1) times
    for (int i = 0; i < n - 1; ++i) {
        bool updated = false;
        for (int u = 0; u < n; ++u) {
            if (distances[u] != numeric_limits<int>::max()) {
                for (const Pair &neighbor : graph[u]) {
                    int v = neighbor.first;
                    int weight = neighbor.second;

                    // Relax the edge
                    if (distances[u] + weight < distances[v]) {
                        distances[v] = distances[u] + weight;
                        updated = true;
                    }
                }
            }
        }
        // Stop early if no updates
        if (!updated) break;
    }

    // Step 2: Detect negative weight cycles
    vector<bool> inNegativeCycle(n, false); // To track nodes in negative cycles
    for (int u = 0; u < n; ++u) {
        if (distances[u] != numeric_limits<int>::max()) {
            for (const Pair &neighbor : graph[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                // If we can still relax the edge, mark the nodes involved
                if (distances[u] + weight < distances[v]) {
                    inNegativeCycle[v] = true;
                }
            }
        }
    }

    // Step 3: Propagate negative cycle effect
    queue<int> q;
    vector<bool> visited(n, false);

    // Add all nodes in negative cycles to the queue
    for (int i = 0; i < n; ++i) {
        if (inNegativeCycle[i]) {
            q.push(i);
            visited[i] = true;
        }
    }

    // Perform BFS to mark all reachable nodes from negative cycles
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        // Set the distance of nodes in or reachable from negative cycles to 0
        distances[u] = 0;

        for (const Pair &neighbor : graph[u]) {
            int v = neighbor.first;
            if (!visited[v]) {
                q.push(v);
                visited[v] = true;
            }
        }
    }
}