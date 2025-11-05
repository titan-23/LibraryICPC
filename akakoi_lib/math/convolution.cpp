mint g = 3;  // 原始根
void FFT(vector<mint>& a, bool inv = false) {
 int n = a.size(), s = __lg(n);
 static vector<mint> z, iz;
 while (z.size() <= s) {
  z.push_back(g.pow(mint(-1).x / (1 << z.size())));
  iz.push_back(z.back().inv());
 }
 vector<mint> b(n);
 for (int i = 1; i <= s; i++) {
  int w = 1 << (s - i);
  mint base = inv ? iz[i] : z[i], now = 1;
  for (int y = 0; y < n / 2; y += w) {
   for (int x = 0; x < w; x++) {
    auto l = a[y << 1 | x], r = now * a[y << 1 | x | w];
    b[y | x] = l + r;
    b[y | x | n >> 1] = l - r;
   }
   now *= base;
  }
  swap(a, b);
 }
}
vector<mint> Convolution(vector<mint> a, vector<mint> b) {
 int n = a.size(), m = b.size();
 if (!n || !m) return {};
 if (min(n, m) <= 30) {
  vector<mint> ans(n + m - 1);
  for (int i = 0; i < n; i++) {
   for (int j = 0; j < m; j++) {
    ans[i + j] += a[i] * b[j];
   }
  }
  return ans;
 }
 int N = n + m - 1;
 int z = bit_ceil(unsigned(N));
 a.resize(z), b.resize(z);
 FFT(a), FFT(b);
 for (int i = 0; i < z; i++) a[i] *= b[i];
 FFT(a, true);
 a.resize(n + m - 1);
 mint iz = mint(z).inv();
 for (auto& e : a) e *= iz;
 return a;
}