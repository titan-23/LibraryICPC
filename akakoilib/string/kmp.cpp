// p[i] = s[0..i] の接頭辞と接尾辞が一致する最大長
vector<int> kmp(const string &s) {
  int n = s.size();
  vector<int> p(n);
  for (int i = 1; i < n; i++) {
    int g = p[i - 1];
    while (g && s[i] != s[g]) g = p[g - 1];
    p[i] = g + (s[i] == s[g]);
  }
  return p;
}
vector<int> search(const string &s, const string &t) {
  vector<int> res, p = kmp(t);
  int n = s.size(), m = t.size(), j = 0;
  rep(i, s.size()) {
    while (j > 0 && s[i] != t[j]) j = p[j-1];
    if (s[i] == t[j]) j++;
    if (j == m) {
        res.push_back(i-m+1);
        j = p[j-1];
    }
  }
  return res;
}
string primitive_root(const string &s) {
  auto p = kmp(s);
  int n = s.size();
  int t = n - p[n - 1];
  return n%t == 0 ? s.substr(0, t) : s;
}