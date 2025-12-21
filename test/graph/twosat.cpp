// https://judge.yosupo.jp/submission/327578
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/graph/scc.cpp"
#include "./../../akakoilib/graph/twosat.cpp"

void solve() {
    string x; cin >> x >> x;
    int n, m; cin >> n >> m;
    TwoSat sat(n+1);
    rep(i, m) {
        int a, b; cin >> a >> b;
        int x; cin >> x;
        sat.add(abs(a), a>0, abs(b), b>0);
    }
    vector<bool> res = sat.solve();
    if (res.empty()) {
        cout << "s UNSATISFIABLE" << "\n";
        return;
    }
    cout << "s SATISFIABLE" << "\n";
    cout << "v ";
    for (int i = 1; i <= n; ++i) {
        cout << (res[i] > 0 ? i : -i) << " ";
    }
    cout << 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
