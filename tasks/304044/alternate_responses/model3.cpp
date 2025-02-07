#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>

using namespace std;

vector<pair<int,int>> solve(vector<int> p, vector<int> q) {
    int n = p.size();
    vector<pair<int,int>> operations;
    vector<int> pos(n + 1);
    
    // Store current positions of elements in p
    for(int i = 0; i < n; i++) {
        pos[p[i]] = i;
    }
    
    for(int i = 0; i < n; i++) {
        if(p[i] != q[i]) {
            // Find position of q[i] in current p
            int j = pos[q[i]];
            
            // Update positions after swap
            pos[p[i]] = j;
            pos[p[j]] = i;
            
            // Perform swap
            swap(p[i], p[j]);
            
            // Record operation (1-based indexing)
            operations.push_back({i + 1, j + 1});
        }
    }
    
    return operations;
}

vector<vector<pair<int,int>>> solveAll(vector<vector<int>> p_s, vector<vector<int>> q_s) {
    vector<vector<pair<int,int>>> sol;
    for(int i = 0; i < p_s.size(); i++){
        sol.push_back(solve(p_s[i], q_s[i]));
    }
    return sol;
}
