struct TwoSat {
  vector<vector<int>> g;
  TwoSat(int n) { g = vector<vector<int>>(2 * n); }
  // 条件 `i==fi || j==fj` を追加
  void add(int i, bool fi, int j, bool fj) {
    i = 2 * i + !fi, j = 2 * j + !fj;
    g[i ^ 1].push_back(j); g[j ^ 1].push_back(i);
  }
  vector<bool> solve() {
    auto [mem, ng, gr] = SccDecomposition(g);
    int n = g.size() / 2;
    vector<bool> ret(n);
    for (int i = 0; i < n; i++) {
      if (gr[i * 2] == gr[i * 2 + 1]) return vector<bool>();
      ret[i] = gr[i * 2] > gr[i * 2 + 1];
    }
    return ret;
  }
};