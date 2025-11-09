template <typename T>
struct PotUF {
  int n;
  vector<int> par;
  vector<T> dw;
  PotUF(int n) : n(n), par(n, -1), dw(n, T(0)) {}
  int root(int x) {
    if (par[x] < 0) return x;
    int r = root(par[x]);
    dw[x] = dw[x] + dw[par[x]];
    return par[x] = r;
  }
  // weight[y] = weight[x]+w, 矛盾があればfalse
  bool unite(int x, int y, T w) {
    x = root(x); y = root(y);
    if (x == y) {
      auto [ok, val] = diff(x, y);
      return ok && (val == w);
    }
    w = w + dw[x] - dw[y];
    if (par[x] > par[y]) {
      swap(x, y);
      w = -w;
    }
    par[x] += par[y];
    par[y] = x;
    dw[y] = w;
    return true;
  }
  int size(int x) { return -par[root(x)]; }
  bool same(int x, int y) { return root(x) == root(y); }
  pair<bool, T> diff(int x, int y) {
    if (!same(x, y)) return {false, T{}};
    return {true, dw[y]-dw[x]};
  }
};