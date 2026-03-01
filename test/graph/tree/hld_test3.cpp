// https://judge.yosupo.jp/submission/356742
// Vertex Add Subtree Sum
#include "./../../../akakoilib/template/template.cpp"

#include "./../../../akakoilib/ds/fenwick_tree.cpp"
#include "./../../../akakoilib/graph/tree/hld.cpp"

void solve() {
  int n, q; cin >> n >> q;
  vector<ll> A(n);
  rep(i, n) cin >> A[i];
  vector<vector<int>> G(n);
  rep(i, n-1) {
    int p; cin >> p;
    G[i+1].push_back(p);
    G[p].push_back(i+1);
  }
  HLD hld(G, 0);
  BIT fw(n);
  rep(i, n) {
    fw.add(hld.in[i], A[i]);
  }
  rep(i, q) {
    int t; cin >> t;
    if (t == 0) {
      int p, x; cin >> p >> x;
      fw.add(hld.in[p], x);
    } else {
      int u; cin >> u;
      auto [l, r] = hld.subtree_prod(u);
      cout << fw.sum(l, r) << "\n";
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}