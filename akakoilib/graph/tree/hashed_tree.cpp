vector<u64> hs(n, 1); vector<u64> R(n+1);
rep(i, n+1) R[i] = randint(47, (1ull<<61)-1);
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