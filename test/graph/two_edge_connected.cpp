// https://judge.yosupo.jp/submission/356488
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/graph/lowlink.cpp"
#include "./../../akakoilib/graph/two_edge_connected.cpp"

void solve() {
  int n, m; cin >> n >> m;
  vector<vector<int>> G(n);
  rep(i, m) {
    int a, b; cin >> a >> b;
    G[a].push_back(b);
    G[b].push_back(a);
  }
  TwoEdgeCC tree(G);
  cout << tree.groups.size() << "\n";
  for (auto X : tree.groups) {
    cout << X.size() << " ";
    for (int x : X) cout << x << " ";
    cout << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}