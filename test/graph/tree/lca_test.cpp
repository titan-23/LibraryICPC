// https://judge.yosupo.jp/submission/327584
// https://judge.yosupo.jp/submission/327583
#include "./../../../akakoilib/template/template.cpp"
#include "./../../../akakoilib/ds/segtree.cpp"
#include "./../../../akakoilib/graph/tree/lca.cpp"

void solve() {
  int n, q; cin >> n >> q;
  vector<vector<int>> G(n);
  rep(i, n-1) {
    int p; cin >> p;
    G[p].push_back(i+1);
    G[i+1].push_back(p);
  }
  LCA tree(G, 0);
  rep(i, q) {
    int u, v; cin >> u >> v;
    cout << tree.lca(u, v) << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}