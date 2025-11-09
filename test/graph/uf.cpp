// https://judge.yosupo.jp/submission/327579
#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/graph/union_find.cpp"

void solve() {
    int n, q; cin >> n >> q;
    UnionFind uf(n);
    rep(i, q) {
        int t, u, v; cin >> t >> u >> v;
        if (t == 0) {
            uf.unite(u, v);
        } else {
            cout << uf.same(u, v) << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
