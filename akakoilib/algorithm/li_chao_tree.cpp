struct LiChaoTree {
  struct Line {
    ll a, b;
    ll get(ll x) { return a*x+b; }
  };
  int n;
  vector<Line> dat;
  LiChaoTree(int n_) {
    n = 1;
    while (n < n_) n *= 2;
    dat.assign(2*n, {0, INF});
  }
  void _add_line(Line line, int k, int l, int r) {
    int m = (l + r) / 2;
    bool left  = line.get(l) < dat[k].get(l);
    bool mid   = line.get(m) < dat[k].get(m);
    bool right = line.get(r) < dat[k].get(r);
    if (mid) { swap(dat[k], line); swap(left, right); }
    if (r - l <= 1) return;
    if (left != mid) _add_line(line, 2*k, l, m);
    else if (right != mid) _add_line(line, 2*k+1, m, r);
  }
  void _add_segment(int a, int b, Line line, int k, int l, int r) {
    if (r <= a || b <= l) return;
    if (a <= l && r <= b) {
      _add_line(line, k, l, r);
      return;
    }
    _add_segment(a, b, line, 2*k, l, (l+r)/2);
    _add_segment(a, b, line, 2*k+1, (l+r)/2, r);
  }
  void add_segment(int l, int r, ll a, ll b) { // O(log^2N)
    _add_segment(l, r, {a, b}, 1, 0, n);
  }
  void add_line(ll a, ll b) { // O(logN)
    _add_line({a, b}, 1, 0, n);
  }
  ll query(int x) { // O(logN)
    ll res = INF; int k = x + n;
    while (k > 0) { chmin(res, dat[k].get(x)); k >>= 1; }
    return res;
  }
};