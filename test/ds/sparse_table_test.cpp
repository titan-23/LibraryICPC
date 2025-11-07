// https://judge.yosupo.jp/submission/327260

#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/ds/sparse_table.cpp"

int op(int s, int t) {
    return min(s, t);
}
int e() {
    return INT_MAX;
}

void solve() {
    int n, q; cin >> n >> q;
    vector<int> A(n);
    rep(i, n) cin >> A[i];
    SparseTable<int, op, e> seg(A);
    rep(i, q) {
        int l, r; cin >> l >> r;
        cout << seg.prod(l, r) << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
