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
    const int MAX_VISITS = n * 2; // Limit iterations per node

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

            // Calculate new distance, ensuring it doesn't go below 0
            int newDistance = max(0, distances[currentNode] + weight);

            // Update distance if it's smaller and we haven't exceeded visit limit
            if (newDistance < distances[neighborNode] && visitCount[neighborNode] < MAX_VISITS) {
                distances[neighborNode] = newDistance;
                pq.push({distances[neighborNode], neighborNode});
            }
        }
    }

    // Handle unreachable nodes
    for (int i = 0; i < n; i++) {
        if (distances[i] == numeric_limits<int>::max()) {
            distances[i] = -1; // Mark as unreachable
        }
    }

    return distances;
}
