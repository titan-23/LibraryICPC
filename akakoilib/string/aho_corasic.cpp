struct AhoCorasick {
  struct Node {
    int fail, count, to[26];
    Node() : fail(0), count(0) { fill(to, to+26, -1); }
  };
  vector<Node> a;
  AhoCorasick() { a.emplace_back(); }
  void add_string(string s) {
    int now = 0;
    for (char c : s) {
      if (a[now].to[c-'a'] == -1) {
        a[now].to[c-'a'] = a.size();
        a.emplace_back();
      }
      now = a[now].to[c-'a'];
    }
    a[now].count++;
  }
  void build() {
    queue<int> q;
    rep(i, 26) {
      if (a[0].to[i] != -1) q.push(a[0].to[i]);
      else a[0].to[i] = 0;
    }
    while (!q.empty()) {
      int v = q.front(); q.pop();
      a[v].count += a[a[v].fail].count;
      rep(i, 26) {
        if (a[v].to[i] != -1) {
          int x = a[v].to[i];
          a[x].fail = a[a[v].fail].to[i];
          q.push(x);
        } else a[v].to[i] = a[a[v].fail].to[i];
      }
    }
  }
};