/**
 * @brief 重心分解
 * @param seen 探索済みフラグ
 * @param sz 各頂点の部分木のサイズ
 *
 * ```cpp
 * vector<int> sz(N);
 * vector<bool> seen(N);
 * auto centroid_decomposition = [&](auto self, int root) -> void {
 *     int centroid = TreeCentroid(g, root, seen, sz);
 *     seen[centroid] = true;
 *
 *     // ここに処理を書く
 *
 *     for (int nxt : g[centroid]) {
 *         if (seen[nxt]) continue;
 *         self(self, nxt);
 *     }
 * };
 * centroid_decomposition(centroid_decomposition, 0);
 * ```
 */
int TreeCentroid(const vector<vector<int>>& g, int root, vector<int>& seen, vector<int>& sz) {
  int n = g.size();
  if (sz.empty()) sz = vector<int>(n);
  if (seen.empty()) seen = vector<int>(n, false);
  auto dfs = [&](auto dfs, int now, int pre) -> int {
    sz[now] = 1;
    for (int nxt : g[now]) {
      if (nxt == pre || seen[nxt]) continue;
      sz[now] += dfs(dfs, nxt, now);
    }
    return sz[now];
  };
  int total = dfs(dfs, root, -1), centroid = root;
  auto dfs2 = [&](auto dfs2, int now, int pre) -> void {
    bool ok = (total - sz[now]) * 2 <= total;
    for (int nxt : g[now]) {
      if (nxt == pre || seen[nxt]) continue;
      dfs2(dfs2, nxt, now);
      if (sz[nxt] * 2 > total) ok = false;
    }
    if (ok) centroid = now;
  };
  dfs2(dfs2, root, -1);
  return centroid;
}