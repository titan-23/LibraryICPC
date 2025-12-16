// https://atcoder.jp/contests/practice2/submissions/71755266
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/ds/segtree.cpp"

int op(int s, int t) {
    return max(s, t);
}
int e() {
    return -1;
}

void solve() {
    int n, q; cin >> n >> q;
    vector<int> A(n);
    rep(i, n) cin >> A[i];
    Segtree<int, op, e> seg(A);
    rep(i, q) {
        int t; cin >> t;
        if (t == 1) {
            int x, v; cin >> x >> v;
            --x;
            seg.set(x, v);
        } else if (t == 2) {
            int l, r; cin >> l >> r;
            --l;
            cout << seg.prod(l, r) << "\n";
        } else {
            int x, v; cin >> x >> v;
            --x;
            cout << seg.max_right(x, [&v] (int lr) { return lr < v; }) + 1 << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
