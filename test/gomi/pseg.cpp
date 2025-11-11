#include "./../../akakoi_lib/template/template.cpp"

using ull = unsigned long long;
using U = int;
using T = ull;
using F = ull;

static constexpr ull mod = 998244353;
static constexpr ull bit = 31;
static constexpr ull msk = ((ull)1<<bit)-1;

T op(const T s, const T t) {
    T s1 = s>>bit, s2 = s&msk;
    T t1 = t>>bit, t2 = t&msk;
    T c1 = (s1 + t1) % mod;
    T c2 = (s2 + t2) % mod;
    return (c1 << bit) + c2;
}

T mapping(const F f, const T s) {
    T f1 = f>>bit, f2 = f&msk;
    T s1 = s>>bit, s2 = s&msk;
    return (((s1 * f1 + s2 * f2) % mod) << bit) + s2;
}

F composition(const F f, const F g) {
    F f1 = f>>bit, f2 = f&msk;
    F g1 = g>>bit, g2 = g&msk;
    F z1 = (f1 * g1) % mod;
    F z2 = (f1 * g2 + f2) % mod;
    return (z1 << bit) + z2;
}

T e() {
    return 0;
}

F id() {
    return (ull)1<<bit;
}

#include "./../../akakoi_lib/gomi/p-wbtree.cpp"

void solve() {
    int n, q;
    cin >> n >> q;
    using PSegType = ptree::PLWT;
    ptree::init();
    // PSegType::ma.reserve(2e7);

    vector<PSegType> seg(q+1);

    vector<T> A(n);
    rep(i, n) {
        ull a;
        cin >> a;
        A[i] = a<<bit|1;
    }
    seg[q] = PSegType(A);

    rep(i, q) {
        int t, k;
        cin >> t >> k;
        if (k == -1) k = q;

        if (t == 0) {
            int l, r;
            ull b, c;
            cin >> l >> r >> b >> c;
            seg[i] = seg[k].apply(l, r, b<<bit|c);
        } else if (t == 1) {
            int s, l, r;
            cin >> s >> l >> r;
            if (s == -1) s = q;
            auto mid = seg[s].split(r).first.split(l).second;
            auto left = seg[k].split(l).first;
            auto right = seg[k].split(r).second;
            seg[i] = left.merge(mid).merge(right);
        } else {
            int l, r;
            cin >> l >> r;
            ull ans = seg[k].prod(l, r) >> bit;
            cout << ans << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(15);
    solve();
    return 0;
}
