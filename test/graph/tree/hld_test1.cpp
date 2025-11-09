#include "./../../../akakoi_lib/template/template.cpp"

#include "./../../../akakoi_lib/ds/segtree.cpp"
#include "./../../../akakoi_lib/graph/tree/hld.cpp"


const int mod = 998244353;
const int msk = (1ll<<31)-1;

ll op(ll s, ll t) {
  ll a1 = s>>31, b1 = s&msk;
  ll a2 = t>>31, b2 = t&msk;
  return ((a1*a2%mod)<<31) + ((a2*b1+b2)%mod);
}
ll op_rev(ll s, ll t) {
  return op(t, s);
}

ll e() { return 1ll<<31; }

void solve() {
  int n, q; cin >> n >> q;
  vector<vector<int>> G(n);
  vector<ll> A(n);
  rep(i, n) {
    ll a, b; cin >> a >> b;
    A[i] = a<<31|b;
  }
  rep(i, n-1) {
    int a, b; cin >> a >> b;
    G[a].emplace_back(b);
    G[b].emplace_back(a);
  }

  HLD hld(G, 0);
  vector<ll> init(n);
  rep(i, n) {
    init[hld.in[i]] = A[i];
  }
  Segtree<ll, op, e> seg(init);
  Segtree<ll, op_rev, e> seg_rev(init);

  rep(qdx, q) {
    int c; cin >> c;
    if (c == 0) {
      int p, c, d; cin >> p >> c >> d;
      seg.set(hld.in[p], (ll)c<<31|d);
      seg_rev.set(hld.in[p], (ll)c<<31|d);
    } else {
      int u, v, x; cin >> u >> v >> x;
      auto path = hld.path_prod(u, v);
      ll lv = e(), rv = e();
      for (auto [f, l, r] : path) {
        if (f) {
          lv = op(lv, seg_rev.prod(l, r));
        } else {
          rv = op_rev(rv, seg.prod(l, r));
        }
      }
      ll res = op(lv, rv);
      ll a = res>>31, b = res&msk;
      cout << (a*x+b)%mod << "\n";
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}