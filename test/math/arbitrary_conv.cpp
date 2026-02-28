#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/math/modint.cpp"
#include "./../../akakoilib/math/arbitrary_mod_convolution.cpp"

// -----------------------

void solve() {
    int n, m; cin >> n >> m;
    vector<int> A(n), B(m);
    rep(i, n) cin >> A[i];
    rep(i, m) cin >> B[i];
    const int mod = 938234350;
    vector<mint<mod>> X(n), Y(m);
    rep(i, n) X[i] = A[i];
    rep(i, m) Y[i] = B[i];
    auto ans = arbitrary_mod_convolution<mint<mod>>(X, Y);
    rep(i, n+m-1) {
        cout << ans[i].x << " ";
    }
    cout << "\n";
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    // int t; cin >> t;
    int t = 1;
    rep(tdx, t) {
        solve();
    }

    return 0;
}
