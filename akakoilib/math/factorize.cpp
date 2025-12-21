using i128 = __int128_t;
const vector<ll> A1 = {2, 7, 61};
const vector<ll> A2 = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
bool is_primell(ll n) {
  if (n <= 1) return false;
  if (n == 2 || n == 7 || n == 61) return true;
  if (n % 2 == 0) return false;
  ll s = 0, d = n-1;
  while (d%2 == 0) { d >>= 1; s++; }
  const vector<ll> &A = n < 4759123141 ? A1 : A2;
  for (ll a : A) {
    if (a >= n) break;
    i128 x = pow_mod<i128>(a, d, n);
    if (x == 1 || x == n-1) continue;
    for (ll r = 1; r < s; ++r) {
      x = x * x % n;
      if (x == 1) return false;
      else if (x == n-1) break;
    }
    if (x != n-1) return false;
  }
  return true;
}
Random trnd;
ll pollard_rho(ll n) {
  if (is_primell(n)) return n;
  while (1) {
    ll x = trnd.randint(0, n-1);
    ll c = trnd.randint(0, n-1);
    ll y = ((i128)x*x+c)%n;
    ll d = 1;
    auto f = [&] (ll x) -> i128 { return ((i128)x*x+c) % n; };
    while (d == 1) {
      d = gcd(abs(x-y), n);
      x = f(x);
      y = f(f(y));
    }
    if (1 < d && d < n) return pollard_rho(d);
  }
}
vector<ll> factorize(ll n) {
  vector<ll> res;
  while (n > 1) {
    ll f = pollard_rho(n);
    while (n%f == 0) {
      res.emplace_back(f);
      n /= f;
    }
  }
  sort(res.begin(), res.end());
  return res;
}