// https://judge.yosupo.jp/submission/327573
// https://judge.yosupo.jp/submission/356491
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/math/modint.cpp"
#include "./../../akakoilib/graph/potentialized_uf.cpp"

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
