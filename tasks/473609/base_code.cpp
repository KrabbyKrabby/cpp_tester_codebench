#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept> // For throwing exceptions

using namespace std;
typedef pair<int, int> Pair;

// Function to implement Dijkstra's algorithm
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

    vector<bool> visited(n, false);

    // Initialize the distances with max values
    distances.assign(n, numeric_limits<int>::max());
    distances[start] = 0;

    // Min-heap priority queue for Dijkstra
    priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
    pq.push({ 0, start });

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        // Skip the node if it's already visited
        if (visited[currentNode]) continue;
        visited[currentNode] = true;

        // Relax the edges
        for (const Pair& neighbor : graph[currentNode]) {
            int neighborNode = neighbor.first;
            int weight = neighbor.second;

            // Update the distance if a shorter path is found
            if (distances[currentNode] + weight < distances[neighborNode]) {
                distances[neighborNode] = distances[currentNode] + weight;
                pq.push({ distances[neighborNode], neighborNode });
            }
        }
    }
}