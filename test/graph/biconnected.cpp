// https://judge.yosupo.jp/submission/333228
// https://judge.yosupo.jp/submission/356482
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/graph/biconnected.cpp"

void solve() {
  int n, m; cin >> n >> m;
  vector<vector<int>> G(n);
  rep(i, m) {
    int a, b; cin >> a >> b;
    G[a].push_back(b);
    G[b].push_back(a);
  }
  BiConnectedCC bcc(G);
  vector<vector<int>> components;
  vector<bool> used(n, false);
  for (auto& edge_list : bcc.bc) {
    vector<int> res;
    for (auto [u, v] : edge_list) {
      res.push_back(u);
      res.push_back(v);
      used[u] = true;
      used[v] = true;
    }
    sort(res.begin(), res.end());
    res.erase(unique(res.begin(), res.end()), res.end());
    components.push_back(res);
  }
  rep(i, n) {
    if (!used[i]) {
      components.push_back({i});
    }
  }
  cout << components.size() << "\n";
  for (auto& comp : components) {
    cout << comp.size();
    for (int v : comp) cout << " " << v;
    cout << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}
