// https://judge.yosupo.jp/submission/356460
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/math/modint.cpp"
// #include "./../../akakoilib/math/convolution.cpp"

const int mod = 998244353;

// NTT Friendly Primes:
// 998244353 (2^23, root 3)
// 167772161 (2^25, root 3)
// 754974721 (2^24, root 11)
// 長さ $2^{24}$ まで畳み込み可能 このとき mod は 2663300486
void NTT(vector<mint<mod>> &a, bool inv = false) {
  static vector<mint<mod>> r, ir, tmp;
  if (r.empty()) {
    int dm = 1; ll n = 998244352;
    while (n % 2 == 0) n >>= 1, dm++;
    r.resize(dm + 1); ir.resize(dm + 1); r[0] = ir[0] = 1;
    for (int i = 1; i <= dm; i++) {
      r[i] = mint<mod>(3).pow(998244352 >> i);
      ir[i] = r[i].inv();
    }
  }
  tmp.resize(a.size()); int n = a.size(), p = 0;
  for (int i = n >> 1; i; i >>= 1, p++) {
    auto &cur = p & 1 ? tmp : a, &nxt = p & 1 ? a : tmp;
    mint<mod> w = 1, e = inv ? r[p + 1] : ir[p + 1];
    for (int j = 0; j < n; j += i, w *= e) rep(k, i) {
      nxt[j + k] = cur[((j << 1) & (n - 1)) + k] + w * cur[(((j << 1) + i) & (n - 1)) + k];
    }
  }
  if (p & 1) swap(a, tmp);
  if (inv) for (mint<mod> &x : a) x *= mint<mod>(n).inv();
}
vector<mint<mod>> Convolve(vector<mint<mod>> a, vector<mint<mod>> b) {
  if (a.empty() || b.empty()) return {};
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

// -----------------------

void solve() {
    int n, m; cin >> n >> m;
    vector<int> A(n), B(m);
    rep(i, n) cin >> A[i];
    rep(i, m) cin >> B[i];
    vector<mint<mod>> X(n), Y(m);
    rep(i, n) X[i] = A[i];
    rep(i, m) Y[i] = B[i];
    auto ans = Convolve(X, Y);
    rep(i, n+m-1) {
        cout << ans[i].x << " ";
    }
    cout << "\n";
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    solve();

    return 0;
}
