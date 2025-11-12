namespace ptree {
// U, T, op, e, F, mapping, composition, id
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
vector<Node> ch; vector<Data> dat; int ptr;
void init() { // !!! 最初に必ず呼ぶこと !!!
  ptr = 1; ch.emplace_back(0, 0, 0);
  dat.emplace_back(T{}, T{}, F{}, 0);
}
U new_node(T key, F f) {
  if (ch.size() > ptr) {
    ch[ptr] = {0, 0, 1};
    dat[ptr] = {key, key, f, 0};
  } else {
    ch.emplace_back(0, 0, 1);
    dat.emplace_back(key, key, f, 0);
  }
  ptr++; return ptr - 1;
}
U node_copy(U node) {
  U idx = new_node(dat[node].key, dat[node].lazy);
  ch[idx] = {ch[node].left, ch[node].right, ch[node].size};
  dat[idx].data = dat[node].data;
  dat[idx].rev = dat[node].rev;
  return idx;
}
void reserve(U n) { ch.reserve(n); dat.reserve(n); }
void reset() { ptr = 1; }
class PLWT {
private:
  static constexpr int DELTA = 3, GAMMA = 2;
  U root;
  inline U weight_right(U node) const { return ch[ch[node].right].size + 1; }
  inline U weight_left(U node) const { return ch[ch[node].left].size + 1; }
  inline U weight(U node) const { return ch[node].size + 1; }
  void update(U node) {
    ch[node].size = 1 + ch[ch[node].left].size + ch[ch[node].right].size;
    dat[node].data = dat[node].key;
    if (ch[node].left) dat[node].data = op(dat[ch[node].left].data, dat[node].key);
    if (ch[node].right) dat[node].data = op(dat[node].data, dat[ch[node].right].data);
  }
  void push(U node, F f) {
    dat[node].key = mapping(f, dat[node].key);
    dat[node].data = mapping(f, dat[node].data);
    dat[node].lazy = composition(f, dat[node].lazy);
  }
  void propagate(U node) {
    if (dat[node].rev) {
      U l = ch[node].left ? node_copy(ch[node].left) : 0;
      U r = ch[node].right ? node_copy(ch[node].right) : 0;
      ch[node].left = r;
      ch[node].right = l;
      if (ch[node].left) dat[ch[node].left].rev ^= 1;
      if (ch[node].right) dat[ch[node].right].rev ^= 1;
      dat[node].rev = 0;
    }
    if (dat[node].lazy != id()) {
      if (ch[node].left) {
        ch[node].left = node_copy(ch[node].left);
        push(ch[node].left, dat[node].lazy);
      }
      if (ch[node].right) {
        ch[node].right = node_copy(ch[node].right);
        push(ch[node].right, dat[node].lazy);
      }
      dat[node].lazy = id();
    }
  }
  // if (!(weight_left(node)*DELTA>=weight_right(node))) assert(0);
  // if (!(weight_right(node)*DELTA>=weight_left(node))) assert(0);
  void _build(vector<T> const &a) {
    auto dfs = [&] (auto &&dfs, U l, U r) -> U {
      U mid = (l + r) >> 1;
      U node = new_node(a[mid], id());
      if (l != mid) ch[node].left = dfs(dfs, l, mid);
      if (mid+1 != r) ch[node].right = dfs(dfs, mid+1, r);
      update(node); return node;
    };
    if (a.empty()) { root = 0; return; }
    root = dfs(dfs, 0, (U)a.size());
  }
  U _rotate_right(U node) {
    U u = node_copy(ch[node].left);
    ch[node].left = ch[u].right;
    ch[u].right = node;
    update(node); update(u); return u;
  }
  U _rotate_left(U node) {
    U u = node_copy(ch[node].right);
    ch[node].right = ch[u].left;
    ch[u].left = node;
    update(node); update(u); return u;
  }
  U _balance_left(U node) {
    propagate(ch[node].right);
    U u = ch[node].right;
    if (weight_left(ch[node].right) >= weight_right(ch[node].right) * GAMMA) {
      propagate(ch[u].left);
      ch[node].right = _rotate_right(u);
    }
    u = _rotate_left(node); return u;
  }
  U _balance_right(U node) {
    propagate(ch[node].left);
    U u = ch[node].left;
    if (weight_right(ch[node].left) >= weight_left(ch[node].left) * GAMMA) {
      propagate(ch[u].right);
      ch[node].left = _rotate_left(u);
    }
    u = _rotate_right(node); return u;
  }
  U _merge_with_root(U l, U root, U r) {
    if (weight(r) * DELTA < weight(l)) {
      propagate(l); l = node_copy(l);
      ch[l].right = _merge_with_root(ch[l].right, root, r);
      update(l);
      if (weight(ch[l].left) * DELTA < weight(ch[l].right)) return _balance_left(l);
      return l;
    } else if (weight(l) * DELTA < weight(r)) {
      propagate(r); r = node_copy(r);
      ch[r].left = _merge_with_root(l, root, ch[r].left);
      update(r);
      if (weight(ch[r].right) * DELTA < weight(ch[r].left)) return _balance_right(r);
      return r;
    }
    root = node_copy(root);
    ch[root].left = l; ch[root].right = r;
    update(root); return root;
  }
  pair<U, U> _pop_right(U node) { return _split_node(node, ch[node].size-1); }
  U _merge_node(U l, U r) {
    if ((!l) && (!r)) { return 0; }
    if (!l) return node_copy(r);
    if (!r) return node_copy(l);
    l = node_copy(l); r = node_copy(r);
    auto [l_, root_] = _pop_right(l);
    return _merge_with_root(l_, root_, r);
  }
  pair<U, U> _split_node(U node, U k) {
    if (!node) return {0, 0};
    propagate(node);
    U lch = ch[node].left, rch = ch[node].right;
    U tmp = lch ? k-ch[lch].size : k;
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
  PLWT(U root) : root(root) {}
 public:
  PLWT() : root(0) {}
  PLWT(vector<T> &a) { _build(a); }
  PLWT merge(PLWT other) {
    U root = _merge_node(this->root, other.root);
    return PLWT(root);
  }
  pair<PLWT, PLWT> split(U k) {
    auto [l, r] = _split_node(this->root, k);
    return {PLWT(l), PLWT(r)};
  }
  PLWT apply(U l, U r, F f) {
    if (l == r) return PLWT(node_copy(root));
    auto dfs = [&] (auto &&dfs, U node, U left, U right) -> U {
      if (right <= l || r <= left) return node;
      propagate(node);
      U nnode = node_copy(node);
      if (l <= left && right <= r) { push(nnode, f); return nnode; }
      U lsize = ch[ch[nnode].left].size;
      if (ch[nnode].left) ch[nnode].left = dfs(dfs, ch[nnode].left, left, left+lsize);
      if (l <= left+lsize && left+lsize < r) dat[nnode].key = mapping(f, dat[nnode].key);
      if (ch[nnode].right) ch[nnode].right = dfs(dfs, ch[nnode].right, left+lsize+1, right);
      update(nnode);
      return nnode;
    };
    return PLWT(dfs(dfs, root, 0, len()));
  }
  T prod(U l, U r) {
    if (l == r) return e();
    auto dfs = [&] (auto &&dfs, U node, U left, U right) -> T {
      if (right <= l || r <= left) return e();
      if (l <= left && right <= r) return dat[node].data;
      propagate(node);
      U lsize = ch[ch[node].left].size; T res = e();
      if (ch[node].left) res = dfs(dfs, ch[node].left, left, left+lsize);
      if (l <= left+lsize && left+lsize < r) res = op(res, dat[node].key);
      if (ch[node].right) res = op(res, dfs(dfs, ch[node].right, left+lsize+1, right));
      return res;
    };
    return dfs(dfs, root, 0, len());
  }
  PLWT insert(U k, T key) {
    auto [s, t] = _split_node(root, k);
    U x = new_node(key, id());
    return PLWT(_merge_with_root(s, x, t));
  }
  pair<PLWT, T> pop(U k) {
    auto [s_, t] = _split_node(this->root, k+1);
    auto [s, tmp] = _pop_right(s_);
    T res = dat[tmp].key; U root = _merge_node(s, t);
    return {PLWT(root), res};
  }
  PLWT reverse(U l, U r) {
    if (l >= r) return PLWT(node_copy(root));
    auto [s_, t] = _split_node(root, r);
    auto [u, s] = _split_node(s_, l);
    dat[s].rev ^= 1;
    U root = _merge_node(_merge_node(u, s), t);
    return PLWT(root);
  }
  vector<T> tovector() {
    U node = root;
    stack<U> s; vector<T> a; a.reserve(len());
    while (!s.empty() || node) {
      if (node) {
        propagate(node); s.emplace(node);
        node = ch[node].left;
      } else {
        node = s.top(); s.pop();
        a.emplace_back(dat[node].key);
        node = ch[node].right;
      }
    } return a;
  }
  PLWT copy() { return PLWT(node_copy(root)); }
  PLWT set(U k, T v) {
    U new_root = inner_set(root, k, v);
    return PLWT(new_root);
  }
  U inner_set(U node, U k, T v) {
    U nxt = node_copy(node); propagate(nxt);
    U t = ch[ch[nxt].left].size;
    if (t == k) { dat[nxt].key = v; }
    else if (t < k) {
      U new_right = inner_set(ch[nxt].right, k-t-1, v);
      ch[nxt].right = new_right;
    } else {
      U new_left = inner_set(ch[nxt].left, k, v);
      ch[nxt].left = new_left;
    }
    update(nxt); return nxt;
  }
  T get(U k) {
    U node = root;
    while (1) {
      propagate(node); U t = ch[ch[node].left].size;
      if (t == k) { return dat[node].key; }
      if (t < k) { k -= t + 1; node = ch[node].right; }
      else { node = ch[node].left; }
    }
  }
  U len() const { return ch[root].size; }
  static void rebuild(PLWT &tree) {
    reset(); vector<T> a = tree.tovector();
    tree = PLWT(a);
  }
};} // namespace ptree