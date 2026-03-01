#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/math/extgcd.cpp"

// -----------------------

void solve() {
    cout << ModInv(3, 7) << endl;
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
