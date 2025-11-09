// https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=11036283#1
#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/graph/lowlink.cpp"

void solve() {
  int n, m; cin >> n >> m;
  vector<vector<int>> G(n);
  rep(i, m) {
    int u, v; cin >> u >> v;
    G[u].push_back(v);
    G[v].push_back(u);
  }
  LowLink graph(G);
  for (auto [u, v] : graph.bridge) {
    cout << u << " " << v << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}