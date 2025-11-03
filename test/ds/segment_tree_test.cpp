#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/ds/segment_tree.cpp"

const int mod = 998244353;
using T = pair<ll, ll>;

T op(T s, T t) {
    // s[0]x + s[1]
    // t[0](s[0]x + s[1]) + t[1]
    // s[0]*t[0]x + s[1]*t[0]+t[1]
    return {(s.first*t.first)%mod, (s.second*t.first+t.second)%mod};
}
T e() {
    return {1, 0};
}

void solve() {
    int n, q; cin >> n >> q;
    vector<T> A(n);
    rep(i, n) {
        ll a, b; cin >> a >> b;
        A[i] = {a, b};
    }
    Segtree<T, op, e> seg(A);
    rep(i, q) {
        int op; cin >> op;
        if (op == 0) {
            int p, c, d; cin >> p >> c >> d;
            seg.set(p, {c, d});
        } else {
            int l, r, x; cin >> l >> r >> x;
            auto [a, b] = seg.prod(l, r);
            cout << (a*x+b)%mod << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
