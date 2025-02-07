#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <set>

using namespace std;
typedef pair<int, int> Pair;

void dijkstra(int start, const vector<vector<Pair>>& graph, vector<int>& distances) {
    int n = graph.size();
    
    // Handle empty graph
    if (n == 0) {
        throw invalid_argument("Graph is empty.");
    }
    
    // Validate start node
    if (start < 0 || start >= n) {
        throw out_of_range("Start node is out of range.");
    }

    // Initialize distances and visited count
    distances.assign(n, numeric_limits<int>::max());
    distances[start] = 0;
    
    vector<int> visitCount(n, 0);
    vector<bool> inQueue(n, false);
    queue<int> q;
    
    // Start with initial node
    q.push(start);
    inQueue[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inQueue[u] = false;
        
        // Track visit count for cycle detection
        visitCount[u]++;
        
        // If node has been visited too many times, it's in a negative cycle
        if (visitCount[u] > n) {
            distances[u] = 0;
            continue;
        }

        for (const Pair& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            
            // Prevent integer overflow
            if (distances[u] != numeric_limits<int>::max()) {
                int newDist;
                if (weight >= 0) {
                    if (distances[u] > numeric_limits<int>::max() - weight) {
                        continue;  // Skip if would overflow
                    }
                    newDist = distances[u] + weight;
                } else {
                    if (distances[u] < numeric_limits<int>::lowest() - weight) {
                        continue;  // Skip if would underflow
                    }
                    newDist = distances[u] + weight;
                }

                // Update distance if shorter path found
                if (newDist < distances[v]) {
                    distances[v] = newDist;
                    
                    // Add to queue if not already in it
                    if (!inQueue[v]) {
                        q.push(v);
                        inQueue[v] = true;
                    }
                }
            }
        }
    }

    // Final pass to detect and handle remaining negative cycles
    for (int i = 0; i < n; i++) {
        if (visitCount[i] > n) {
            // Set distances of all nodes reachable from negative cycle to 0
            queue<int> cycleQ;
            vector<bool> visited(n, false);
            cycleQ.push(i);
            
            while (!cycleQ.empty()) {
                int u = cycleQ.front();
                cycleQ.pop();
                
                if (visited[u]) continue;
                visited[u] = true;
                distances[u] = 0;
                
                for (const Pair& edge : graph[u]) {
                    if (!visited[edge.first]) {
                        cycleQ.push(edge.first);
                    }
                }
            }
        }
    }
}
