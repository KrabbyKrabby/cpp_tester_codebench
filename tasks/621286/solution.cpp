#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;

// Define an alias for a pair of integers
typedef pair<int, int> Pair;

// Function to implement Dijkstra's algorithm with handling for negative edges and closest to 0 logic
vector<int> dijkstra(int start, const vector<vector<Pair>> &graph) {
    int n = graph.size();

    vector<int> distances(n, numeric_limits<int>::max()); // Initialize distances to infinity
    vector<bool> visited(n, false); // Track visited nodes

    // Check if the given graph is empty
    if(n == 0)
    {
        return distances;
    }

    auto cmp = [](const Pair &a, const Pair &b) {
        int distA = abs(a.first), distB = abs(b.first);
        if (distA == distB) return a.first > b.first; // Prefer closer to zero
        return distA > distB;
    };

    priority_queue<Pair, vector<Pair>, decltype(cmp)> pq(cmp); // Custom min-heap priority queue

    distances[start] = 0;
    pq.push({0, start}); // Push the starting node

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (visited[currentNode]) continue;
        visited[currentNode] = true;

        // Update distances to adjacent nodes
        for (const Pair &neighbor : graph[currentNode]) {
            int neighborNode = neighbor.first;
            int weight = neighbor.second;
            int proposedDistance = currentDistance + weight;

            // Update distance if it's closer to zero or if it's a better distance
            if (abs(proposedDistance) < abs(distances[neighborNode]) ||
                (abs(proposedDistance) == abs(distances[neighborNode]) && proposedDistance > distances[neighborNode])) {
                distances[neighborNode] = proposedDistance;
                pq.push({proposedDistance, neighborNode});
            }
        }
    }

    return distances;
}