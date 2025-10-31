struct Matching {
  int n;
  vector<vector<int>> g;
  vector<int> mt;
  vector<int> is_ev, gr_buf;
  vector<pair<int, int>> nx;
  int st;

  int group(int x) {
    if (gr_buf[x] == -1 || is_ev[gr_buf[x]] != st) return gr_buf[x];
    return gr_buf[x] = group(gr_buf[x]);
  }

  void match(int p, int b) {
    int d = mt[p];
    mt[p] = b;
    if (d == -1 || mt[d] != p) return;
    if (nx[p].second == -1) {
      mt[d] = nx[p].first;
      match(nx[p].first, d);
    } else {
      match(nx[p].first, nx[p].second);
      match(nx[p].second, nx[p].first);
    }
  }

  bool arg() {
    is_ev[st] = st;
    gr_buf[st] = -1;
    nx[st] = pair<int, int>(-1, -1);
    queue<int> q;
    q.push(st);
    while (!q.empty()) {
      int a = q.front();
      q.pop();
      for (auto b : g[a]) {
        if (b == st) continue;
        if (mt[b] == -1) {
          mt[b] = a;
          match(a, b);
          return true;
        }
        if (is_ev[b] == st) {
          int x = group(a), y = group(b);
          if (x == y) continue;
          int z = -1;
          while (x != -1 || y != -1) {
            if (y != -1) swap(x, y);
            if (nx[x] == pair<int, int>(a, b)) {
              z = x;
              break;
            }
            nx[x] = pair<int, int>(a, b);
            x = group(nx[mt[x]].first);
          }
          for (int v : {group(a), group(b)}) {
            while (v != z) {
              q.push(v);
              is_ev[v] = st;
              gr_buf[v] = z;
              v = group(nx[mt[v]].first);
            }
          }
        } else if (is_ev[mt[b]] != st) {
          is_ev[mt[b]] = st;
          nx[b] = pair<int, int>(-1, -1);
          nx[mt[b]] = pair<int, int>(a, -1);
          gr_buf[mt[b]] = b;
          q.push(mt[b]);
        }
      }
    }
    return false;
  }

  Matching(const vector<vector<int>>& _g) : n(int(_g.size())), g(_g), mt(n, -1), is_ev(n, -1), gr_buf(n), nx(n) {
    for (st = 0; st < n; st++) {
      if (mt[st] == -1) arg();
    }
  }

  vector<pair<int, int>> max_match() {
    vector<pair<int, int>> res;
    for (int i = 0; i < n; ++i) {
      if (i < mt[i]) res.push_back(pair<int, int>(i, mt[i]));
    }
    return res;
  }
};
