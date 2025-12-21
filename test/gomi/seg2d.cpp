#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/gomi/seg2D.cpp"

ll op(ll s, ll t) { return s + t; }
ll e() { return 0; }

void solve() {
    int n, q; cin >> n >> q;
    Seg2D<int, ll, op, e> seg(1e9, 1e9);
    seg.reserve(1e7);
    rep(i, n) {
        int x, y, w; cin >> x >> y >> w;
        seg.set(x, y, w);
    }
    rep(_, q) {
        int t; cin >> t;
        if (t == 0) {
            int x, y, w; cin >> x >> y >> w;
            seg.set(x, y, seg.get(x, y) + w);
        } else {
            int l, d, r, u; cin >> l >> d >> r >> u;
            cout << seg.prod(l, r, d, u) << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
