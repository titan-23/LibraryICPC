struct LowLink {
  int n;
  vector<int> ord, low, arti;
  vector<pair<int, int>> bridge;
  vector<vector<int>> G;
  LowLink(vector<vector<int>> G) : n(G.size()), G(G), ord(n, -1), low(n, -1) {
    int k = 0;
    rep(i, n) if (ord[i] == -1) dfs(i, k, -1);
    sort(arti.begin(), arti.end());
    sort(bridge.begin(), bridge.end());
  }
  int dfs(int v, int k, int p) {
    low[v] = (ord[v] = k++);
    int cnt = 0;
    bool is_arti = false, second = false;
    for (int x : G[v]) {
      if (ord[x] == -1) {
        cnt++;
        k = dfs(x, k, v);
        chmin(low[v], low[x]);
        is_arti |= (p != -1) && (low[x] >= ord[v]);
        if (ord[v] < low[x]) bridge.push_back(minmax(v, x));
      } else if (x != p || second) {
        chmin(low[v], ord[x]);
      } else {
        second = true;
      }
    }
    is_arti |= p == -1 && cnt > 1;
    if (is_arti) arti.push_back(v);
    return k;
  }
};