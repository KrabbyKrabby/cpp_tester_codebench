#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
 
using namespace std;
 
vector<pair<int,int>> solve(vector<int> p, vector<int> q) {
    int n = p.size();
 
    vector<pair<int, int>> operations;
 
    for (int i = 0; i < n; ++i) {
        if (p[i] != q[i]) {
            int j = 0;
            for (int k = 0; k < n; ++k) {
                if (p[k] == q[i]) {
                    j = k;
                    break;
                }
            }
            operations.push_back({i + 1, j + 1});
            swap(p[i], p[j]);
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