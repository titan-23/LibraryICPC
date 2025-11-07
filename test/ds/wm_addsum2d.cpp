// https://judge.yosupo.jp/submission/327267
#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/ds/wavelet_matrix.cpp"
#include "./../../akakoi_lib/ds/segtree.cpp"
#include "./../../akakoi_lib/ds/wavelet_matrix_seg.cpp"

ll op(ll s, ll t) { return s + t; }
ll e() { return 0; }

void solve() {
    int n, q; cin >> n >> q;
    WaveletMatrixSeg<int, ll, op, e> wm;
    map<pair<int, int>, ll> mp;
    rep(i, n) {
        int x, y, w; cin >> x >> y >> w;
        mp[{x, y}] += w;
    }
    for (auto [pos, w] : mp) {
        wm.set_point(pos.first, pos.second, w);
    }
    vector<tuple<int, int, int, int, int>> Q(q);
    rep(i, q) {
        int op; cin >> op;
        if (op == 0) {
            int x, y, w; cin >> x >> y >> w;
            if (mp.count({x, y}) == 0) {
                wm.set_point(x, y, 0);
            }
            Q[i] = {0, x, y, w, -1};
        } else {
            int l, d, r, u; cin >> l >> d >> r >> u;
            Q[i] = {1, l, d, r, u};
        }
    }
    wm.build();
    for (auto [op, l, d, r, u] : Q) {
        if (op == 0) {
            int x = l, y = d, w = r;
            mp[{x, y}] += w;
            wm.update_point(x, y, mp[{x, y}]);
        } else {
            cout << wm.range_prod(l, r, d, u) << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
