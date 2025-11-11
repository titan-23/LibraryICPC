void FHT(vector<mint>& a, bool inv = false) {
  int h = __lg(a.size());
  rep(i, h) rep(j, 1 << h) if (~j >> i & 1) {
    mint x = a[j], y = a[j | 1 << i];
    a[j] = x + y; a[j | 1 << i] = x - y;
    if (inv) a[j] /= 2, a[j | 1 << i] /= 2;
  }
  return;
}

vector<mint> XorConvolution(vector<mint> a, vector<mint> b) {
  FHT(a); FHT(b);
  rep(i, a.size()) a[i] *= b[i];
  FHT(a, true);
  return a;
}