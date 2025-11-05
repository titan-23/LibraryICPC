#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/algorithm/mo.cpp"

#include "./../../akakoi_lib/ds/fenwick_tree.cpp"

void solve() {
  int n, q; cin >> n >> q;
  vector<int> A(n);
  rep(i, n) cin >> A[i];
  Mo mo(n);
  rep(qdx, q) {
    int l, r; cin >> l >> r;
    mo.add_query(l, r);
  }

  vector<int> S = A;
  sort(A.begin(), A.end());
  A.erase(unique(A.begin(), A.end()), A.end());
  auto getA = [&] (int a) -> int {
    return lower_bound(A.begin(), A.end(), a) - A.begin();
  };
  vector<int> B(n);
  rep(i, n) {
    B[i] = getA(S[i]);
  }

  vector<ll> ans(q);
  BIT fw(n);
  ll res = 0;

  auto add_left = [&] (int i) -> void {
    res += fw.sum(0, B[i]);
    fw.add(B[i], 1);
  };
  auto add_right = [&] (int i) -> void {
    res += fw.sum(B[i]+1, n);
    fw.add(B[i], 1);
  };
  auto del_left = [&] (int i) -> void {
    res -= fw.sum(0, B[i]);
    fw.add(B[i], -1);
  };
  auto del_right = [&] (int i) -> void {
    res -= fw.sum(B[i]+1, n);
    fw.add(B[i], -1);
  };
  auto out = [&] (int i) -> void { ans[i] = res; };
  mo.run(add_left, add_right, del_left, del_right, out);
  rep(i, q) {
    cout << ans[i] << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}