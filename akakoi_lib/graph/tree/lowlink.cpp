struct LowLink {
  int n;
  vector<int> ord, low, arti;
  vector<pair<int, int>> bridge;
  vector<vector<int>> G;
  LowLink(vector<vector<int>> G) : n(G.size()), G(G), ord(n, -1), low(n, -1) {
    int k = 0;
    rep(i, n) if (ord[i] == -1) {
      dfs(i, k, -1);
    }
  }
  int dfs(int x, int k, int p) {
    low[x] = (ord[x] = k++);
    int cnt = 0;
    bool is_arti = false, second = false;
    for (const int e : G[x]) {
      if (ord[e] == -1) {
        cnt++;
        k = dfs(e, k, x);
        low[x] = min(low[x], low[e]);
        is_arti |= (p != -1) && (low[e] >= ord[x]);
        if (ord[x] < low[e]) bridge.push_back(minmax(x, e));
      } else if (e != p || second) {
        low[x] = min(low[x], ord[e]);
      } else {
        second = true;
      }
    }
    is_arti |= p == -1 && cnt > 1;
    if (is_arti) arti.push_back(x);
    return k;
  }
};