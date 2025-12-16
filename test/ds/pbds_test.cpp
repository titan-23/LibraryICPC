// https://judge.yosupo.jp/submission/337595
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/ds/pbds.cpp"

void solve() {
    int n, q; cin >> n >> q;
    vector<int> A(n);
    rep(i, n) cin >> A[i];

    ordered_set<int> S(A.begin(), A.end());
    rep(_, q) {
        int t, x; cin >> t >> x;
        if (t == 0) {
            S.insert(x);
        } else if (t == 1) {
            S.erase(x);
        } else if (t == 2) {
            if (S.size() < x) cout << -1 << "\n";
            else {
                auto it = S.find_by_order(x-1);
                cout << (*it) << "\n";
            }
        } else if (t == 3) {
            int cnt = S.order_of_key(x+1);
            cout << cnt << "\n";
        } else if (t == 4) {
            auto it = S.upper_bound(x);
            if (it == S.begin()) cout << -1 << "\n";
            else {
                --it;
                cout << (*it) << "\n";
            }
        } else if (t == 5) {
            auto it = S.lower_bound(x);
            if (it == S.end()) cout << -1 << "\n";
            else cout << (*it) << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
