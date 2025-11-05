tuple<ll, ll, ll> ext_gcd(ll a, ll b) {
 if(b == 0) return {a, 1, 0};
 ll q = a / b, r = a % b;
 auto [g, s, t] = ext_gcd(b, r);
 ll x = t, y = s - q * t;
 return {g, x, y};
}
ll modinv_gcd(ll a, ll m) {
 auto [g, x, y] = ext_gcd(a, m);
 if (g != 1) return -1;
 return x % m;
}