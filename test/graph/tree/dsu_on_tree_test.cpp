#include "./../../../akakoilib/template/template.cpp"
#include "./../../../akakoilib/graph/tree/dsu_on_tree.cpp"

void solve() {
    int n, r; cin >> n >> r;
    vector<int> A(n);
    rep(i, n) cin >> A[i];
    vector<vector<int>> G(n);
    rep(i, n-1) {
        int u, v; cin >> u >> v;
        --u; --v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    DSUonTree tree(G, r);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
