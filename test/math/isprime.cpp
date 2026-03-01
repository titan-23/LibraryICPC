// https://judge.yosupo.jp/submission/356465
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
        if (is_primell(n)) {
            cout << "Yes" << "\n";
        } else {
            cout << "No" << "\n";
        }
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
