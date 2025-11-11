// https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=11044716#1
#include "./../../akakoi_lib/template/template.cpp"
const ll INF = 1e18;
#include "./../../akakoi_lib/graph/bellman_ford.cpp"

void solve() {
  int n, m, r; cin >> n >> m >> r;
  vector<vector<pair<int, ll>>> G(n);
  rep(_, m) {
    int s, t, d; cin >> s >> t >> d;
    G[s].push_back({t, d});
  }
  auto dist = bellman_ford(G, r);
  if (dist.empty()) {
    cout << "NEGATIVE CYCLE" << endl;
    return;
  }
  rep(i, n) {
    if (dist[i] < INF) {
        cout << dist[i] << "\n";
    } else {
        cout << "INF" << "\n";
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}