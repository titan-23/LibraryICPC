pair<vector<ll>, vector<ll>> SA(string s) {
   ll n = s.size() + 1, lim = 256;
   vector<ll> sa(n), lcp(n), x(s.begin(), s.end()) + 1, y(n), ws(max(n, lim)), rk(n);
   iota(sa.begin(), sa.end(), 0);
   for(ll j = 0, p = 0; p < n; j = max(1LL, j * 2), lim = p) {
      p = j;
      iota(y.begin(), y.end(), n - j);
      for(ll i = 0; i < n; ++i)
         if(sa[i] >= j) y[p++] = sa[i] - j;
      fill(ws.begin(), ws.end(), 0);
      for(ll i = 0; i < n; ++i) ws[x[i]]++;
      for(ll i = 1; i < lim; ++i) ws[i] += ws[i - 1];
      for(ll i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
      swap(x, y);
      p = 1;
      x[sa[0]] = 0;
      for(ll i = 1; i < n; ++i) {
         ll a = sa[i - 1], b = sa[i];
         x[b] = (y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
      }
   }
   for(ll i = 1; i < n; ++i) rk[sa[i]] = i;
   for(ll i = 0, k = 0; i < n - 1; lcp[rk[i++]] = k) {
      if(k) k--;
      while(s[i + k] == s[sa[rk[i] - 1] + k]) k++;
   }
   sa.erase(sa.begin());
   lcp.erase(lcp.begin());
   return {sa, lcp};
}