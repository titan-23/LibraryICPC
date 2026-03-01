struct TwoSat {
  vector<vector<int>> G;
  TwoSat(int n) : G(2*n) {}
  // 条件 `i==fi || j==fj` を追加
  void add(int i, bool fi, int j, bool fj) {
    i = 2*i + !fi, j = 2*j + !fj;
    G[i^1].push_back(j); G[j^1].push_back(i);
  }
  vector<bool> solve() {
    auto [mem, ng, gr] = SccDecomposition(G);
    int n = G.size() / 2;
    vector<bool> ret(n);
    rep(i, n) {
      if (gr[i*2] == gr[i*2+1]) return vector<bool>();
      ret[i] = gr[i*2] > gr[i*2+1];
    }
    return ret;
  }
};