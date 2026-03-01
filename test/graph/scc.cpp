// https://judge.yosupo.jp/submission/327575
// https://judge.yosupo.jp/submission/356485
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/graph/scc.cpp"

void solve() {
    int n, m; cin >> n >> m;
    vector<vector<int>> G(n);
    rep(i, m) {
        int u, v; cin >> u >> v;
        G[u].push_back(v);
    }
    auto res = SccDecomposition(G);
    auto &members = res.members;
    cout << members.size() << "\n";
    for (auto &v : members) {
        cout << v.size() << " ";
        for (int x : v) cout << x << " ";
        cout << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
