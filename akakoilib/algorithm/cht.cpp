template <bool isMin = true>
struct CHT {
#define x first
#define y second
  using ld = long double;
  using pll = pair<ll, ll>;
  CHT() = default;
  deque<pll> v;
  bool empty() { return v.empty(); }
  void clear() { v.clear(); }
  inline int sgn(ll x) { return !x ? 0 : (x < 0 ? -1 : 1); }
  // 3本の直線a,b,cのうちbが不要か判定(誤差注意)
  inline bool check(const pll& a, const pll& b, const pll& c) {
    if (b.y == a.y || c.y == b.y) return sgn(b.x - a.x) * sgn(c.y - b.y) >= sgn(c.x - b.x) * sgn(b.y - a.y);
    return ld(b.x - a.x) * sgn(c.y - b.y) / ld(abs(b.y - a.y)) >= ld(c.x - b.x) * sgn(b.y - a.y) / ld(abs(c.y - b.y));
  }
  // 直線y=ax+bを追加 / 傾きaは単調に追加! / 償却 O(1)
  void add(ll a, ll b) {
    if (!isMin) a = -a, b = -b;
    pll line(a, b);
    if (empty()) v.push_front(line);
    else {
      if (ll c = v[0].x; c <= a) {
        if (c == a) {
          if (v[0].y <= b) return;
          v.pop_front();
        }
        while (v.size() > 1 && check(line, v[0], v[1])) v.pop_front();
        v.push_front(line);
      } else {
        assert(a <= v.back().x);
        if (v.back().x == a) {
          if (v.back().y <= b) return;
          v.pop_back();
        }
        while (v.size() > 1 && check(v[v.size() - 2], v.back(), line)) v.pop_back();
        v.push_back(line);
      }
    }
  }
  ll get_y(const pll& a, const ll& x) { return a.x*x + a.y; }
  ll query(ll x) { // 任意xに対する最適値を取得 / O(log N)
    assert(!empty());
    int l = -1, r = v.size() - 1;
    while (l + 1 < r) {
      int m = (l + r) / 2;
      if (get_y(v[m], x) >= get_y(v[m + 1], x)) l = m;
      else r = m;
    }
    return get_y(v[r], x) * (isMin ? 1 : -1);
  }
  // x が単調増加する場合のクエリ / 償却 O(1)
  ll query_monotone_inc(ll x) {
    assert(!empty());
    while (v.size() > 1 && get_y(v[0], x) >= get_y(v[1], x)) v.pop_front();
    return get_y(v[0], x) * (isMin ? 1 : -1);
  }
  // x が単調減少する場合のクエリ / 償却 O(1)
  ll query_monotone_dec(ll x) {
    assert(!empty());
    while (v.size() > 1 && get_y(v[v.size() - 1], x) >= get_y(v[v.size() - 2], x)) v.pop_back();
    return get_y(v.back(), x) * (isMin ? 1 : -1);
  }
#undef x
#undef y
};