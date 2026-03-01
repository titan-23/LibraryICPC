// https://judge.yosupo.jp/submission/327582
// https://judge.yosupo.jp/submission/356743
#include "./../../../akakoilib/template/template.cpp"
#include "./../../../akakoilib/graph/tree/hld.cpp"

void solve() {
  int n, q; cin >> n >> q;
  vector<vector<int>> G(n);
  rep(i, n-1) {
    int u, v; cin >> u >> v;
    G[u].push_back(v);
    G[v].push_back(u);
  }
  HLD hld(G, 0);
  rep(i, q) {
    int s, t, k; cin >> s >> t >> k;
    cout << hld.jump(s, t, k) << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}
