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
    vector<int> bestDistances(n, numeric_limits<int>::max());
    vector<int> visitCount(n, 0);
    const int MAX_VISITS = n * 2; // Limit the number of visits per node

    priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

    distances[start] = 0;
    bestDistances[start] = 0;
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

            // Check for potential overflow
            if (distances[currentNode] != numeric_limits<int>::max() &&
                distances[currentNode] + weight < distances[neighborNode]) {
                
                distances[neighborNode] = distances[currentNode] + weight;
                
                // Update best distance if current distance is better
                if (distances[neighborNode] < bestDistances[neighborNode]) {
                    bestDistances[neighborNode] = distances[neighborNode];
                }

                pq.push({distances[neighborNode], neighborNode});
            }
        }
    }

    // Replace unreachable nodes with numeric_limits<int>::max()
    for (int i = 0; i < n; ++i) {
        if (bestDistances[i] == numeric_limits<int>::max()) {
            bestDistances[i] = numeric_limits<int>::max();
        }
    }

    return bestDistances;
}