#include <bits/stdc++.h>
using namespace std;

template <class T, T (*op)(T, T), T (*e)()> // for seg
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

    // for seg ---
    Segtree<T, op, e> seg, rseg;
    void build_seg(vector<T> a) {
        vector<T> b(a.size());
        rep(i, n) b[i] = a[hld[i]];
        seg = Segtree<T, op, e>(b);
        reverse(b.begin(), b.end());
        rseg = Segtree<T, op, e>(b);
    }
    T path_prod(int u, int v) {
        T lv = e(), rv = e();
        while (head[u] != head[v]) {
            if (dep[head[u]] > dep[head[v]]) {
                lv = op(lv, rseg.prod(n-in[u]-1, n-in[head[u]]));
                u = par[head[u]];
            } else {
                rv = op(seg.prod(in[head[v]], in[v]+1), rv);
                v = par[head[v]];
            }
        }
        if (dep[u] > dep[v]) {
            lv = op(lv, rseg.prod(n-in[u]-1, n-in[v]));
        } else {
            lv = op(lv, seg.prod(in[u], in[v]+1));
        }
        return op(lv, rv);
    }
    T get(int k) { return seg.get(in[k]); }
    void set(int k, T v) {
        seg.set(in[k], v);
        rseg.set(n-in[k]-1, v);
    }
    T subtree_prod(int v) { return seg.prod(in[v], out[v]); }
};
