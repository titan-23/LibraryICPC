template <class T, T (*op)(T, T), T (*e)()>
struct Seg2DLight {
private:
  int h, w, hs, ws;
  vector<vector<T>> data;
  inline int bit_length(int x) {
    return x ? 32 - __builtin_clz(x) : 0;
  }
public:
  Seg2DLight(int h, int w) : h(h), w(w) {
    hs = 1<<bit_length(h);
    ws = 1<<bit_length(w);
    data.resize(hs*2, vector<T>(ws*2, e()));
  }
  void build(vector<vector<T>> a) {
    rep(i, h) rep(j, w) { data[i+hs][j+ws] = a[i][j]; }
    for (int i = hs; i < hs+h; ++i) {
      for (int j = ws-1; j > 0; --j) {
        data[i][j] = op(data[i][j<<1], data[i][j<<1|1]);
      }
    }
    for (int i = hs-1; i > 0; --i) {
      for (int j = 1; j < ws*2; ++j) {
        data[i][j] = op(data[i<<1][j], data[i<<1|1][j]);
      }
    }
  }
  T get(int y, int x) { return data[y+hs][x+ws]; }
  void set(int y, int x, T v) {
    assert(0 <= y && y < h); assert(0 <= x && x < w);
    y += hs; x += ws;
    data[y][x] = v;
    for (int nx = x; nx > 1; nx >>= 1) {
      data[y][nx>>1] = op(data[y][nx], data[y][nx^1]);
    }
    for (int ny = y; ny > 1; ny >>= 1) {
      int nx = x;
      data[ny>>1][nx] = op(data[ny][nx], data[ny^1][nx]);
      for (nx >>= 1; nx >= 1; nx >>= 1) {
        data[ny>>1][nx] = op(data[ny>>1][nx<<1], data[ny>>1][nx<<1|1]);
      }
    }
  }
  T prod(int u, int d, int l, int r) {
    assert(0 <= u && u <= d && d <= h);
    assert(0 <= l && l <= r && r <= w);
    u += hs; d += hs;
    l += ws; r += ws;
    T ans = e();
    auto pw = [&] (vector<T> &dat) -> void {
      int nl = l, nr = r;
      while (nl < nr) {
        if (nl & 1) ans = op(ans, dat[nl++]);
        if (nr & 1) ans = op(ans, dat[--nr]);
        nl >>= 1; nr >>= 1;
      }
    };
    while (u < d) {
      if (u & 1) pw(data[u++]);
      if (d & 1) pw(data[--d]);
      u >>= 1; d >>= 1;
    }
    return ans;
  }
};