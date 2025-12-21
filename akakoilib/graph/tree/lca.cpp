struct LCA {
  int n; vector<int> path, in;
  Segtree<int, op, e> seg;
  LCA(vector<vector<int>> G, int root) : n((int)G.size()), path(n), in(n, -1) {
    vector<int> p(n, -1), a(n);
    int time = -1, ptr = 0, s[n];
    s[ptr++] = root;
    while (ptr) {
      int v = s[--ptr];
      if (time >= 0) path[time] = p[v];
      in[v] = ++time;
      for (int x: G[v]) if (in[x] == -1) {
        p[x] = v; s[ptr++] = x;
      }
    }
    rep(i, n) a[i] = in[path[i]];
    seg = Segtree<int, op, e>(a);
  }
  int lca(int u, int v) {
    if (u == v) return u;
    int l = in[u], r = in[v];
    if (l > r) swap(l, r);
    return path[seg.prod(l, r)];
  }
};