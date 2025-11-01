// https://judge.yosupo.jp/submission/325053

#include "./../../akakoi_lib/template/template.cpp"

#include "./../../akakoi_lib/graph/scc.cpp"


void solve() {
    int n, m; cin >> n >> m;
    vector<vector<int>> G(n);
    rep(i, m) {
        int a, b; cin >> a >> b;
        G[a].push_back(b);
    }
    auto scc = get_scc_graph(G);
    cout << scc.groups.size() << "\n";
    for (auto &g : scc.groups) {
        cout << g.size() << " ";
        for (auto x : g) {
            cout << x << " ";
        }
        cout << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
