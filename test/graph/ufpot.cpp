// https://judge.yosupo.jp/submission/327570
#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/math/modint.cpp"
#include "./../../akakoi_lib/graph/potentialized_uf.cpp"

void solve() {
    int n, q; cin >> n >> q;
    PotUF<mint<998244353>> uf(n);
    rep(i, q) {
        int t; cin >> t;
        if (t == 0) {
            int u, v, x; cin >> u >> v >> x;
            cout << uf.unite(u, v, mint<998244353>(x)) << "\n";
        } else {
            int u, v; cin >> u >> v;
            auto [ok, diff] = uf.diff(u, v);
            if (ok) {
                cout << diff.x << "\n";
            } else {
                cout << -1 << "\n";
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
