// その成分内の任意の辺を2本削除しても、成分内の連結性が保たれる
// 成分内の任意の2頂点間には、辺素なパスが3本以上存在する
// 成分内に橋もサイズ2のカット(切ると非連結になる2本の辺の組)も含まない
// 縮約後の構造は木とは限らない
struct ThreeEdgeCC {
  int n;
  vector<vector<int>> G;
  vector<vector<int>> groups; // 各成分の頂点リスト
  vector<int> id;             // id[v]: 頂点vが属する成分ID
  vector<vector<int>> tree;   // 縮約後の単純グラフ (隣接リスト)
  vector<int> pre, post, path, low, deg, nxt, seen;
  int id_gen;
  ThreeEdgeCC(vector<vector<int>> G) : n(G.size()), G(G), id(n, -1), pre(n, -1), post(n), path(n, n), low(n, n), deg(n, 0), nxt(n), seen(n, 0), id_gen(0) {
    iota(nxt.begin(), nxt.end(), 0);
    rep(i, n) if (!seen[i]) dfs(i, n);
    fill(seen.begin(), seen.end(), 0);
    rep(i, n) if (!seen[i]) {
      groups.push_back({});
      int gid = groups.size() - 1, u = i;
      while (1) {
        seen[u] = 1; id[u] = gid;
        groups.back().push_back(u);
        u = nxt[u];
        if (u == i) break;
      }
    }
    tree.resize(groups.size());
    rep(v, n) for (int x : G[v]) if (id[v] != id[x]) {
      tree[id[v]].push_back(id[x]);
    }
    for (auto &t : tree) {
      sort(t.begin(), t.end());
      t.erase(unique(t.begin(), t.end()), t.end());
    }
  }
  void absorb(int u, int v) { deg[u] += deg[v]; swap(nxt[u], nxt[v]); }
  void dfs(int v, int p) {
    seen[v] = 1; pre[v] = id_gen++;
    for (int x : G[v]) if (x != v) {
      if (x == p) { p = n; continue; }
      if (seen[x]) {
        if (pre[x] < pre[v]) {
          low[v] = min(low[v], pre[x]); deg[v]++;
        } else {
          int w = path[v]; deg[v]--;
          while (w != n && pre[w] <= pre[x] && pre[x] < post[w]) { absorb(v, w); w = path[w]; }
          path[v] = w;
        }
        continue;
      }
      dfs(x, v);
      if (path[x] == n && deg[x] <= 1) {
        deg[v] += deg[x];
        low[v] = min(low[v], low[x]);
        continue;
      }
      if (deg[x] == 0) x = path[x];
      if (low[v] > low[x]) { low[v] = low[x]; swap(x, path[v]); }
      while (x != n) { absorb(v, x); x = path[x]; }
    }
    post[v] = id_gen;
  }
};