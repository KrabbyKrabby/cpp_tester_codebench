#include <iostream>
#include <vector>
#include <queue>
#include <limits>

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

            // Check for negative edges
            if (weight < 0) {
                distances[neighborNode] = numeric_limits<int>::min();
            } else if (distances[currentNode] + weight < distances[neighborNode]) {
                distances[neighborNode] = distances[currentNode] + weight;
                pq.push({distances[neighborNode], neighborNode});
            }
        }
    }

    return distances;
}