#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;
typedef pair<int, int> Pair;

// Function to implement Dijkstra's algorithm
vector<int> dijkstra(int start, const vector<vector<Pair>> &graph) {
    int n = graph.size();
    vector<int> distances(n, numeric_limits<int>::max()); 
    vector<bool> visited(n, false); 

    priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (visited[currentNode]) continue;
        visited[currentNode] = true;

        for (const Pair &neighbor : graph[currentNode]) {
            int neighborNode = neighbor.first;
            int weight = neighbor.second;

            if (distances[currentNode] + weight < distances[neighborNode]) {
                distances[neighborNode] = distances[currentNode] + weight;
                pq.push({distances[neighborNode], neighborNode});
            }
        }
    }

    // Find the minimum distance closest to 0
    int min_distance = numeric_limits<int>::max();
    for (int i = 0; i < n; ++i) {
        if (distances[i] < min_distance) {
            min_distance = distances[i];
        }
    }

    // Shift distances to make them non-negative
    for (int i = 0; i < n; ++i) {
        distances[i] -= min_distance;
    }

    return distances;
}