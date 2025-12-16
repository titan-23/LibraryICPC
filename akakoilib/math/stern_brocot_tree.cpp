template<class T>
struct SternBrocotTree {
  struct Node {
    // 開区間 (p/q, r/s), (p+r)/(q+s) がこのノードの値
    T p, q, r, s;
    T den() const { return q + s; } /// @brief 分母
    T num() const { return p + r; } /// @brief 分子
    Node left(T d) const { return {p, q, r+p*d, s+q*d}; }
    Node right(T d) const { return {p+r*d, q+s*d, r, s}; }
    pair<pair<T, T>, pair<T, T>> range() const { return {{p, q}, {r, s}}; } // 開区間を返す
  };
  Node root() { return {0, 1, 1, 0}; }
  pair<char, T> get_parent_step(const Node &v) {
    T ls = v.p + v.q, rs = v.r + v.s;
    if (ls == rs) return {'?', T(0)};
    if (ls < rs) {
      T k = rs / ls;
      if (rs % ls == 0) --k;
      return {'L', k};
    } else {
      T k = ls / rs;
      if (ls % rs == 0) --k;
      return {'R', k};
    }
  }
  constexpr T internal_gcd(T a, T b) {
    while (b != 0) { T temp = b; b = a % b; a = temp; }
    return a;
  }
  Node get_node(T p, T q) { // p/qに対応するノードを返す
    T g = internal_gcd(p, q);
    p /= g; q /= g;
    T tp = p, tq = q;
    Node now = root();
    while (tp != 1 || tq != 1) {
      if (tp == tq) break;
      if (tp > tq) {
        T k = tp / tq;
        if (tp % tq == 0) --k;
        now = now.right(k); tp -= tq * k;
      } else {
        T k = tq / tp;
        if (tq % tp == 0) --k;
        now = now.left(k);  tq -= tp * k;
      }
    }
    return now;
  }
  vector<pair<char, T>> encode_path(const Node &a) {
    vector<pair<char, T>> path;
    Node now = a;
    while (1) {
      auto [dir, cnt] = get_parent_step(now);
      if (cnt == T(0)) break;
      path.emplace_back(dir, cnt);
      if (dir == 'L') {
        now.r -= now.p * cnt;
        now.s -= now.q * cnt;
      } else {
        now.p -= now.r * cnt;
        now.q -= now.s * cnt;
      }
    }
    reverse(path.begin(), path.end());
    return path;
  }
  Node decode_path(const vector<pair<char, T>> &P) {
    Node now = root();
    for (const auto &[d, c] : P) {
      now = (d == 'L') ? now.left(c) : now.right(c);
    }
    return now;
  }
  Node lca(const Node &a, const Node &b) {
    auto pathA = encode_path(a);
    auto pathB = encode_path(b);
    vector<pair<char, T>> res;
    int n = min(pathA.size(), pathB.size());
    rep(i, n) {
      if (pathA[i].first != pathB[i].first) break;
      res.emplace_back(pathA[i].first, min(pathA[i].second, pathB[i].second));
      if (pathA[i].second != pathB[i].second) break;
    }
    return decode_path(res);
  }
  /// @brief nodeの祖先であって、深さがkのノードを返す
  pair<bool, Node> ancestor(const Node &node, T k) {
    auto path = encode_path(node);
    T dep = 0;
    for (auto &[_, c] : path) dep += c;
    if (k > dep) return {false, Node{}};
    Node res = root();
    T now = 0;
    for (const auto &[d, c] : path) {
      T need = k - now;
      if (need <= 0) break;
      if (need <= c) {
        res = d == 'L' ? res.left(need) : res.right(need);
        break;
      } else {
        res = d == 'L' ? res.left(c) : res.right(c);
        now += c;
      }
    }
    return {true, res};
  }
  /// @brief 判定関数fの境界を探索する / O(log d)
  /// @brief f(0/1) != f(1/0) であること
  /// @param d 探索する分母分子の上限
  /// @return 境界の区間を持つノード
  /// node.p/node.q: f(0/1)と同じ値を返す最大の分数)
  /// node.r/node.s: f(1/0)と同じ値を返す最小の分数)
  template<class F>
  Node binary_search(F f, T d) {
    Node v = root();
    bool bl = f(0, 1), bm = f(1, 1);
    while (1) {
      bool R = (bl == bm);
      T cx = R ? v.p : v.r, cy = R ? v.q : v.s;
      T sx = R ? v.r : v.p, sy = R ? v.s : v.q;
      T lim = d;
      if (sx) lim = min(lim, (d - cx) / sx);
      if (sy) lim = min(lim, (d - cy) / sy);
      if (lim == 0) return v;
      auto check = [&] (T k) -> bool {
        return (f(cx+sx*k, cy+sy*k) == bl) == R;
      };
      T k = 1;
      while (k <= lim && check(k)) {
        if (k > lim / 2) { k = lim + 1; break; }
        k *= 2;
      }
      T ok = max((T)1, k / 2), ng = min(k, lim + 1);
      while (ng - ok > 1) {
        T mid = ok + (ng - ok) / 2;
        (check(mid) ? ok : ng) = mid;
      }
      if (ok > lim) ok = lim;
      v = R ? v.right(ok) : v.left(ok);
      if (ok == lim) return v;
      bm = f(v.num(), v.den());
    }
  }
};