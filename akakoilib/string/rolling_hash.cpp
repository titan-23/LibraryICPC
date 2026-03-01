const ll mod = (1LL << 61) - 1;
ll add(ll a, ll b) { return (a += b) >= mod ? a-mod : a; }
ll mul(ll a, ll b) {
  __int128_t c = (__int128_t)a * b;
  return add(c>>61, c&mod);
}
ll r = 7954398468495;
struct RollingHash {
  ll n;
  vector<ll> hs, pw;
  RollingHash(string s) : n(s.size()), hs(n+1), pw(n+1, 1) {
    rep(i, n) {
      pw[i+1] = mul(pw[i], r);
      hs[i+1] = add(mul(hs[i], r), s[i]-'a'+1);
    }
  }
  ll get(ll l, ll r) { return add(hs[r], mod - mul(hs[l], pw[r-l])); }
};