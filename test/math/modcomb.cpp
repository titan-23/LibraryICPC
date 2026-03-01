// https://judge.yosupo.jp/submission/356474

#include "./../../akakoilib/template/template.cpp"
// #include "./../../akakoilib/math/modint.cpp"
// mint(123456789)*mint(888777666)/mint(777666555)-mint(666555444).pow(555444333)+mint(987654321)
// 998244353 -> 514992251
// 1000000007 -> 435462530
using ull = unsigned long long;
// template <ull mod>

ull mod;

struct mint {
  ull x;
  mint(const ull x = 0) : x(x % mod) {} // `x >= 0 !`
  mint operator-() const { return mint(mod - x); }
  mint operator+(const mint &y) const { return mint(*this) += y; }
  mint operator-(const mint &y) const { return mint(*this) -= y; }
  mint operator*(const mint &y) const { return mint(*this) *= y; }
  mint operator/(const mint &y) const { return mint(*this) /= y; }
  mint &operator+=(const mint &y) {
    x += y.x;
    if (x >= mod) x -= mod;
    return *this;
  }
  mint &operator-=(const mint &y) {
    if (x < y.x) x += mod;
    x -= y.x;
    return *this;
  }
  mint &operator*=(const mint &y) {
    x = x * y.x % mod;
    return *this;
  }
  mint &operator/=(const mint &y) {
    x = x * y.inv().x % mod;
    return *this;
  }
  mint pow(ull n) const {
    mint r(1), m(x);
    while (n) {
      if (n & 1) r *= m;
      m *= m;
      n >>= 1;
    }
    return r;
  }
  mint inv() const { return pow(mod - 2); }
  bool operator==(const mint &y) const { return x == y.x; }
};

#include "./../../akakoilib/math/mod_comb.cpp"

// -----------------------

void solve() {
    int t; cin >> t >> mod;
    ModComb mc(1e7);
    rep(tdx, t) {
        int n, k; cin >> n >> k;
        cout << mc.nCr(n, k).x << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(15);
    cerr << fixed << setprecision(15);

    int t = 1;
    // cin >> t;
    for (int i = 0; i < t; ++i) {
        solve();
    }
    return 0;
}
