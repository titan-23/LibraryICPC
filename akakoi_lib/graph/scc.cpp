struct SccInfo {
  vector<vector<int>> members;  ///< 各強連結成分の頂点
  vector<vector<int>> graph_decomposed;  ///< 強連結成分による分解グラフ
  vector<int> group;  ///< 各頂点の所属する強連結成分の番号
};
SccInfo SccDecomposition(const vector<vector<int>>& g) {
  int n = g.size();
  vector<vector<int>> g2(n);
  rep(i, n) for (int j : g[i]) g2[j].push_back(i);
  vector<int> order, component(n, -1), seen(n, false);
  auto dfs = [&](auto dfs, int now) -> void {
    seen[now] = true;
    for (int nxt : g[now]) if (!seen[nxt]) dfs(dfs, nxt);
    order.push_back(now);
  };
  auto dfs2 = [&](auto dfs2, int now, int idx) -> void {
    component[now] = idx;
    for (int nxt : g2[now]) if (component[nxt] == -1) dfs2(dfs2, nxt, idx);
  };
  rep(i, n) if (!seen[i]) dfs(dfs, i);
  reverse(order.begin(), order.end());
  int idx = 0;
  for (int now : order) if (component[now] == -1) dfs2(dfs2, now, idx), idx++;
  int n_n = *max_element(component.begin(), component.end()) + 1;
  vector<vector<int>> ret(n_n), ret2(n_n);
  rep(i, n) ret[component[i]].push_back(i);
  rep(i, n) for (int j : g[i]) if (component[i] != component[j]) ret2[component[i]].push_back(component[j]);
  rep(i, n_n) sort(ret2[i].begin(), ret2[i].end()), ret2[i].erase(unique(ret2[i].begin(), ret2[i].end()), ret2[i].end());
  return {ret, ret2, component};
}