// @param seen 探索済みフラグ
// @param sz 各頂点の部分木のサイズ
// vector<int> sz(N);
// vector<bool> seen(N);
// auto cd = [&] (auto &&cd, int root) -> void {
//   int centroid = TreeCentroid(G, root, seen, sz);
//   seen[centroid] = true;
//   // ここに処理を書く
//   for (int nxt : G[centroid]) if (!seen[nxt]) {
//     cd(cd, nxt);
//   }
// };
// cd(cd, root);
int TreeCentroid(const vector<vector<int>>& G, int root, vector<int>& seen, vector<int>& sz) {
  int n = G.size();
  if (sz.empty()) sz = vector<int>(n);
  if (seen.empty()) seen = vector<int>(n, false);
  auto dfs = [&](auto dfs, int now, int pre) -> int {
    sz[now] = 1;
    for (int nxt : G[now]) {
      if (nxt == pre || seen[nxt]) continue;
      sz[now] += dfs(dfs, nxt, now);
    }
    return sz[now];
  };
  int total = dfs(dfs, root, -1), centroid = root;
  auto dfs2 = [&](auto dfs2, int now, int pre) -> void {
    bool ok = (total - sz[now]) * 2 <= total;
    for (int nxt : G[now]) {
      if (nxt == pre || seen[nxt]) continue;
      dfs2(dfs2, nxt, now);
      if (sz[nxt] * 2 > total) ok = false;
    }
    if (ok) centroid = now;
  };
  dfs2(dfs2, root, -1);
  return centroid;
}