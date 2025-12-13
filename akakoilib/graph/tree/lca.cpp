int op_lca(int s, int t) { return min(s, t); }
int e_lca() { return 1e9; }
struct LCA {
  int n;
  vector<int> path, in;
  Segtree<int, op_lca, e_lca> seg;
  LCA(vector<vector<int>> G, int root) : n((int)G.size()), path(n), in(n, -1) {
    vector<int> par(n, -1), a(n);
    int time = -1, ptr = 0, s[n];
    s[ptr++] = root;
    while (ptr) {
      int v = s[--ptr];
      if (time >= 0) path[time] = par[v];
      ++time;
      in[v] = time;
      for (int x: G[v]) {
        if (in[x] != -1) continue;
        par[x] = v;
        s[ptr++] = x;
      }
    }
    rep(i, n) a[i] = in[path[i]];
    seg = Segtree<int, op_lca, e_lca>(a);
  }
  int lca(int u, int v) {
    if (u == v) return u;
    int l = in[u], r = in[v];
    if (l > r) swap(l, r);
    return path[seg.prod(l, r)];
  }
};