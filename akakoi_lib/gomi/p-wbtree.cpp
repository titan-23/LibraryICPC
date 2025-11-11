template <class U, class T, T (*op)(T, T), T (*e)(), class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()>
class PersistentLazyWBTree {
public:
  struct MemoeyAllocator {
    // (memo) #pragma pack(push, 1) #pragma pack(pop)
    struct Node {
      U left, right, size;
      Node() : left(0), right(0), size(0) {}
      Node(U l, U r, U s) : left(l), right(r), size(s) {}
    };
    struct Data {
      T key, data; F lazy; int8_t rev;
      Data() : rev(0) {}
      Data(T k, T d, F l, int8_t r) : key(k), data(d), lazy(l), rev(r) {}
    };
    vector<Node> ch;
    vector<Data> dat;
    int ptr;
    MemoeyAllocator() : ptr(1) {
      ch.emplace_back(0, 0, 0);
      dat.emplace_back(T{}, T{}, F{}, 0);
    }
    U copy(U node) {
      U idx = new_node(dat[node].key, dat[node].lazy);
      ch[idx] = {ch[node].left, ch[node].right, ch[node].size};
      dat[idx].data = dat[node].data;
      dat[idx].rev = dat[node].rev;
      return idx;
    }
    U new_node(T key, F f) {
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
    void reserve(U n) { ch.reserve(n); dat.reserve(n); }
    void reset() { ptr = 1; }
  };
  static MemoeyAllocator M;
private:
  using PLTM = PersistentLazyWBTree<U, T, F, op, mapping, composition, e, id>;
  static constexpr int DELTA = 3, GAMMA = 2;
  U root;
  inline U weight_right(U node) const { return M.ch[M.ch[node].right].size + 1; }
  inline U weight_left(U node) const { return M.ch[M.ch[node].left].size + 1; }
  inline U weight(U node) const { return M.ch[node].size + 1; }
  void update(U node) {
    M.ch[node].size = 1 + M.ch[M.ch[node].left].size + M.ch[M.ch[node].right].size;
    M.dat[node].data = M.dat[node].key;
    if (M.ch[node].left) M.dat[node].data = op(M.dat[M.ch[node].left].data, M.dat[node].key);
    if (M.ch[node].right) M.dat[node].data = op(M.dat[node].data, M.dat[M.ch[node].right].data);
  }
  void push(U node, F f) {
    M.dat[node].key = mapping(f, M.dat[node].key);
    M.dat[node].data = mapping(f, M.dat[node].data);
    M.dat[node].lazy = composition(f, M.dat[node].lazy);
  }
  void propagate(U node) {
    if (M.dat[node].rev) {
      U l = M.ch[node].left ? M.copy(M.ch[node].left) : 0;
      U r = M.ch[node].right ? M.copy(M.ch[node].right) : 0;
      M.ch[node].left = r;
      M.ch[node].right = l;
      if (M.ch[node].left) M.dat[M.ch[node].left].rev ^= 1;
      if (M.ch[node].right) M.dat[M.ch[node].right].rev ^= 1;
      M.dat[node].rev = 0;
    }
    if (M.dat[node].lazy != id()) {
      if (M.ch[node].left) {
        M.ch[node].left = M.copy(M.ch[node].left);
        push(M.ch[node].left, M.dat[node].lazy);
      }
      if (M.ch[node].right) {
        M.ch[node].right = M.copy(M.ch[node].right);
        push(M.ch[node].right, M.dat[node].lazy);
      }
      M.dat[node].lazy = id();
    }
  }
  void balance_check(U node) const {
    if (!(weight_left(node)*DELTA >= weight_right(node))) assert(false);
    if (!(weight_right(node) * DELTA >= weight_left(node))) assert(false);
  }
  void _build(vector<T> const &a) {
    auto dfs = [&] (auto &&dfs, U l, U r) -> U {
      U mid = (l + r) >> 1;
      U node = M.new_node(a[mid], id());
      if (l != mid) M.ch[node].left = dfs(dfs, l, mid);
      if (mid+1 != r) M.ch[node].right = dfs(dfs, mid+1, r);
      update(node);
      return node;
    };
    if (a.empty()) { root = 0; return; }
    root = dfs(dfs, 0, (U)a.size());
  }
  U _rotate_right(U node) {
    U u = M.copy(M.ch[node].left);
    M.ch[node].left = M.ch[u].right;
    M.ch[u].right = node;
    update(node); update(u);
    return u;
  }
  U _rotate_left(U node) {
    U u = M.copy(M.ch[node].right);
    M.ch[node].right = M.ch[u].left;
    M.ch[u].left = node;
    update(node); update(u);
    return u;
  }
  U _balance_left(U node) {
    propagate(M.ch[node].right);
    U u = M.ch[node].right;
    if (weight_left(M.ch[node].right) >= weight_right(M.ch[node].right) * GAMMA) {
      propagate(M.ch[u].left);
      M.ch[node].right = _rotate_right(u);
    }
    u = _rotate_left(node);
    return u;
  }
  U _balance_right(U node) {
    propagate(M.ch[node].left);
    U u = M.ch[node].left;
    if (weight_right(M.ch[node].left) >= weight_left(M.ch[node].left) * GAMMA) {
      propagate(M.ch[u].right);
      M.ch[node].left = _rotate_left(u);
    }
    u = _rotate_right(node);
    return u;
  }
  U _merge_with_root(U l, U root, U r) {
    if (weight(r) * DELTA < weight(l)) {
      propagate(l);
      l = M.copy(l);
      M.ch[l].right = _merge_with_root(M.ch[l].right, root, r);
      update(l);
      if (weight(M.ch[l].left) * DELTA < weight(M.ch[l].right)) return _balance_left(l);
      return l;
    } else if (weight(l) * DELTA < weight(r)) {
      propagate(r);
      r = M.copy(r);
      M.ch[r].left = _merge_with_root(l, root, M.ch[r].left);
      update(r);
      if (weight(M.ch[r].right) * DELTA < weight(M.ch[r].left)) return _balance_right(r);
      return r;
    }
    root = M.copy(root);
    M.ch[root].left = l;
    M.ch[root].right = r;
    update(root);
    return root;
  }
  pair<U, U> _pop_right(U node) { return _split_node(node, M.ch[node].size-1); }
  U _merge_node(U l, U r) {
    if ((!l) && (!r)) { return 0; }
    if (!l) return M.copy(r);
    if (!r) return M.copy(l);
    l = M.copy(l); r = M.copy(r);
    auto [l_, root_] = _pop_right(l);
    return _merge_with_root(l_, root_, r);
  }
  pair<U, U> _split_node(U node, U k) {
    if (!node) { return {0, 0}; }
    propagate(node);
    U lch = M.ch[node].left, rch = M.ch[node].right;
    U tmp = lch ? k-M.ch[lch].size : k;
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
  PLTM _new(U root) { return PLTM(root); }
  PersistentLazyWBTree(U root) : root(root) {}
 public:
  PersistentLazyWBTree() : root(0) {}
  PersistentLazyWBTree(vector<T> &a) { _build(a); }
  PLTM merge(PLTM other) {
    U root = _merge_node(this->root, other.root);
    return _new(root);
  }
  pair<PLTM, PLTM> split(U k) {
    auto [l, r] = _split_node(this->root, k);
    return {_new(l), _new(r)};
  }
  PLTM apply(U l, U r, F f) {
    if (l == r) return _new(M.copy(root));
    auto dfs = [&] (auto &&dfs, U node, U left, U right) -> U {
      if (right <= l || r <= left) return node;
      propagate(node);
      U nnode = M.copy(node);
      if (l <= left && right < r) { push(nnode, f); return nnode; }
      U lsize = M.ch[M.ch[nnode].left].size;
      if (M.ch[nnode].left) M.ch[nnode].left = dfs(dfs, M.ch[nnode].left, left, left+lsize);
      if (l <= left+lsize && left+lsize < r) M.dat[nnode].key = mapping(f, M.dat[nnode].key);
      if (M.ch[nnode].right) M.ch[nnode].right = dfs(dfs, M.ch[nnode].right, left+lsize+1, right);
      update(nnode);
      return nnode;
    };
    return _new(dfs(dfs, root, 0, len()));
  }
  T prod(U l, U r) {
    if (l == r) return e();
    auto dfs = [&] (auto &&dfs, U node, U left, U right) -> T {
      if (right <= l || r <= left) return e();
      if (l <= left && right < r) return M.dat[node].data;
      propagate(node);
      U lsize = M.ch[M.ch[node].left].size;
      T res = e();
      if (M.ch[node].left) res = dfs(dfs, M.ch[node].left, left, left+lsize);
      if (l <= left+lsize && left+lsize < r) res = op(res, M.dat[node].key);
      if (M.ch[node].right) res = op(res, dfs(dfs, M.ch[node].right, left+lsize+1, right));
      return res;
    };
    return dfs(dfs, root, 0, len());
  }
  PLTM insert(U k, T key) {
    auto [s, t] = _split_node(root, k);
    U new_node = M.new_node(key, id());
    return _new(_merge_with_root(s, new_node, t));
  }
  pair<PLTM, T> pop(U k) {
    auto [s_, t] = _split_node(this->root, k+1);
    auto [s, tmp] = _pop_right(s_);
    T res = M.dat[tmp].key;
    U root = _merge_node(s, t);
    return {_new(root), res};
  }
  PLTM reverse(U l, U r) {
    if (l >= r) return _new(M.copy(root));
    auto [s_, t] = _split_node(root, r);
    auto [u, s] = _split_node(s_, l);
    M.dat[s].rev ^= 1;
    U root = _merge_node(_merge_node(u, s), t);
    return _new(root);
  }
  vector<T> tovector() {
    U node = root;
    stack<U> s;
    vector<T> a;
    a.reserve(len());
    while (!s.empty() || node) {
      if (node) {
        propagate(node);
        s.emplace(node);
        node = M.ch[node].left;
      } else {
        node = s.top(); s.pop();
        a.emplace_back(M.dat[node].key);
        node = M.ch[node].right;
      }
    }
    return a;
  }
  PLTM copy() { return _new(copy(root)); }
  PLTM set(U k, T v) {
    U node = M.copy(root);
    U root = node;
    U pnode = 0;
    int d = 0;
    stack<U> path = {node};
    while (1) {
      propagate(node);
      U t = M.ch[M.ch[node].left].size;
      if (t == k) {
        node = M.copy(node);
        M.dat[node].key = v;
        path.emplace(node);
        if (d) M.ch[pnode].left = node;
        else M.ch[pnode].right = node;
        while (!path.empty()) {
          update(path.top()); path.pop();
        }
        return _new(root);
      }
      pnode = node;
      if (t < k) {
        k -= t + 1;
        d = 0;
        node = M.copy(M.ch[node].right);
      } else {
        d = 1;
        node = M.copy(M.ch[node].left);
      }
      path.emplace_back(node);
      if (d) M.ch[pnode].left = node;
      else M.ch[pnode].right = node;
    }
  }
  T get(U k) {
    U node = root;
    while (1) {
      propagate(node);
      U t = M.ch[M.ch[node].left].size;
      if (t == k) {
        return M.dat[node].key;
      }
      if (t < k) {
        k -= t + 1;
        node = M.ch[node].right;
      } else {
        node = M.ch[node].left;
      }
    }
  }
  U len() const { return M.ch[root].size; }
  static void rebuild(PLTM &tree) {
    PLTM::M.reset();
    vector<T> a = tree.tovector();
    tree = PLTM(a);
  }
};
template <class U, class T, T (*op)(T, T), T (*e)(), class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()>
typename PersistentLazyWBTree<U, T, op, e, F, mapping, composition, id>::MemoeyAllocator PersistentLazyWBTree<U, T, op, e, F, mapping, composition, id>::M;