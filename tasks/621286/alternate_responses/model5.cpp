#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;
typedef pair<int, int> Pair;

vector<int> dijkstra(int start, const vector<vector<Pair>>& graph) {
    int n = graph.size();
    vector<int> distances(n, numeric_limits<int>::max());
    vector<int> visitCount(n, 0);
    vector<bool> visited(n, false);
    const int MAX_VISITS = n * 2; // Limit iterations per node
    const int MIN_DISTANCE = -1000000; // Threshold for negative distances

    priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        // Skip if we've visited this node too many times
        if (visitCount[currentNode] >= MAX_VISITS) {
            continue;
        }

        visitCount[currentNode]++;

        for (const Pair& neighbor : graph[currentNode]) {
            int neighborNode = neighbor.first;
            int weight = neighbor.second;

            // Skip if the neighbor has been visited too many times
            if (visitCount[neighborNode] >= MAX_VISITS) {
                continue;
            }

            // Check if the new distance would be too negative
            long long potentialDistance = (long long)distances[currentNode] + weight;
            if (potentialDistance < MIN_DISTANCE) {
                potentialDistance = MIN_DISTANCE;
            }

            // Update distance if it's smaller
            if (potentialDistance < distances[neighborNode]) {
                distances[neighborNode] = potentialDistance;
                pq.push({distances[neighborNode], neighborNode});
            }
        }
    }

    // Handle unreachable nodes
    for (int i = 0; i < n; ++i) {
        if (distances[i] == numeric_limits<int>::max()) {
            distances[i] = -1; // Mark as unreachable
        }
    }

    return distances;
}