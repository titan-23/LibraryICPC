struct Mo {
  using u64 = unsigned long long;
  u64 max_n;
  vector<pair<int, int>> lr;
  u64 hilbertorder(u64 x, u64 y) {
    u64 res = 0;
    for (u64 s = max_n >> 1; s; s >>= 1) {
      bool rx = (x & s), ry = (y & s);
      res = (res << 2) | (rx ? (ry ? 2 : 1) : (ry ? 3 : 0));
      if (!rx) {
        if (ry) { x = max_n-1-x; y = max_n-1-y; }
        swap(x, y);
      }
    }
    return res;
  }
  Mo(int n) {
    max_n = 1 << 20;
    while (max_n <= n) max_n <<= 1;
  }
  void add_query(int l, int r) { lr.emplace_back(l, r); }
  template<typename add_l, typename add_r, typename erase_l, typename erase_r, typename output>
  void run(const add_l& al, const add_r& ar, const erase_l& el, const erase_r& er, const output& out) {
    int q = lr.size();
    vector<int> qi(q); iota(qi.begin(), qi.end(), 0);
    vector<u64> eval(q);
    rep(i, q) eval[i] = hilbertorder(lr[i].first, lr[i].second);
    sort(qi.begin(), qi.end(), [&] (int i, int j) { return eval[i] < eval[j]; });
    int l = 0, r = 0;
    for (int i : qi) {
      const auto [li, ri] = lr[i];
      while (l > li) al(--l); // 左端を拡張
      while (r < ri) ar(r++); // 右端を拡張
      while (l < li) el(l++); // 左端を縮小
      while (r > ri) er(--r); // 右端を縮小
      out(i);
    }
  }
};