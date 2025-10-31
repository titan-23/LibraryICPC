struct HLD {
    vector<vector<int>> G;
    int n, time;
    vector<int> size, par, dep, head, hld, in, out;
    HLD(vector<vector<int>> G, int root) : G(G), n(G.size()), time(0) {
        size.resize(n);
        par.resize(n);
        dep.resize(n);
        head.resize(n);
        hld.resize(n);
        in.resize(n);
        out.resize(n);
        dfs_sz(root, -1);
        head[root] = root;
        dfs_hld(root, -1);
    }
    void dfs_sz(int v, int p) {
        par[v] = p;
        size[v] = 1;
        if (G[v].size() && G[v][0] == p) swap(G[v][0], G[v].back());
        for (int &x : G[v]) { // need `&`
            if (x == p) continue;
            dep[x] = dep[v] + 1;
            dfs_sz(x, v);
            size[v] += size[x];
            if (size[G[v][0]] < size[x]) swap(G[v][0], x);
        }
    }
    void dfs_hld(int v, int p) {
        in[v] = time++;
        hld[in[v]] = v;
        for (int &x : G[v]) {
            if (x == p) continue;
            head[x] = (G[v][0] == x ? head[v] : x);
            dfs_hld(x, v);
        }
        out[v] = time;
    }

    int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }
    int lca(int u, int v) {
        while (1) {
            if (in[u] > in[v]) swap(u, v);
            if (head[u] == head[v]) return u;
            v = par[head[v]];
        }
    }
    int la(int v, int k) {
        while (1) {
            int h = head[v];
            if (in[v] - k >= in[h]) return hld[in[v] - k];
            k -= in[v] - in[h] + 1;
            v = par[h];
            if (v == -1) return -1;
        }
    }
    int jump(int s, int t, int k) {
        int x = lca(s, t);
        int d = dep[s] + dep[t] - 2*dep[x];
        if (k > d) return -1;
        if (dep[s] - dep[x] >= k) {
            return la(s, k);
        } else {
            return la(t, d-k);
        }
    }
    // {utov, s, t} // [s, t)
    vector<tuple<bool, int, int>>  path_prod(int u, int v) {
        vector<tuple<bool, int, int>> res;
        while (head[u] != head[v]) {
            if (dep[head[u]] > dep[head[v]]) {
                res.push_back({false, in[head[u]], in[u]+1});
                u = par[head[u]];
            } else {
                res.push_back({true, in[head[v]], in[v]+1});
                v = par[head[v]];
            }
        }
        if (dep[u] > dep[v]) {
            res.push_back({false, in[v], in[u]+1});
        } else {
            res.push_back({true, in[u], in[v]+1});
        }
        return res;
    }
    int get(int k) { return in[k]; }
    pair<int, int> subtree_prod(int v) { return {in[v], out[v]}; }
};