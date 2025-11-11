struct FastSet {
  using u64 = unsigned long long;
  const int W = 63;
  int n, log;
  vector<vector<u64>> a;
  inline constexpr int bit_length(u64 x) {
    return x ? 64-__builtin_clzll(x) : 0;
  }
  FastSet(int n) : n(n) {
    a.clear();
    int m = n;
    do {
      a.push_back(vector<u64>((m+W)>>6));
      m = (m+W)>>6;
    } while (m > 1);
    log = (int)a.size();
  }
  bool contains(int i) { return (a[0][i>>6]>>(i&W)) & 1; }
  void insert(int i) {
    rep(h, log) {
      a[h][i>>6] |= 1ull << (i&W);
      i >>= 6;
    }
  }
  void erase(int i) {
    rep(h, log) {
      a[h][i>>6] &= ~(1ull << (i&W));
      if (a[h][i>>6]) break;
      i >>= 6;
    }
  }
  int ge(int v) { // i以上で最小の要素
    int d = 0;
    while (1) {
      if (d >= (int)a.size() || ((v>>6) >= a[d].size())) { return -1; }
      u64 m = a[d][v>>6] & ((~(u64)0) << (v & W));
      if (m == 0) {
        d++; v = (v >> 6) + 1;
      } else {
        v = (v >> 6 << 6) + bit_length(m & -m) - 1;
        if (d == 0) break;
        v <<= 6; d--;
      }
    }
    return v;
  }
  int gt(int v) { return ge(v+1); }
  int le(int v) { // i以下で最大の要素
    int d = 0;
    while (1) {
      if (v < 0 || d >= (int)a.size()) return -1;
      u64 m = a[d][v >> 6] & ~((~1ull) << (v & W));
      if (m == 0) {
        d++; v = (v >> 6) - 1;
      } else {
        v = (v >> 6 << 6) + bit_length(m) - 1;
        if (d == 0) break;
        v <<= 6; v += W; --d;
      }
    }
    return v;
  }
  int lt(int v) { return le(v-1); }
};