template<int M, int R>
void ntt(vector<mint<M>>& a, bool inv) {
  int n = a.size();
  for (int i = 0, j = 1; j < n-1; ++j) {
    for (int k = (n>>1); k > (i^=k); k >>= 1);
    if (j < i) swap(a[i], a[j]);
  }
  mint<M> r = R, ir = r.inv();
  for (int k = 1; k < n; k <<= 1) {
    mint<M> w_base = (inv ? ir : r).pow((M-1)/(k<<1));
    for (int i = 0; i < n; i += k<<1) {
      mint<M> w = 1;
      rep(j, k) {
        mint<M> u = a[i+j], v = a[i+j+k]*w;
        a[i+j] = u+v; a[i+j+k] = u-v;
        w *= w_base;
      }
    }
  }
  if (!inv) return;
  mint<M> vinv = mint<M>(n).inv();
  rep(i, n) a[i] *= vinv;
}
template<int M, int R> vector<mint<M>>
sub(const vector<int> &A, const vector<int> &B, int z) {
  int n = 1; while (n < z) n <<= 1;
  vector<mint<M>> a(n), b(n);
  rep(i, A.size()) a[i] = A[i];
  rep(i, B.size()) b[i] = B[i];
  ntt<M,R>(a,0); ntt<M,R>(b,0);
  rep(i, n) a[i] *= b[i];
  ntt<M, R>(a,1); return a;
}
template<typename T>
vector<T> arbitrary_mod_convolution(vector<T> a, vector<T> b) {
  if(a.empty() || b.empty()) return {};
  int z = a.size() + b.size() - 1;
  vector<int> A(a.size()), B(b.size());
  rep(i, a.size()) A[i] = a[i].val();
  rep(i, b.size()) B[i] = b[i].val();
  constexpr int m1 = 167772161, m2 = 469762049, m3 = 754974721;
  auto c1 = sub<m1, 3>(A, B, z);
  auto c2 = sub<m2, 3>(A, B, z);
  auto c3 = sub<m3, 11>(A, B, z);
  ull mod = (T(0) - T(1)).val() + 1;
  vector<T> res(z);
  ll i12 = mint<m2>(m1).inv().val();
  ll i123 = mint<m3>((ll)m1*m2).inv().val();
  ll m12 = (ll)m1*m2;
  rep(i, z) {
    ll v1 = c1[i].val(), v2 = c2[i].val(), v3 = c3[i].val();
    ll t1 = (v2 - v1 + m2) * i12 % m2;
    ll x = v1 + m1 * t1;
    ll t2 = (v3 - x % m3 + m3) * i123 % m3;
    res[i] = (x + (__int128)m12 * t2) % mod;
  }
  return res;
}