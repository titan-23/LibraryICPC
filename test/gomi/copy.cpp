#include "./../../akakoi_lib/template/template.cpp"

namespace ptree {
    using U = int;
    using T = char;
    using F = int;
    T op(T s, T t) { return 'a'; }
    T mapping(F f, T s) { return s; }
    F composition(F f, F g) { return f; }
    T e() { return 'a'; }
    F id() { return 0; }
// U, T, op, e, F, mapping, composition, id
// (memo) #pragma pack(push, 1) #pragma pack(pop)
}

#include "./../../akakoi_lib/gomi/p-wbtree.cpp"

// -----------------------

void solve() {
    int m; cin >> m;
    string s; cin >> s;
    int sz = s.size();
    vector<char> S(sz);
    rep(i, sz) S[i] = s[i];
    ptree::init();
    ptree::PLWT wb(S);
    int n;
    cin >> n;
    rep(i, n) {
        if (n > 20 && i % (n/20) == 0) {
            ptree::PLWT::rebuild(wb);
        }
        int a, b, c;
        cin >> a >> b >> c;
        auto ab = wb.split(b).first.split(a).second;
        auto [y, z] = wb.split(c);
        wb = y.merge(ab).merge(z).split(m).first;
    }
    vector<char> ans = wb.tovector();
    for (const char &c : ans) {
        cout << c;
    }
    cout << endl;
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
