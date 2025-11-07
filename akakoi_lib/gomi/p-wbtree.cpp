template <class SizeType, class T, T (*op)(T, T), T (*e)(),
    class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()>
class PersistentLazyWBTree {
public:
  struct MemoeyAllocator {
    struct Node {
      SizeType left, right, size;
      Node() : left(0), right(0), size(0) {}
      Node(SizeType l, SizeType r, SizeType s) : left(l), right(r), size(s) {}
    };
    struct Data {
      T key, data; F lazy; int8_t rev;
      Data() : rev(0) {}
      Data(T k, T d, F l, int8_t r) : key(k), data(d), lazy(l), rev(r) {}
    };
    vector<Node> ch;
    vector<Data> dat;
    size_t ptr;
    MemoeyAllocator() : ptr(1) {
      ch.emplace_back(0, 0, 0);
      dat.emplace_back(T{}, T{}, F{}, 0);
    }
    SizeType copy(SizeType node) {
      SizeType idx = new_node(dat[node].key, dat[node].lazy);
      ch[idx] = {ch[node].left, ch[node].right, ch[node].size};
      dat[idx].data = dat[node].data;
      dat[idx].rev = dat[node].rev;
      return idx;
    }
    SizeType new_node(const T key, const F f) {
      if (ch.size() > ptr) {
        ch[ptr] = {0, 0, 1};
        dat[ptr] = {key, key, f, 0};
      } else {
        ch.emplace_back(0, 0, 1);
        dat.emplace_back(key, key, f, 0);
      }
      ptr++;
      return ptr - 1;
    }
    void reserve(SizeType n) {
      ch.reserve(n); dat.reserve(n);
    }
    void reset() { ptr = 1; }
  };
  static MemoeyAllocator ma;
private:
  using PLTM = PersistentLazyWBTree<SizeType, T, F, op, mapping, composition, e, id>;
  static constexpr int DELTA = 3, GAMMA = 2;
  SizeType root;
  inline SizeType weight_right(SizeType node) const { return ma.ch[ma.ch[node].right].size + 1; }
  inline SizeType weight_left(SizeType node) const { return ma.ch[ma.ch[node].left].size + 1; }
  inline SizeType weight(SizeType node) const { return ma.ch[node].size + 1; }
  void update(SizeType node) {
    ma.ch[node].size = 1 + ma.ch[ma.ch[node].left].size + ma.ch[ma.ch[node].right].size;
    ma.dat[node].data = ma.dat[node].key;
    if (ma.ch[node].left) ma.dat[node].data = op(ma.dat[ma.ch[node].left].data, ma.dat[node].key);
    if (ma.ch[node].right) ma.dat[node].data = op(ma.dat[node].data, ma.dat[ma.ch[node].right].data);
  }
  void push(SizeType node, F f) {
    ma.dat[node].key = mapping(f, ma.dat[node].key);
    ma.dat[node].data = mapping(f, ma.dat[node].data);
    ma.dat[node].lazy = composition(f, ma.dat[node].lazy);
  }
  void propagate(SizeType node) {
    if (ma.dat[node].rev) {
      SizeType l = ma.ch[node].left ? ma.copy(ma.ch[node].left) : 0;
      SizeType r = ma.ch[node].right ? ma.copy(ma.ch[node].right) : 0;
      ma.ch[node].left = r;
      ma.ch[node].right = l;
      if (ma.ch[node].left) ma.dat[ma.ch[node].left].rev ^= 1;
      if (ma.ch[node].right) ma.dat[ma.ch[node].right].rev ^= 1;
      ma.dat[node].rev = 0;
    }
    if (ma.dat[node].lazy != id()) {
      if (ma.ch[node].left) {
        ma.ch[node].left = ma.copy(ma.ch[node].left);
        push(ma.ch[node].left, ma.dat[node].lazy);
      }
      if (ma.ch[node].right) {
        ma.ch[node].right = ma.copy(ma.ch[node].right);
        push(ma.ch[node].right, ma.dat[node].lazy);
      }
      ma.dat[node].lazy = id();
    }
  }
  void balance_check(SizeType node) const {
    if (!(weight_left(node)*DELTA >= weight_right(node))) assert(false);
    if (!(weight_right(node) * DELTA >= weight_left(node))) assert(false);
  }
  void _build(vector<T> const &a) {
    auto build = [&] (auto &&build, SizeType l, SizeType r) -> SizeType {
      SizeType mid = (l + r) >> 1;
      SizeType node = ma.new_node(a[mid], id());
      if (l != mid) ma.ch[node].left = build(build, l, mid);
      if (mid+1 != r) ma.ch[node].right = build(build, mid+1, r);
      update(node);
      return node;
    };
    if (a.empty()) {
      root = 0; return;
    }
    root = build(build, 0, (SizeType)a.size());
  }
  SizeType _rotate_right(SizeType node) {
    SizeType u = ma.copy(ma.ch[node].left);
    ma.ch[node].left = ma.ch[u].right;
    ma.ch[u].right = node;
    update(node);
    update(u);
    return u;
  }
  SizeType _rotate_left(SizeType node) {
    SizeType u = ma.copy(ma.ch[node].right);
    ma.ch[node].right = ma.ch[u].left;
    ma.ch[u].left = node;
    update(node);
    update(u);
    return u;
  }
  SizeType _balance_left(SizeType node) {
    propagate(ma.ch[node].right);
    SizeType u = ma.ch[node].right;
    if (weight_left(ma.ch[node].right) >= weight_right(ma.ch[node].right) * GAMMA) {
      propagate(ma.ch[u].left);
      ma.ch[node].right = _rotate_right(u);
    }
    u = _rotate_left(node);
    return u;
  }
  SizeType _balance_right(SizeType node) {
    propagate(ma.ch[node].left);
    SizeType u = ma.ch[node].left;
    if (weight_right(ma.ch[node].left) >= weight_left(ma.ch[node].left) * GAMMA) {
      propagate(ma.ch[u].right);
      ma.ch[node].left = _rotate_left(u);
    }
    u = _rotate_right(node);
    return u;
  }
  SizeType _merge_with_root(SizeType l, SizeType root, SizeType r) {
    if (weight(r) * DELTA < weight(l)) {
      propagate(l);
      l = ma.copy(l);
      ma.ch[l].right = _merge_with_root(ma.ch[l].right, root, r);
      update(l);
      if (weight(ma.ch[l].left) * DELTA < weight(ma.ch[l].right)) return _balance_left(l);
      return l;
    } else if (weight(l) * DELTA < weight(r)) {
      propagate(r);
      r = ma.copy(r);
      ma.ch[r].left = _merge_with_root(l, root, ma.ch[r].left);
      update(r);
      if (weight(ma.ch[r].right) * DELTA < weight(ma.ch[r].left)) return _balance_right(r);
      return r;
    }
    root = ma.copy(root);
    ma.ch[root].left = l;
    ma.ch[root].right = r;
    update(root);
    return root;
  }
  pair<SizeType, SizeType> _pop_right(SizeType node) { return _split_node(node, ma.ch[node].size-1); }
  SizeType _merge_node(SizeType l, SizeType r) {
    if ((!l) && (!r)) { return 0; }
    if (!l) return ma.copy(r);
    if (!r) return ma.copy(l);
    l = ma.copy(l);
    r = ma.copy(r);
    auto [l_, root_] = _pop_right(l);
    return _merge_with_root(l_, root_, r);
  }
  pair<SizeType, SizeType> _split_node(SizeType node, SizeType k) {
    if (!node) { return {0, 0}; }
    propagate(node);
    SizeType lch = ma.ch[node].left, rch = ma.ch[node].right;
    SizeType tmp = lch ? k-ma.ch[lch].size : k;
    if (tmp == 0) {
      return {lch, _merge_with_root(0, node, rch)};
    } else if (tmp < 0) {
      auto [l, r] = _split_node(lch, k);
      return {l, _merge_with_root(r, node, rch)};
    } else {
      auto [l, r] = _split_node(rch, tmp-1);
      return {_merge_with_root(lch, node, l), r};
    }
  }
  PLTM _new(SizeType root) { return PLTM(root); }
  PersistentLazyWBTree(SizeType root) : root(root) {}
 public:
  PersistentLazyWBTree() : root(0) {}
  PersistentLazyWBTree(vector<T> &a) { _build(a); }
  PLTM merge(PLTM other) {
    SizeType root = _merge_node(this->root, other.root);
    return _new(root);
  }
  pair<PLTM, PLTM> split(SizeType k) {
    auto [l, r] = _split_node(this->root, k);
    return {_new(l), _new(r)};
  }
  PLTM apply(SizeType l, SizeType r, F f) {
    assert(0 <= l && l <= r && r <= len());
    if (l == r) return _new(ma.copy(root));
    auto dfs = [&] (auto &&dfs, SizeType node, SizeType left, SizeType right) -> SizeType {
      if (right <= l || r <= left) return node;
      propagate(node);
      SizeType nnode = ma.copy(node);
      if (l <= left && right < r) {
        push(nnode, f);
        return nnode;
      }
      SizeType lsize = ma.ch[ma.ch[nnode].left].size;
      if (ma.ch[nnode].left) ma.ch[nnode].left = dfs(dfs, ma.ch[nnode].left, left, left+lsize);
      if (l <= left+lsize && left+lsize < r) ma.dat[nnode].key = mapping(f, ma.dat[nnode].key);
      if (ma.ch[nnode].right) ma.ch[nnode].right = dfs(dfs, ma.ch[nnode].right, left+lsize+1, right);
      update(nnode);
      return nnode;
    };
    return _new(dfs(dfs, root, 0, len()));
  }
  T prod(SizeType l, SizeType r) {
    assert(0 <= l && l <= r && r <= len());
    if (l == r) return e();
    auto dfs = [&] (auto &&dfs, SizeType node, SizeType left, SizeType right) -> T {
      if (right <= l || r <= left) return e();
      if (l <= left && right < r) return ma.dat[node].data;
      propagate(node);
      SizeType lsize = ma.ch[ma.ch[node].left].size;
      T res = e();
      if (ma.ch[node].left) res = dfs(dfs, ma.ch[node].left, left, left+lsize);
      if (l <= left+lsize && left+lsize < r) res = op(res, ma.dat[node].key);
      if (ma.ch[node].right) res = op(res, dfs(dfs, ma.ch[node].right, left+lsize+1, right));
      return res;
    };
    return dfs(dfs, root, 0, len());
  }
  PLTM insert(SizeType k, T key) {
    assert(0 <= k && k <= len());
    auto [s, t] = _split_node(root, k);
    SizeType new_node = ma.new_node(key, id());
    return _new(_merge_with_root(s, new_node, t));
  }
  pair<PLTM, T> pop(SizeType k) {
    assert(0 <= k && k < len());
    auto [s_, t] = _split_node(this->root, k+1);
    auto [s, tmp] = _pop_right(s_);
    T res = ma.dat[tmp].key;
    SizeType root = _merge_node(s, t);
    return {_new(root), res};
  }
  PLTM reverse(SizeType l, SizeType r) {
    assert(0 <= l && l <= r && r <= len());
    if (l >= r) return _new(ma.copy(root));
    auto [s_, t] = _split_node(root, r);
    auto [u, s] = _split_node(s_, l);
    ma.dat[s].rev ^= 1;
    SizeType root = _merge_node(_merge_node(u, s), t);
    return _new(root);
  }
  vector<T> tovector() {
    SizeType node = root;
    stack<SizeType> s;
    vector<T> a;
    a.reserve(len());
    while (!s.empty() || node) {
      if (node) {
        propagate(node);
        s.emplace(node);
        node = ma.ch[node].left;
      } else {
        node = s.top(); s.pop();
        a.emplace_back(ma.dat[node].key);
        node = ma.ch[node].right;
      }
    }
    return a;
  }
  PLTM copy() const { return _new(copy(root)); }
  PLTM set(SizeType k, T v) {
    assert(0 <= k && k < len());
    SizeType node = ma.copy(root);
    SizeType root = node;
    SizeType pnode = 0;
    int d = 0;
    stack<SizeType> path = {node};
    while (1) {
      propagate(node);
      SizeType t = ma.ch[ma.ch[node].left].size;
      if (t == k) {
        node = ma.copy(node);
        ma.dat[node].key = v;
        path.emplace(node);
        if (d) ma.ch[pnode].left = node;
        else ma.ch[pnode].right = node;
        while (!path.empty()) {
          update(path.top());
          path.pop();
        }
        return _new(root);
      }
      pnode = node;
      if (t < k) {
        k -= t + 1;
        node = ma.copy(ma.ch[node].right);
        d = 0;
      } else {
        d = 1;
        node = ma.copy(ma.ch[node].left);
      }
      path.emplace_back(node);
      if (d) ma.ch[pnode].left = node;
      else ma.ch[pnode].right = node;
    }
  }
  T get(SizeType k) {
    assert(0 <= k && k < len());
    SizeType node = root;
    while (1) {
      propagate(node);
      SizeType t = ma.ch[ma.ch[node].left].size;
      if (t == k) {
        return ma.dat[node].key;
      }
      if (t < k) {
        k -= t + 1;
        node = ma.ch[node].right;
      } else {
        node = ma.ch[node].left;
      }
    }
  }
  SizeType len() const { return ma.ch[root].size; }
  static void rebuild(PLTM &tree) {
    PLTM::ma.reset();
    vector<T> a = tree.tovector();
    tree = PLTM(a);
  }
};
template <class SizeType, class T, T (*op)(T, T), T (*e)(),
    class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()>
typename PersistentLazyWBTree<SizeType, T, op, e, F, mapping, composition, id>::MemoeyAllocator PersistentLazyWBTree<SizeType, T, op, e, F, mapping, composition, id>::ma;