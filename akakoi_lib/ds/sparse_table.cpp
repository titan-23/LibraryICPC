template <class T, T (*op)(T, T), T (*e)()>
struct SparseTable {
  int n;
  vector<T> data;
  vector<int> offset;
  SparseTable(vector<T> a) : n(a.size()) {
    int log = 32 - __builtin_clz(n) - 1;
    offset.resize(log+1);
    int sm = 0;
    for (int i = 0; i <= log; ++i) {
      offset[i] = sm;
      sm += n - (1<<i) + 1;
    }
    data.resize(sm);
    memcpy(data.data(), a.data(), n*sizeof(T));
    rep(i, log) {
      int l = 1 << i;
      int s = n - l + 1;
      const int x = s - l;
      const auto pre1 = &data[offset[i]];
      const auto pre2 = &data[offset[i]+l];
      auto nxt = &data[offset[i+1]];
      rep(j, x) nxt[j] = op(pre1[j], pre2[j]);
    }
  }
  T prod(int l, int r) {
    if (l == r) return e();
    int u = 32 - __builtin_clz(r-l) - 1;
    return op(data[offset[u]+l], data[offset[u]+r-(1<<u)]);
  }
  T get(int k) { return data[k]; }
};