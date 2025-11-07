using u64 = unsigned long long;
struct BitVector {
  int n, bs;
  const int msk = 63;
  vector<u64> bit, acc;
  BitVector() {}
  BitVector(int n) : n(n), bs((n+msk)>>6), bit(bs+1, 0), acc(bs+1, 0) {}
  void set(int k) {  bit[k>>6] |= (1ull) << (k & msk); }
  void build() { rep(i, bs) acc[i+1] += acc[i] + __builtin_popcountll(bit[i]); }
  bool access(int k) { return (bit[k>>6] >> (k&msk)) & 1; }
  int rank0(int r) { return r - rank1(r); }
  int rank1(int r) { return acc[r>>6] + __builtin_popcountll(bit[r>>6] & ((1ull << (r&msk)) - 1)); }
};
template<typename T, const int log=31>
struct WaveletMatrix {
  array<BitVector, log> v;
  array<int, log> mid;
  int n;
  WaveletMatrix(vector<T> a) : n(a.size()) {
    for (int bit = log-1; bit >= 0; --bit) {
      vector<T> zero, one;
      v[bit] = BitVector(n);
      rep(i, n) {
        if (a[i] >> bit & 1) {
          v[bit].set(i);
          one.emplace_back(a[i]);
        } else {
          zero.emplace_back(a[i]);
        }
      }
      v[bit].build();
      mid[bit] = zero.size();
      a = zero;
      a.insert(a.end(), one.begin(), one.end());
    }
  }
  int rank(int r, int x) { // a[0, r)に含まれるxの個数
    int l = 0;
    for (int bit = log-1; bit >= 0; --bit) {
      if (x >> bit & 1) {
        l = v[bit].rank1(l) + mid[bit];
        r = v[bit].rank1(r) + mid[bit];
      } else {
        l = v[bit].rank0(l);
        r = v[bit].rank0(r);
      }
    }
    return r - l;
  }
  T kth_smallest(int l, int r, int k) {
    T s = 0;
    for (int bit = log-1; bit >= 0; --bit) {
      int r0 = v[bit].rank0(r), l0 = v[bit].rank0(l);
      int cnt = r0 - l0;
      if (cnt <= k) {
        s |= (T)1 << bit;
        k -= cnt;
        l = l - l0 + mid[bit];
        r = r - r0 + mid[bit];
      } else {
        l = l0; r = r0;
      }
    }
    return s;
  }
  T kth_largest(int l, int r, int k) { return kth_smallest(l, r, r-l-k-1); }
  int range_freq(int l, int r, T x) { // a[l, r)でx未満の要素数
    int ans = 0;
    for (int bit = log-1; bit >= 0; --bit) {
      int l0 = v[bit].rank0(l), r0 = v[bit].rank0(r);
      if (x >> bit & 1) {
        ans += r0 - l0;
        l += mid[bit] - l0;
        r += mid[bit] - r0;
      } else {
        l = l0; r = r0;
      }
    }
    return ans;
  }
  int range_freq(int l, int r, T x, T y) { // a[l, r)でx以上y未満である要素数
    return range_freq(l, r, y) - range_freq(l, r, x);
  }
  T prev_value(int l, int r, T y) const { // a[l, r)でy未満であるような要素のうち最大
    int x = range_freq(l, r, y);
    if (x == 0) return -1;
    return kth_smallest(l, r, x-1);
  }
  T next_value(int l, int r, T x) const {
    int c = range_freq(l, r, x);
    if (c == r - l) return -1;
    return kth_smallest(l, r, c);
  }
};