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
    vector<int> bestDistances(n, numeric_limits<int>::max());
    const int MAX_VISITS = n * 2; // Limit iterations per node

    priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

    distances[start] = 0;
    bestDistances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        visitCount[currentNode]++;
        
        // Skip if we've visited this node too many times
        if (visitCount[currentNode] > MAX_VISITS) {
            continue;
        }

        for (const Pair& neighbor : graph[currentNode]) {
            int neighborNode = neighbor.first;
            int weight = neighbor.second;

            // Check if we found a shorter path
            if (distances[currentNode] != numeric_limits<int>::max() &&
                distances[currentNode] + weight < distances[neighborNode]) {
                
                distances[neighborNode] = distances[currentNode] + weight;
                
                // Update best distance if current is closer to 0
                if (abs(distances[neighborNode]) < abs(bestDistances[neighborNode])) {
                    bestDistances[neighborNode] = distances[neighborNode];
                }

                pq.push({distances[neighborNode], neighborNode});
            }
        }
    }

    // Return the best distances found
    for (int i = 0; i < n; i++) {
        if (bestDistances[i] == numeric_limits<int>::max()) {
            distances[i] = numeric_limits<int>::max();
        } else {
            distances[i] = bestDistances[i];
        }
    }

    return distances;
}