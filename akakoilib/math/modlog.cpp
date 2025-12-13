ll ModLog(ll x, ll y, ll mod) { // return k s.t. x^k=y (mod m) / O(sqrt(m))
  ll m = ceil(sqrt(mod)) + 1;
  ll now_y = y;
  map<ll, ll> mp;
  rep(i, m) {
    mp[now_y] = i;
    (now_y *= x) %= mod;
  }
  ll x_pow = 1;
  rep(i, m) (x_pow *= x) %= mod;
  ll now_x = x_pow;
  for (int i = 1; i <= m; i++) {
    if (mp.find(now_x) != mp.end()) return i * m - mp[now_x];
    now_x = now_x * x_pow % mod;
  }
  return -1;
}