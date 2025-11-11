using i128 = __i128_t;
// const vector<ll> A1 = {2, 7, 61}; // 4759123141
const vector<ll> A2 = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
bool is_primell(ll n) {
  if (n <= 1) return false;
  if (n == 2 || n == 7 || n == 61) return true;
  if (n % 2 == 0) return false;
  ll d = n - 1;
  ll s = __builtin_ctzll(d);
  d >>= s;
  for (const ll a : A2) {
    if (n <= a) return true;
    ll t, x = pow_mod<i128>(a, d, n);
    if (x != 1) {
      for (t = 0; t < s; ++t) {
        if (x == n - 1) break;
        x = (i128)x * x % n;
      }
      if (t == s) return false;
    }
  }
  return true;
}
class PollardRho {
private:
  inline int bit_length(ll x) const { return x ? 64 - __builtin_clzll(x) : 0; }
  ll pollard_rho(ll n) {
    if (n & 1 == 0) return 2;
    if (n % 3 == 0) return 3;
    int s = bit_length((n - 1) & (1 - n)) - 1;
    ll d = n >> s;
    for (ll a : A2) {
      ll p = pow_mod<i128>(a, d, n);
      if (p == 1 || p == n-1 || a%n == 0) continue;
      bool upd = false;
      rep(i, s) {
        ll prev = p;
        p = ((i128)p * p) % n;
        if (p == 1) return gcd(prev-1, n);
        if (p == n-1) {
          upd = true;
          break;
        }
      }
      if (!upd) {
        for (ll i = 2; i < n; ++i) {
          ll x = i, y = ((i128)i * i + 1) % n;
          ll f = gcd(abs(x - y), n);
          while (f == 1) {
            ll x = ((i128)x * x + 1) % n;
            ll y = ((i128)y * y + 1) % n;
            y = ((i128)y * y + 1) % n;
            ll f = gcd(abs(x - y), n);
          }
          if (f != n) return f;
        }
      }
    }
    return n;
  }
public:
  map<ll, int> factorize(ll n) {
    map<ll, int> res;
    stack<ll> todo; todo.emplace(n);
    while (!todo.empty()) {
      ll v = todo.top(); todo.pop();
      if (v <= 1) continue;
      ll f = pollard_rho(v);
      if (is_primell(f)) {
        int cnt = 0;
        while (v % f == 0) { cnt++; v /= f; }
        res[f] += cnt;
        todo.emplace(v);
      } else if (is_primell(v/f)) {
        f = v / f;
        int cnt = 0;
        while (v % f == 0) { cnt++; v /= f; }
        res[f] += cnt;
        todo.emplace(v);
      } else {
        todo.emplace(f); todo.emplace(v / f);
      }
    }
    return res;
  }
};