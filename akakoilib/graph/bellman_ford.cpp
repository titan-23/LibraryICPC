vector<ll> bellman_ford(vector<vector<pair<int, ll>>> G, int s) {
  int n = G.size();
  vector<ll> dist(n, INF); dist[s] = 0;
  bool negcycle = n > 0;
  rep(i, n) {
    bool upd = false;
    rep(v, n) for (auto &[x, c] : G[v]) {
      if (dist[v] == INF) continue; // !need
      if (chmin(dist[x], dist[v]+c)) upd = true;
    }
    if (!upd) { negcycle = false; break; }
  }
  if (negcycle) return {};
  return dist;
}