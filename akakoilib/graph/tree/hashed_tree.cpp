hs.resize(n, 1); vector<u64> R(n+1);
random_device rd; mt19937_64 rnd(seed == -1 ? rd() : seed);
uniform_int_distribution<u64> dist(47, (1ull<<61)-1);
rep(i, n+1) R[i] = dist(rnd);
auto dfs = [&] (auto &&dfs, int v, int p) -> int {
  int d = 0;
  for (int x : G[v]) if (x != p) {
    int dx = dfs(dfs, x, v);
    d = max(d, dx);
    hs[v] = mul(hs[v], add(R[dx], hs[x]));
  }
  return d+1;
};
dfs(dfs, root, -1);