#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;
typedef pair<int, int> Pair;

vector<int> dijkstra(int start, const vector<vector<Pair>> &graph) {
    int n = graph.size();
    vector<int> distances(n, numeric_limits<int>::max());
    vector<int> visitCount(n, 0);
    const int MAX_VISITS = n * 2; // Limit visits to prevent infinite loops
    
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
        
        for (const Pair &neighbor : graph[currentNode]) {
            int neighborNode = neighbor.first;
            int weight = neighbor.second;
            
            // Check for potential overflow
            if (distances[currentNode] != numeric_limits<int>::max() &&
                distances[currentNode] + weight < distances[neighborNode]) {
                
                // Update distance only if it's better than current
                int newDistance = distances[currentNode] + weight;
                if (newDistance < distances[neighborNode]) {
                    distances[neighborNode] = newDistance;
                    pq.push({distances[neighborNode], neighborNode});
                }
            }
        }
    }
    
    // Handle unreachable nodes
    for (int i = 0; i < n; ++i) {
        if (distances[i] == numeric_limits<int>::max()) {
            distances[i] = -1; // Mark unreachable nodes with -1
        }
    }
    
    return distances;
}