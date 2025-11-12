#include "./../../../akakoi_lib/template/template.cpp"
#include "./../../../akakoi_lib/graph/flow/max_flow.cpp"

void solve() {
    int n, m; cin >> n >> m;
    Dinic<ll> G(n);
    rep(i, m) {
        int a, b, c; cin >> a >> b >> c;
        --a; --b;
        G.add_edge(a, b, c, i);
    }
    cout << G.max_flow(0, n-1) << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}
