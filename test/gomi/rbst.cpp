#include "./../../akakoi_lib/template/template.cpp"
#include "./../../akakoi_lib/math/modint.cpp"
#include "./../../akakoi_lib/other/random.cpp"
#include "./../../akakoi_lib/gomi/rbst.cpp"

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

void dynamic_sequence_range_affine_range_sum() {
    int n, q;
    cin >> n >> q;
    vector<ll> A(n);
    rep(i, n) {
        ll a;
        cin >> a;
        A[i] = a<<bit|1ll;
    }

    using Tree = LazyRBST<ll, op, e, ll, mapping, composition, id>;
    Tree s(A);

    rep(qdx, q) {
        int com; cin >> com;
        if (com == 0) {
            int i;
            ll x;
            cin >> i >> x;
            s.insert(i, x<<bit|1ll);
        } else if (com == 1) {
            int i;
            cin >> i;
            s.pop(i);
        } else if (com == 2) {
            int l, r;
            cin >> l >> r;
            s.reverse(l, r);
        } else if (com == 3) {
            int l, r;
            ll b, c;
            cin >> l >> r >> b >> c;
            s.apply(l, r, b<<bit|c);
        } else {
            int l, r;
            cin >> l >> r;
            ll res = s.prod(l, r)>>bit;
            cout << res << '\n';
        }
    }
    // cerr << s.height() << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  dynamic_sequence_range_affine_range_sum();
}