// https://judge.yosupo.jp/submission/356697
#include "./../../akakoilib/template/template.cpp"
// #include "./../../akakoilib/math/modint.cpp"
// #include "./../../akakoilib/other/random.cpp"
#include "./../../akakoilib/gomi/lct.cpp"

static constexpr int mod = 998244353;
static constexpr int bit = 31;
static constexpr int msk = (1ll<<bit)-1;

ll op(ll s, ll t) {
    ll a1 = s>>bit, b1 = s&msk;
    ll a2 = t>>bit, b2 = t&msk;
    return ((a1*a2%mod)<<bit) | ((a2*b1+b2) % mod);
}
ll mapping(ll f, ll s) { return 0; }
ll composition(ll f, ll g) { return 0; }
ll e() { return 1ll << bit; }
ll id() { return 0; }


void solve() {
    int n, q;
    cin >> n >> q;
    LazyLinkCutTree<ll, op, e, ll, mapping, composition, id> lct(n);
    rep(i, n) {
        ll a, b; cin >> a >> b;
        lct.set(i, a<<bit|b);
    }
    rep(_, n-1) {
        int u, v; cin >> u >> v;
        lct.merge(u, v);
    }
    rep(qdx, q) {
        int t; cin >> t;
        if (t == 0) {
            int u, v, w, x;
            cin >> u >> v >> w >> x;
            lct.split(u, v);
            lct.merge(w, x);
        } else if (t == 1) {
            int p;
            ll c, d;
            cin >> p >> c >> d;
            lct.set(p, (c<<bit)|d);
        } else {
            int u, v;
            ll x;
            cin >> u >> v >> x;
            ll res = lct.path_prod(u, v);
            cout << ((res>>bit) * x + (res&msk)) % mod << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
