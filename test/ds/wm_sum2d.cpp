// https://judge.yosupo.jp/submission/337622
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/ds/wavelet_matrix.cpp"
#include "./../../akakoilib/ds/segtree.cpp"
#include "./../../akakoilib/ds/wavelet_matrix_seg.cpp"

ll op(ll s, ll t) { return s + t; }
ll e() { return 0; }

void solve() {
    int n, q; cin >> n >> q;
    WaveletMatrixSeg<int, ll, op, e> wm;
    rep(i, n) {
        int x, y, w; cin >> x >> y >> w;
        wm.set_point(x, y, w);
    }
    wm.build();
    rep(i, q) {
        int l, d, r, u; cin >> l >> d >> r >> u;
        cout << wm.range_prod(l, r, d, u) << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
