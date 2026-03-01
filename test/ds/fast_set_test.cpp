// https://judge.yosupo.jp/submission/356493
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/ds/fast_set.cpp"

void solve() {
    int n, q; cin >> n >> q;
    string t; cin >> t;
    FastSet S(n);
    rep(i, n) if (t[i] == '1') {
        S.insert(i);
    }
    rep(_, q) {
        int t, k; cin >> t >> k;
        if (t == 0) {
            S.insert(k);
        } else if (t == 1) {
            S.erase(k);
        } else if (t == 2) {
            cout << S.contains(k) << "\n";
        } else if (t == 3) {
            cout << S.ge(k) << "\n";
        } else if (t == 4) {
            cout << S.le(k) << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
