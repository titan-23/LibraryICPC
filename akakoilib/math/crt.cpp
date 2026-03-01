// {x, M} (x≡r[i] mod m[i]) / 解なしなら {0, -1}
pair<ll, ll> CRT(const vector<ll>& r, const vector<ll>& m) {
  ll n = r.size(), rr = 0, mm = 0;
  rep(i, n) {
    auto [g, x, y] = ExtGcd(mm, m[i]);
    if ((r[i] - rr) % g) return {0, -1};
    ll z = (r[i] - rr) / g * x % (m[i] / g);
    rr += mm * z;
    mm *= m[i] / g;
  }
  return {rr, mm};
}