using ull = unsigned long long;
ull maximum_independent_set(vector<vector<int>> G) {
  const int n = G.size();
  vector<ull> E(n);
  rep(i, n) {
    for (auto& e : G[i]) E[i] |= 1ull << e;
  }
  int ans = 0;
  ull res = 0;
  auto dfs = [&] (auto&& dfs, ull now, ull rest) -> void {
    pair<int, int> p = {-1, -1};
    while (true) {
      bool f = false;
      rep(v, n) if ((rest >> v) & 1) {
        int deg = popcount(E[v] & rest);
        chmax(p, pair<int, int>{deg, v});
        if (deg <= 1) {
          rest ^= 1ull<<v, rest &= ~E[v], now |= 1ull<<v;
          f = true;
        }
      }
      if (!f) break;
      p = {-1, -1};
    }
    const int sz1 = popcount(now), sz2 = popcount(rest);
    if (chmax(ans, sz1)) res = now;
    if (sz2 == 0 || sz1 + sz2 <= ans) return;
    const int v = p.second;
    rest &= ~(1ull << v);
    dfs(dfs, now, rest);
    now |= 1ull << v;
    dfs(dfs, now, rest & (~E[v]));
  };
  dfs(dfs, 0, (1ull << n) - 1);
  return res;
}