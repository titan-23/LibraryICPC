#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/math/floor_sum.cpp"

// -----------------------

void solve() {
    int t; cin >> t;
    rep(_, t) {
        int n, m, a, b; cin >> n >> m >> a >> b;
        cout << floor_sum(n, m, a, b) << "\n";
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
