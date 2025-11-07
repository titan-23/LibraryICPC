struct HLD {
  vector<vector<int>> G;
  int n, time = 0;
  vector<int> size, par, dep, head, hld, in, out;
  // in:  頂点idxのHLD配列上のインデックス(頂点→配列)
  // hld: HLD配列のidx番目の要素が元の木のどの頂点か(配列→頂点)
  HLD(vector<vector<int>> G, int root) : G(G), n(G.size()), time(0),
      size(n), par(n), dep(n), head(n), hld(n), in(n), out(n) {
    dfs_sz(root, -1);
    head[root] = root;
    dfs_hld(root, -1);
  }
  void dfs_sz(int v, int p) {
    par[v] = p;
    size[v] = 1;
    if (G[v].size() && G[v][0] == p) swap(G[v][0], G[v].back());
    for (int &x : G[v]) if (x != p) { // need `&`
      dep[x] = dep[v] + 1;
      dfs_sz(x, v);
      size[v] += size[x];
      if (size[G[v][0]] < size[x]) swap(G[v][0], x);
    }
  }
  void dfs_hld(int v, int p) {
    in[v] = time++;
    hld[in[v]] = v;
    for (int x : G[v]) if (x != p) {
      head[x] = (G[v][0] == x ? head[v] : x);
      dfs_hld(x, v);
    }
    out[v] = time;
  }

  int dist(int u, int v) { return dep[u] + dep[v] - 2*dep[lca(u, v)]; }
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
  vector<tuple<bool, int, int>>  path_prod(int u, int v) {
    // 半開区間!
    // true -> (uから遠い方、uに近い方)
    // false -> (uに近い方、uに遠い方)
    // rep(i, n) init[hld.in[i]] = A[i];
    // seg.set(hld.in[p], v);
    // auto path = hld.path_prod(u, v);
    // ll lv = e(), rv = e();
    // for (auto [f, l, r] : path) {
    //     if (f) lv = op(lv, seg_rev.prod(l, r));
    //     else rv = op_rev(rv, seg.prod(l, r));
    // }
    // ll res = op(lv, rv);
    vector<tuple<bool, int, int>> res;
    while (head[u] != head[v]) {
      if (dep[head[u]] > dep[head[v]]) {
        res.emplace_back(true, in[head[u]], in[u]+1);
        u = par[head[u]];
      } else {
        res.emplace_back(false, in[head[v]], in[v]+1);
        v = par[head[v]];
      }
    }
    if (dep[u] > dep[v]) res.emplace_back(true, in[v], in[u]+1);
    else res.emplace_back(false, in[u], in[v]+1);
    return res;
  }
  pair<int, int> subtree_prod(int v) { return {in[v], out[v]}; }
};