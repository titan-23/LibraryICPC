// z[i]: s と s[i..] のLCP長
vector<int> ZAlgorithm(const vector<int>& s) {
  int n = s.size(), l = -1, r = -1;
  vector<int> z(n, n);
  for(int i = 1; i < n; ++i) {
    int& x = z[i] = (i < r) ? min(r - i, z[i - l]) : 0;
    while(i + x < n && s[i + x] == s[x]) x++;
    if(i + x > r) l = i, r = i + x;
  }
  return z;
}