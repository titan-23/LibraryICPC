struct Bimatch {
    using vi = vector<int>;
    vector<vi> g;
    vi d, mc, used, vv;
    Bimatch(int n, int m) : g(n), mc(m, -1), used(n) {}
    void add(int u, int v) {
        g[u].push_back(v);
    }
    void bfs() {
        d.assign((int)g.size(), -1);
        queue<int> q;
        for (int i = 0; i < (int)g.size(); ++i) {
            if (!used[i]) {
                q.push(i);
                d[i] = 0;
            }
        }
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            for (auto e : g[x]) {
                int c = mc[e];
                if (c >= 0 && d[c] == -1) {
                    d[c] = d[x] + 1;
                    q.push(c);
                }
            }
        }
    }
    bool dfs(int x) {
        vv[x] = true;
        for (auto e : g[x]) {
            int c = mc[e];
            if (c < 0 || (!vv[c] && d[c] == d[x] + 1 && dfs(c))) {
                mc[e] = x;
                used[x] = true;
                return true;
            }
        }
        return false;
    }
    int match() {
        int ret = 0;
        while (true) {
            bfs();
            vv.assign((int)g.size(), false);
            int f = 0;
            for (int i = 0; i < (int)g.size(); ++i) {
                if (!used[i] && dfs(i)) {
                    f++;
                }
            }
            if (f == 0) return ret;
            ret += f;
        }
    }
};