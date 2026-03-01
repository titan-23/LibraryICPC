// https://judge.yosupo.jp/submission/337596
// https://judge.yosupo.jp/submission/356497
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/ds/segtree.cpp"

#include "./../../akakoilib/math/modint.cpp"

const int mod = 998244353;
struct T {
    mint<mod> a, b;
};

T op(T s, T t) {
    return {s.a*t.a, s.b*t.a+t.b};
}
T e() {
    return {1, 0};
}

void solve() {
    int n, q; cin >> n >> q;
    vector<T> A(n);
    rep(i, n) {
        int a, b; cin >> a >> b;
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
            cout << (a*x+b).x << "\n";
        }
    }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}