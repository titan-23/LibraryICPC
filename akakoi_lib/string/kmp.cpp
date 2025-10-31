vector<ll> Kmp(string s) {
   int n = s.size();
   vector<ll> p(n);
   for(int i = 1; i < n; i++) {
      ll g = p[i - 1];
      while(g && s[i] != s[g]) g = p[g - 1];
      p[i] = g + (s[i] == s[g]);
   }
   return p;
}