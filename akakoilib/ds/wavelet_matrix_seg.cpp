// applyはgetの点のみでよい
#define all(a) (a).begin(), (a).end()
template<typename T, typename Mo, Mo(*op)(Mo, Mo), Mo(*e)()>
class WaveletMatrixSeg {
private:
  int log;
  vector<tuple<T, T, Mo>> pos;
  vector<BitVector> v;
  vector<pair<T, T>> xy;
  vector<T> y;
  vector<int> mid;
  vector<Segtree<Mo, op, e>> seg;
public:
  WaveletMatrixSeg() {}
  void set_point(T x, T y, Mo w) {pos.emplace_back(x, y, w);}
  void build() {
    xy.reserve(pos.size());
    for (auto &[x, y, _]: pos) xy.emplace_back(x, y);
    sort(all(xy)); xy.erase(unique(all(xy)), xy.end());
    y.reserve(xy.size());
    for (auto &[x, y_]: xy) y.emplace_back(y_);
    sort(all(y)); y.erase(unique(all(y)), y.end());
    log = y.size() == 0 ? 0 : 32 - __builtin_clz(y.size());
    v.resize(log); mid.resize(log); seg.resize(log);
    vector<T> a; a.reserve(xy.size());
    for (auto &[_, v]: xy) {
      a.emplace_back(lower_bound(all(y), v) - y.begin());
    }
    for (int bit = log-1; bit >= 0; --bit) {
      vector<T> zero, one;
      v[bit] = BitVector(a.size());
      rep(i, a.size()) {
        if (a[i] >> bit & 1) {
          v[bit].set(i);
          one.emplace_back(a[i]);
        } else {
          zero.emplace_back(a[i]);
        }
      }
      v[bit].build(); mid[bit] = zero.size();
      a = zero; a.insert(a.end(), all(one));
    }
    vector<vector<Mo>> ws(log, vector<Mo>(xy.size(), e()));
    for (const auto &[x, y_, w]: pos) {
      int k = lower_bound(all(xy), make_pair(x, y_)) - xy.begin();
      int iy = lower_bound(all(y), y_) - y.begin();
      for (int bit = log-1; bit >= 0; --bit) {
        if (iy >> bit & 1) k = v[bit].rank1(k) + mid[bit];
        else k = v[bit].rank0(k);
        ws[bit][k] = op(ws[bit][k], w);
      }
    }
    rep(i, log) seg[i] = Segtree<Mo, op, e>(ws[i]);
  }
  void update_point(T x1, T y1, Mo val) {
    int k = lower_bound(all(xy), make_pair(x1, y1)) - xy.begin();
    int iy = lower_bound(all(y), y1) - y.begin();
    for (int bit = log-1; bit >= 0; --bit) {
      if (iy >> bit & 1) k = v[bit].rank1(k) + mid[bit];
      else k = v[bit].rank0(k);
      seg[bit].set(k, val);
    }
  }
  // 領域 [x1, x2) x [y1, y2) の prod を求める / O(log^2N)
  Mo range_prod(T x1, T x2, T y1, T y2) {
    int l = lower_bound(all(xy), make_pair(x1, (T)0)) - xy.begin();
    int r = lower_bound(all(xy), make_pair(x2, (T)0)) - xy.begin();
    int d = lower_bound(all(y), y1) - y.begin();
    int u = lower_bound(all(y), y2) - y.begin();
    if (log == 0) return e();
    auto dfs = [&] (auto&& dfs, int bit, int l, int r, int y_l, int y_r) -> Mo {
      if (l >= r || y_r <= d || y_l >= u) return e();
      int m = y_l + (1 << bit);
      int l0 = v[bit].rank0(l), r0 = v[bit].rank0(r);
      int l1 = v[bit].rank1(l) + mid[bit];
      int r1 = v[bit].rank1(r) + mid[bit];
      if ((d <= y_l && y_r <= u) || bit == 0) {
        Mo res = e();
        if (d < m) res = op(res, seg[bit].prod(l0, r0));
        if (m < u) res = op(res, seg[bit].prod(l1, r1));
        return res;
      }
      return op(dfs(dfs, bit-1, l0, r0, y_l, m),
                dfs(dfs, bit-1, l1, r1, m, y_r));
    };
    return dfs(dfs, log - 1, l, r, 0, 1 << log);
  }
  Mo sum(T x1, T x2, T y2) {
    int l = lower_bound(all(xy), make_pair(x1, (T)0)) - xy.begin();
    int r = lower_bound(all(xy), make_pair(x2, (T)0)) - xy.begin();
    int u = lower_bound(all(y), y2) - y.begin();
    Mo ans = e();
    for (int bit = log-1; bit >= 0; --bit) {
      int l0 = v[bit].rank0(l), r0 = v[bit].rank0(r);
      if (u>>bit & 1) {
        l += mid[bit] - l0; r += mid[bit] - r0;
        ans += seg[bit].prod(l0, r0);
      } else {
        l = l0; r = r0;
      }
    }
    return ans;
  }
};