// https://judge.yosupo.jp/submission/327570
#include "./../../akakoilib/template/template.cpp"
using ull = unsigned long long;
#include "./../../akakoilib/graph/max_independent_set.cpp"

void solve() {
    int n, m; cin >> n >> m;
    vector<vector<int>> G(n);
    rep(i, m) {
        int u, v; cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    auto X = maximum_independent_set(G);
    cout << __builtin_popcountll(X) << "\n";
    rep(i, n) {
        if (X >> i & 1) cout << i << " ";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
