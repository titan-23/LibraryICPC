//(x, y, g) s.t. ax + by = g, g = gcd(a, b)
tuple<ll, ll, ll> ExtGcd(ll a, ll b) {
  if(b == 0) return {a, 1, 0};
  ll q = a / b, r = a % b;
  auto [g, s, t] = ExtGcd(b, r);
  ll x = t, y = s - q * t;
  return {g, x, y};
}
ll ModInv(ll a, ll m) {
  auto [g, x, y] = ExtGcd(a, m);
  if (g != 1) return -1;
  return x % m;
}