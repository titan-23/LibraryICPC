// https://judge.yosupo.jp/submission/327261
#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/ds/wavelet_matrix.cpp"

void solve() {
    int n, q; cin >> n >> q;
    vector<int> A(n);
    rep(i, n) cin >> A[i];
    WaveletMatrix<int> wm(A);
    rep(i, q) {
        int l, r, k; cin >> l >> r >> k;
        cout << wm.kth_smallest(l, r, k) << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
