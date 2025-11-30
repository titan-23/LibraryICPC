struct DSUonTree {
private:
  vector<vector<int>> G;
  int n;
  vector<int> sz, euler, in, out;
  int t, root;
  int dfs1(int v, int p=-1) {
    sz[v] = 1;
    if (G[v].size() >= 2 && G[v][0] == p) {
      swap(G[v][0], G[v][1]);
    }
    for (int &x : G[v]) { // need `&`
      if (x == p) continue;
      sz[v] += dfs1(x, v);
      if (sz[x] > sz[G[v][0]]) swap(x, G[v][0]);
    }
    return sz[v];
  }
  void dfs2(int v, int p=-1) {
    euler[t] = v;
    in[v] = t++;
    for (int x : G[v]) if (x != p) {
      dfs2(x, v);
    }
    out[v] = t;
  }
public:
  DSUonTree(vector<vector<int>> G, int root=0)
      : G(G), n(G.size()),
        sz(n), euler(n), in(n), out(n),
        t(0), root(root) {
    dfs1(root);
    dfs2(root);
  }
  int idx(int u) const { return in[u]; }
  template <typename UPDATE, typename QUERY, typename CLEAR, typename RESET>
  void run(UPDATE &outdate, QUERY &query, CLEAR &clear, RESET &reset) {
    auto dsu = [&](auto dsu, int v, int p=-1, bool keep=false) -> void {
      for (int i = 1; i < G[v].size(); i++) if (G[v][i] != p) {
        dsu(dsu, G[v][i], v, false);
      }
      if (sz[v] != 1) dsu(dsu, G[v][0], v, true);
      if (sz[v] != 1) {
        for (int i = out[G[v][0]]; i < out[v]; i++) outdate(euler[i]);
      }
      outdate(v);
      query(v);
      if (!keep) {
        for (int i = in[v]; i < out[v]; i++) clear(euler[i]);
        reset();
      }
    };
    dsu(dsu, root);
  }
};