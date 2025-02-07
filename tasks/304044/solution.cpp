#include <bits/stdc++.h>
using namespace std;

struct KactlSolver {
    int n;
    vector<int> PP, QQ;
    vector<vector<int>> cc;
    vector<int> vv, uu, dd, dd_, par;

    KactlSolver(int n, const vector<int>& p, const vector<int>& q)
        : n(n), PP(p), QQ(q),
          cc(n + 1, vector<int>(n + 1, 0)),
          vv(n + 1, 0), uu(n + 1, 0),
          dd(n + 1, INT_MAX), dd_(n + 1, 0), par(n + 1, -1)
    {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cc[i][j] = abs(j - i) + abs(QQ[j] - PP[i]);
            }
        }
    }

    void dijkstra() {
        fill(dd.begin(), dd.end(), INT_MAX);
        fill(par.begin(), par.end(), -1);
        vector<bool> visited(n + 1, false);
        for (int u = 1; u <= n; u++) if (vv[u] == 0) dd[u] = 0;
        while (true) {
            int best = -1;
            for (int u = 0; u <= n; u++) {
                if (!visited[u] && dd[u] != INT_MAX) {
                    if (best == -1 || dd[best] > dd[u]) best = u;
                }
            }
            if (best == -1) break;
            visited[best] = true;
            for (int v_ = 1; v_ <= n; v_++) {
                int w = uu[v_];
                int cost = dd[best] + cc[best][v_] - cc[w][v_] + dd_[best] - dd_[w];
                if (!visited[w] && dd[w] > cost) {
                    dd[w] = cost;
                    par[w] = best;
                }
            }
        }
    }

    void tracePath() {
        int w = 0, u = par[w], v;
        for (v = 1; v <= n; v++) {
            if (uu[v] == 0) {
                int cost = dd[u] + cc[u][v] - cc[w][v] + dd_[u] - dd_[w];
                if (dd[w] == cost) break;
            }
        }
        for (w = 0; (u = par[w]) != -1; w = u) {
            int t = vv[u];
            vv[u] = v;
            uu[v] = u;
            v = t;
        }
    }

    int edmondsKarp() {
        fill(vv.begin(), vv.end(), 0);
        fill(uu.begin(), uu.end(), 0);
        fill(dd_.begin(), dd_.end(), 0);
        int ans = 0;
        for (int _ = 1; _ <= n; _++) {
            dijkstra();
            tracePath();
            for (int i = 0; i <= n; i++) if (dd[i] != INT_MAX) dd_[i] += dd[i];
            ans += dd_[0];
        }
        return ans;
    }

    vector<pair<int,int>> solve() {
        edmondsKarp();
        vector<int> A(n * n), B(n * n);
        int cnt = 0;
        while (true) {
            int u = -1;
            for (int w = 1; w <= n; w++) {
                if (w < vv[w]) u = w;
                else if (w > vv[w]) {
                    A[cnt] = u; B[cnt] = w; cnt++;
                    int t = PP[u]; PP[u] = PP[w]; PP[w] = t;
                    t = vv[u]; vv[u] = vv[w]; vv[w] = t;
                    break;
                }
            }
            if (u == -1) break;
        }
        for (int i = 1; i <= n; i++) vv[PP[i]] = QQ[i];
        while (true) {
            int u = -1;
            for (int w = 1; w <= n; w++) {
                if (w < vv[w]) u = w;
                else if (w > vv[w]) {
                    int i, j;
                    for (i = 1; i <= n; i++) if (PP[i] == u) break;
                    for (j = 1; j <= n; j++) if (PP[j] == w) break;
                    A[cnt] = i; B[cnt] = j; cnt++;
                    int t = PP[i]; PP[i] = PP[j]; PP[j] = t;
                    t = vv[u]; vv[u] = vv[w]; vv[w] = t;
                    break;
                }
            }
            if (u == -1) break;
        }
        vector<pair<int,int>> res(cnt);
        for (int i = 0; i < cnt; i++) res[i] = {A[i], B[i]};
        return res;
    }
};

vector<vector<pair<int,int>>> solveAll(vector<vector<int>> p_s, vector<vector<int>> q_s) {
    vector<vector<pair<int,int>>> sol;
    int t = p_s.size();
    int j = -1;
    while (t--) {
        j++;
        int n = p_s[j].size();
        vector<int> p(n + 1), q(n + 1);
        for (int i = 1; i <= n; i++) p[i] = p_s[j][i - 1];
        for (int i = 1; i <= n; i++) q[i] = q_s[j][i - 1];;
        KactlSolver solver(n, p, q);
        auto ans = solver.solve();
        sol.emplace_back(ans);
    }
    return sol;
}