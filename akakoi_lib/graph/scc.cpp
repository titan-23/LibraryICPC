struct SCC {
  // {groups, G, ids_inv, ids}
  // scc, 頂点を縮約した隣接リスト, 新たなグラフの頂点->もとの頂点集合, もとの頂点->新たなグラフの頂点
  vector<vector<int>> groups, G, ids_inv;
  vector<int> ids;
};
SCC get_scc_graph(const vector<vector<int>> G) {
  int n = G.size();
  vector<int> st(n, 0), lowlink(n, -1), order(n, -1), ids(n, 0);
  int ptr = 0, time = 0, group_cnt = 0;
  auto dfs = [&] (auto &&dfs, int v) -> void {
    order[v] = time;
    lowlink[v] = time;
    time++;
    st[ptr] = v;
    ptr++;
    for (int x : G[v]) {
      if (order[x] == -1) {
        dfs(dfs, x);
        lowlink[v] = min(lowlink[v], lowlink[x]);
      } else {
        lowlink[v] = min(lowlink[v], order[x]);
      }
    }
    if (lowlink[v] == order[v]) {
      while (1) {
        int u = st[ptr-1];
        ptr--;
        order[u] = n;
        ids[u] = group_cnt;
        if (u == v) break;
      }
      group_cnt++;
    }
  };
  rep(v, n) if (order[v] == -1) {
    dfs(dfs, v);
  }
  vector<vector<int>> groups(group_cnt);
  rep(v, n) groups[group_cnt-1-ids[v]].emplace_back(v);
  vector<vector<int>> F(*max_element(ids.begin(), ids.end())+1);
  rep(v, n) {
    for (int x : G[v]) if (ids[v] != ids[x]) {
      F[ids[v]].emplace_back(ids[x]);
    }
  }
  for (vector<int> &f : F) {
    sort(f.begin(), f.end());
    f.erase(unique(f.begin(), f.end()), f.end());
  }
  vector<vector<int>> ids_inv(F.size());
  rep(i, ids.size()) ids_inv[ids[i]].emplace_back(i);
  return {groups, F, ids_inv, ids};
}