// https://judge.yosupo.jp/submission/356476
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/math/stern_brocot_tree.cpp"

// -----------------------

void solve() {
    SternBrocotTree<ll> SBT;
    int t; cin >> t;
    rep(tdx, t) {
        string s; cin >> s;
        if (s == "DECODE_PATH") {
            int k; cin >> k;
            vector<pair<char, ll>> P(k);
            rep(i, k) {
                char c; int n; cin >> c >> n;
                P[i] = {c, n};
            }
            auto ans = SBT.decode_path(P);
            cout << ans.num() << " " << ans.den() << "\n";
        } else if (s == "ENCODE_PATH") {
            ll a, b; cin >> a >> b;
            auto node = SBT.get_node(a, b);
            auto P = SBT.encode_path(node);
            cout << P.size() << " ";
            for (auto [c, n] : P) {
                cout << c << " " << n << " ";
            }
            cout << "\n";
        } else if (s == "LCA") {
            ll a, b, c, d; cin >> a >> b >> c >> d;
            auto p = SBT.get_node(a, b);
            auto q = SBT.get_node(c, d);
            auto lca = SBT.lca(p, q);
            cout << lca.num() << " " << lca.den() << "\n";
        } else if (s == "ANCESTOR") {
            int k, a, b; cin >> k >> a >> b;
            auto node = SBT.get_node(a, b);
            auto [ok, ans] = SBT.ancestor(node, k);
            if (ok) {
                cout << ans.num() << " " << ans.den() << "\n";
            } else {
                cout << -1 << "\n";
            }
        } else if (s == "RANGE") {
            ll a, b; cin >> a >> b;
            auto node = SBT.get_node(a, b);
            auto [p, q] = node.range();
            cout << p.first << " " << p.second << " " << q.first << " " << q.second << "\n";
        }
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
