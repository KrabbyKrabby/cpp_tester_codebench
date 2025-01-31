#include <iostream>
#include <vector>
#include <limits>

using namespace std;
typedef pair<int, int> Pair;

// Function to implement Bellman-Ford algorithm
bool bellmanFord(int start, const vector<vector<Pair>> &graph) {
    int n = graph.size();
    vector<int> distances(n, numeric_limits<int>::max()); 

    distances[start] = 0;

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n; ++j) {
            for (const Pair &neighbor : graph[j]) {
                int neighborNode = neighbor.first;
                int weight = neighbor.second;

                if (distances[j] != numeric_limits<int>::max() && distances[j] + weight < distances[neighborNode]) {
                    distances[neighborNode] = distances[j] + weight;
                }
            }
        }
    }

    // Check for negative-weight cycles
    for (int i = 0; i < n; ++i) {
        for (const Pair &neighbor : graph[i]) {
            int neighborNode = neighbor.first;
            int weight = neighbor.second;

            if (distances[i] != numeric_limits<int>::max() && distances[i] + weight < distances[neighborNode]) {
                return false;
            }
        }
    }

    // Print the shortest distances from the start node
    cout << "Node\tDistance from start" << endl;
    for (int i = 0; i < n; ++i) {
        cout << i << "\t" << distances[i] << endl;
    }

    return true;
}