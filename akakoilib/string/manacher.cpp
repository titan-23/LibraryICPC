// r[i]: 中心 i の奇数長回文の最大半径(回文長 = 2*r[i]-1)
vector<int> Manacher(string s) {
  int n = s.size();
  int i = 0, j = 0;
  vector<int> r(n);
  while(i < n) {
    while(i >= j && i + j < n && s[i-j] == s[i+j]) j++;
    r[i] = j;
    int k = 1;
    while(i >= k && i+k < n && k + r[i-k] < j) {
      r[i+k] = r[i-k];
      k++;
    }
    i += k, j -= k;
  }
  return r;
}