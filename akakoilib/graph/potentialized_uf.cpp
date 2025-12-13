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
    int rx = root(x), ry = root(y);
    if (rx == ry) return dw[y] - dw[x] == w;
    w = w + dw[x] - dw[y];
    if (par[rx] > par[ry]) { swap(rx, ry); w = -w; }
    par[rx] += par[ry];
    par[ry] = rx;
    dw[ry] = w;
    return true;
  }
  int size(int x) { return -par[root(x)]; }
  bool same(int x, int y) { return root(x) == root(y); }
  pair<bool, T> diff(int x, int y) {
    if (!same(x, y)) return {false, T(0)};
    return {true, dw[y]-dw[x]};
  }
};