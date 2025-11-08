void NTT(vector<mint> &a, bool inv = false) {
  static vector<mint> r, ir, tmp;
  if (r.empty()) {
    int dm = 1; ll n = 998244352;
    while (n % 2 == 0) n >>= 1, dm++;
    r.resize(dm + 1); ir.resize(dm + 1); r[0] = ir[0] = 1;
    for (int i = 1; i <= dm; i++) {
      r[i] = mint(3).pow(998244352 >> i);
      ir[i] = r[i].inv();
    }
  }
  tmp.resize(a.size()); int n = a.size(), p = 0;
  for (int i = n >> 1; i; i >>= 1, p++) {
    auto &cur = p & 1 ? tmp : a, &nxt = p & 1 ? a : tmp;
    mint w = 1, e = inv ? r[p + 1] : ir[p + 1];
    for (int j = 0; j < n; j += i, w *= e) rep(k, i) {
      nxt[j + k] = cur[((j << 1) & (n - 1)) + k] + w * cur[(((j << 1) + i) & (n - 1)) + k];
    }
  }
  if (p & 1) swap(a, tmp);
  if (inv) for (mint &x : a) x *= mint(n).inv();
}
vector<mint> Convolve(vector<mint> a, vector<mint> b) {
  int n = 1;
  int z = a.size() + b.size();
  while (n + 1 < z) n <<= 1;
  a.resize(n), b.resize(n);
  NTT(a), NTT(b);
  rep(i, n) a[i] *= b[i];
  NTT(a, true);
  while (a.size() + 1 > z) a.pop_back();
  return a;
}