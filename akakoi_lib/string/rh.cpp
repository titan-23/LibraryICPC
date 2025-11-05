const ll mod = (1LL << 61) - 1;
ll add(ll a, ll b) { return (a += b) >= mod ? a - mod : a; }
ll mul(ll a, ll b) {
 __int128_t c = (__int128_t)a * b;
 return add(c >> 61, c & mod);
}
// mt19937 rnd(rndom_device{}());
// ll base = uniform_int_distribution<ll>(414123123, 1000000000)(rnd);
ll r = 7954398468495;
struct RH {
 ll n;
 vector<ll> hs, pw;
 RH(string s) : n(s.size()), hs(n + 1), pw(n + 1, 1) {
   for(int i = 0; i < n; ++i) {
    pw[i + 1] = mul(pw[i], r);
    hs[i + 1] = add(mul(hs[i], r), s[i]);
   }
 }
 ll get(ll l, ll r) const { return add(hs[r], mod - mul(hs[l], pw[r - l])); }
};