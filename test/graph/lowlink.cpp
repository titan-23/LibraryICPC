#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/graph/lowlink.cpp"

void solve() {
  int n, m; cin >> n >> m;
  vector<vector<int>> G(n);
  rep(i, m) {
    int u, v; cin >> u >> v;
    G[u].push_back(v);
    G[v].push_back(u);
  }
  LowLink graph(G);
  for (int v : graph.arti) {
    cout << v << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}