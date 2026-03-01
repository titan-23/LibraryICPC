// https://judge.yosupo.jp/submission/356425
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/string/z.cpp"

// -----------------------

void solve() {
    string s; cin >> s;
    vector<int> S(s.size());
    rep(i, s.size()) S[i] = s[i];
    vector<int> ans = ZAlgorithm(S);
    rep(i, ans.size()) {
        cout << ans[i] << " ";
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
