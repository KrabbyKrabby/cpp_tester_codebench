#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <limits>

using namespace std;

vector<pair<int,int>> solve(vector<int> p, vector<int> q) {
    int n = p.size();
    vector<pair<int, int>> operations;
    for (int i = 0; i < n; ++i) {
        if (p[i] != q[i]) {
            int min_j = -1;
            int min_cost = numeric_limits<int>::max();
            for (int j = 0; j < n; ++j) {
                if (p[j] == q[i]) {
                    int cost = min(abs(i - j), abs(p[i] - p[j]));
                    if (cost < min_cost) {
                        min_cost = cost;
                        min_j = j;
                    }
                }
            }
            operations.push_back({i + 1, min_j + 1});
            swap(p[i], p[min_j]);
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
