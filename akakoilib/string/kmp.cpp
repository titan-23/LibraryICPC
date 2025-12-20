// p[i] = s[0..i] の接頭辞と接尾辞が一致する最大長
vector<int> kmp(const string &s) {
  int n = s.size();
  vector<int> p(n);
  for(int i = 1; i < n; i++) {
    int g = p[i - 1];
    while(g && s[i] != s[g]) g = p[g - 1];
    p[i] = g + (s[i] == s[g]);
  }
  return p;
}
string primitive_root(const string &s) {
  auto p = kmp(s);
  int n = s.size();
  int t = n - p[n - 1];
  if(n % t == 0) return s.substr(0, t);
  return s;
}