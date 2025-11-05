pair<ll, ll> CRT(const vector<ll>& r, const vector<ll>& m) {
  ll n = r.size();
  ll rr = 0, mm = 0;
  for(int i = 0; i < n; i++) {
    auto [g, x, y] = ExtGcd(mm, m[i]);
    if((r[i] - rr) % g) return {0, -1};
    ll z = (r[i] - rr) / g * x % (m[i] / g);
    rr += mm * z;
    mm *= m[i] / g;
  }
  return {rr, mm};
}