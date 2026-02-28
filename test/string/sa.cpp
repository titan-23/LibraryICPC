// https://judge.yosupo.jp/submission/356430
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/string/sa.cpp"

// -----------------------

void solve() {
    string s; cin >> s;
    auto [sa, lcp] = SA(s);
    rep(i, s.size()) {
        cout << sa[i] << " ";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    solve();

    return 0;
}
