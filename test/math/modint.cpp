#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/math/modint.cpp"

// -----------------------

const int mod = 998244353;
// const int mod = 1000000007;

void solve() {
    mint<mod> a = mint<mod>(123456789)*mint<mod>(888777666)/mint<mod>(777666555)-mint<mod>(666555444).pow(555444333)+mint<mod>(987654321);
    cout << a.x << endl;
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
