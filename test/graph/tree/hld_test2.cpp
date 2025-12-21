// https://judge.yosupo.jp/submission/327581
#include "./../../../akakoilib/template/template.cpp"

#include "./../../../akakoilib/ds/fenwick_tree.cpp"
#include "./../../../akakoilib/graph/tree/hld.cpp"

void solve() {
  int n, q; cin >> n >> q;
  vector<ll> A(n);
  rep(i, n) cin >> A[i];
  vector<vector<int>> G(n);
  rep(i, n-1) {
    int u, v; cin >> u >> v;
    G[u].push_back(v);
    G[v].push_back(u);
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
      int u, v; cin >> u >> v;
      ll res = 0;
      for (auto [_, l, r] : hld.path_prod(u, v)) {
        res += fw.sum(l, r);
      }
      cout << res << "\n";
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}