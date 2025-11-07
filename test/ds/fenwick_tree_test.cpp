// https://judge.yosupo.jp/submission/327254

#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/ds/fenwick_tree.cpp"

void solve() {
    int n, q; cin >> n >> q;
    vector<ll> A(n);
    rep(i, n) cin >> A[i];
    BIT fw(A);
    rep(i, q) {
        int c; cin >> c;
        if (c == 0) {
            int p, x; cin >> p >> x;
            fw.add(p, x);
        } else {
            int l, r; cin >> l >> r;
            cout << fw.sum(l, r) << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
