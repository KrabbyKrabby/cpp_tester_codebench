#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

vector<pair<int,int>> solve(vector<int> p, vector<int> q) {
    int n = p.size();
    vector<pair<int,int>> operations;
    
    for (int i = 0; i < n; ++i) {
        if (p[i] != q[i]) {
            int best_j = -1;
            int min_cost = INT_MAX;
            
            // Find all positions where q[i] exists in p
            for (int j = i + 1; j < n; ++j) {
                if (p[j] == q[i]) {
                    // Calculate cost as min(|i-j|, |pi-pj|)
                    int cost = min(abs(i - j), abs(p[i] - p[j]));
                    if (cost < min_cost) {
                        min_cost = cost;
                        best_j = j;
                    }
                }
            }
            
            if (best_j != -1) {
                operations.push_back({i + 1, best_j + 1}); // +1 because indices are 1-based
                swap(p[i], p[best_j]);
            }
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
