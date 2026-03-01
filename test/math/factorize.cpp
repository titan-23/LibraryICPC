// https://judge.yosupo.jp/submission/356466
// https://judge.yosupo.jp/submission/356467
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/other/random.cpp"
Random trnd;

template<typename T>
T pow_mod(T a, T b, T mod) {
    T res = 1;
    while (b) {
        if (b & 1) {
            res *= a;
            res %= mod;
        }
        a *= a;
        a %= mod;
        b >>= 1;
    }
    return res;
}

#include "./../../akakoilib/math/factorize.cpp"

// -----------------------

void solve() {
    int q; cin >> q;
    rep(i, q) {
        ll n; cin >> n;
        vector<ll> f = factorize(n);
        cout << f.size() << " ";
        for (ll x : f) cout << x << " ";
        cout << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(15);
    cerr << fixed << setprecision(15);

    int t = 1;
    // cin >> t;
    for (int i = 0; i < t; ++i) {
        solve();
    }
    return 0;
}
