#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;
typedef pair<int, int> Pair;

// Function to implement Dijkstra's algorithm
void dijkstra(int start, const vector<vector<Pair>> &graph) {
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

    // Print the shortest distances from the start node
    cout << "Node\tDistance from start" << endl;
    for (int i = 0; i < n; ++i) {
        cout << i << "\t" << distances[i] << endl;
    }
}