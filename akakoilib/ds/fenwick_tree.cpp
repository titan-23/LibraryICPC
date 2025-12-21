struct BIT {
  int n;
  vector<ll> d;
  BIT(int n) : n(n), d(n+1) {}
  BIT(vector<ll> a) : n(a.size()), d(n+1) {
    rep(i, n) d[i+1] = a[i];
    for (int i = 1; i < n; ++i) {
      if (i+(i&-i) <= n) d[i+(i&-i)] += d[i];
    }
  }
  void add(int i, ll x) {
    i++;
    while (i <= n) d[i] += x, i += i & -i;
  }
  ll sum(int r) {
    ll s = 0;
    while (r) s += d[r], r -= r & -r;
    return s;
  }
  ll sum(int l, int r) { return sum(r) - sum(l); }
  int bisect_left(ll w) { // sum(0, i+1)>=wとなる最小のi
    int i = 0, s = 1<<(32-__builtin_clz(n));
    while (s) {
      if (i+s <= n && d[i+s] < w) { w -= d[i+s]; i += s; }
      s >>= 1;
    }
    return (w ? i : -1);
  }
  int bisect_right(ll w) { // sum(0, i+1)>wとなる最小のi
    int i = 0, s = 1<<(32-__builtin_clz(n));
    while (s) {
      if (i+s <= n && d[i+s] <= w) { w -= d[i+s]; i += s; }
      s >>= 1;
    }
    return i;
  }
};
struct BITRAQRSQ {
  BIT fw0, fw1;
  BITRAQRSQ(int n) : fw0(n), fw1(n) {}
  void add_range(int l, int r, ll w) {
    fw0.add(l, -w * l); fw0.add(r, w * r);
    fw1.add(l, w); fw1.add(r, -w);
  }
  ll pref(int r) { return fw0.sum(r) + (ll)r*fw1.sum(r); }
  ll sum(int l, int r) { return pref(r) - pref(l); }
};