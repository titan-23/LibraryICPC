// https://judge.yosupo.jp/submission/337599
// https://judge.yosupo.jp/submission/356495

#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/math/modint.cpp"
#include "./../../akakoilib/ds/lazy_segtree.cpp"

const int mod = 998244353;

struct T {
    mint<mod> a, b;
};

struct F {
    mint<mod> a, b;
};

T op(T s, T t) {
    return {s.a+t.a, s.b+t.b};
}

T mapping(F f, T s) {
    return {s.a*f.a+s.b*f.b, s.b};
}

F composition(F f, F g) {
    return {f.a*g.a, f.a*g.b+f.b};
}

T e() {
    return {0, 0};
}

F id() {
    return {1, 0};
}

void solve() {
    int n, q;
    cin >> n >> q;
    LazySegtree<T, op, e, F, mapping, composition, id> seg(n);
    rep(i, n) {
        ll a;
        cin >> a;
        seg.set(i, {a, 1});
    }
    rep(_, q) {
        int com;
        cin >> com;
        if (com == 0) {
            int l, r;
            ll b, c;
            cin >> l >> r >> b >> c;
            seg.apply(l, r, {b, c});
        } else {
            int l, r;
            cin >> l >> r;
            mint<mod> ans = seg.prod(l, r).a;
            cout << ans.x << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
