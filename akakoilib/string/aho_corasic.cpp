const int W = 26;
struct AhoCorasick {
  char B;
  struct Node {
    int cnt, fail;
    array<int, W> ch;
    Node() : cnt(0), fail(0) { ch.fill(-1); }
  };
  vector<Node> a;
  vector<vector<int>> failtree;
  int root;
  AhoCorasick(char B) : B(B), a(1), root(0), failtree(1) { a[0].fail = 0; }
  int add_string(const string &s) {
    int v = root;
    for (char c : s) {
      c -= B;
      if (a[v].ch[c] == -1) {
        a[v].ch[c] = a.size();
        a.emplace_back();
      }
      v = a[v].ch[c];
    }
    a[v].cnt++;
    return v;
  }
  void build() {
    queue<int> q;
    rep(i, W) {
      if (a[0].ch[i] != -1) {
        a[a[0].ch[i]].fail = 0;
        q.push(a[0].ch[i]);
      } else { a[0].ch[i] = 0; }
    }
    while (!q.empty()) {
      int v = q.front(); q.pop();
      a[v].cnt += a[a[v].fail].cnt;
      rep(i, W) {
        int x = a[v].ch[i];
        if (x != -1) {
          a[x].fail = a[a[v].fail].ch[i];
          q.push(x);
        } else { a[v].ch[i] = a[a[v].fail].ch[i]; }
      }
    }
    failtree.resize(a.size());
    rep(i, a.size()) if (i >= 1) {
      failtree[i].push_back(a[i].fail);
      failtree[a[i].fail].push_back(i);
    }
  }
  int next(int v, char c) const {
    return a[v].ch[c-B];
  }
};