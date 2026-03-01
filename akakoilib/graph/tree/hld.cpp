struct HLD {
  vector<vector<int>> G;
  int n, t = 0;
  vector<int> size, par, dep, head, hld, in, out;
  // in:  頂点idxのHLD配列上のインデックス(頂点→配列)
  // hld: HLD配列のidx番目の要素が元の木のどの頂点か(配列→頂点)
  HLD(vector<vector<int>> G, int root) : G(G), n(G.size()), t(0), size(n, 1), par(n), dep(n), head(n), hld(n), in(n), out(n) {
    dfs_sz(root, -1);
    head[root] = root;
    dfs_hld(root, -1);
  }
  void dfs_sz(int v, int p) {
    par[v] = p;
    if (G[v].size() && G[v][0] == p) swap(G[v][0], G[v].back());
    for (int &x : G[v]) if (x != p) { // need `&`
      dep[x] = dep[v] + 1;
      dfs_sz(x, v);
      size[v] += size[x];
      if (size[G[v][0]] < size[x]) swap(G[v][0], x);
    }
  }
  void dfs_hld(int v, int p) {
    in[v] = t++;
    hld[in[v]] = v;
    for (int x : G[v]) if (x != p) {
      head[x] = (G[v][0] == x ? head[v] : x);
      dfs_hld(x, v);
    }
    out[v] = t;
  }
  int dist(int u, int v) { return dep[u]+dep[v]-2*dep[lca(u, v)]; }
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
    return (dep[s]-dep[x]>=k) ? la(s, k) : la(t, d-k);
  }
  vector<tuple<bool, int, int>>  path_prod(int u, int v) {
    // true -> (uから遠い方、uに近い方)
    // false -> (uに近い方、uに遠い方)
    // T op_rev(s, t) { return op(t, s); }
    // rep(i, n) v[hld.in[i]] = A[i];
    // Segtree<T,op,e> seg(v); Segtree<T,op_rev,e> rev(v);
    // seg.set(hld.in[p], v); rev.set(hld.in[p], v);
    // T lv = e(), rv = e(); // 半開区間!
    // for (auto [f, l, r] : hld.path_prod(u, v)) {
    //     if (f) lv = op(lv, rev.prod(l, r));
    //     else rv = op_rev(rv, seg.prod(l, r));
    // }
    // T res = op(lv, rv);
    vector<tuple<bool, int, int>> res;
    while (head[u] != head[v]) {
      if (dep[head[u]] > dep[head[v]]) {
        res.emplace_back(1, in[head[u]], in[u]+1);
        u = par[head[u]];
      } else {
        res.emplace_back(0, in[head[v]], in[v]+1);
        v = par[head[v]];
      }
    }
    if (dep[u] > dep[v]) res.emplace_back(1, in[v], in[u]+1);
    else res.emplace_back(0, in[u], in[v]+1);
    return res;
  }
  pair<int, int> subtree_prod(int v) {return {in[v], out[v]};}
};