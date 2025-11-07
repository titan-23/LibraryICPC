// https://judge.yosupo.jp/submission/327259

#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/ds/lazy_segtree.cpp"

static constexpr ll mod = 998244353;
static constexpr ll bit = 30;
static constexpr ll msk = (1<<bit)-1;

ll op(const ll s, const ll t) {
    ll s1 = s>>bit, s2 = s&msk;
    ll t1 = t>>bit, t2 = t&msk;
    ll c1 = (s1 + t1) % mod;
    ll c2 = (s2 + t2) % mod;
    return (c1 << 30) + c2;
}

ll mapping(const ll f, const ll s) {
    ll f1 = f>>bit, f2 = f&msk;
    ll s1 = s>>bit, s2 = s&msk;
    return (((s1 * f1 + s2 * f2) % mod) << 30) + s2;
}

ll composition(const ll f, const ll g) {
    ll f1 = f>>bit, f2 = f&msk;
    ll g1 = g>>bit, g2 = g&msk;
    ll z1 = (f1 * g1) % mod;
    ll z2 = (f1 * g2 + f2) % mod;
    return (z1 << 30) + z2;
}

ll e() {
    return 0;
}

ll id() {
    return 1<<bit;
}

void solve() {
    int n, q;
    cin >> n >> q;
    LazySegtree<ll, op, e, ll, mapping, composition, id> seg(n);
    rep(i, n) {
        ll a;
        cin >> a;
        seg.set(i, a<<bit|1);
    }
    rep(_, q) {
        int com;
        cin >> com;
        if (com == 0) {
            int l, r;
            ll b, c;
            cin >> l >> r >> b >> c;
            seg.apply(l, r, b<<bit|c);
        } else {
            int l, r;
            cin >> l >> r;
            ll ans = seg.prod(l, r) >> bit;
            cout << ans << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
