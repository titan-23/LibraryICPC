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
  inline U weight_right(U node) { return ch[ch[node].right].size + 1; }
  inline U weight_left(U node) { return ch[ch[node].left].size + 1; }
  inline U weight(U node) { return ch[node].size + 1; }
  void update(U v) {
    ch[v].size = 1 + ch[ch[v].left].size + ch[ch[v].right].size;
    dat[v].data = dat[v].key;
    if (ch[v].left) dat[v].data = op(dat[ch[v].left].data, dat[v].key);
    if (ch[v].right) dat[v].data = op(dat[v].data, dat[ch[v].right].data);
  }
  void push(U v, F f) {
    dat[v].key = mapping(f, dat[v].key);
    dat[v].data = mapping(f, dat[v].data);
    dat[v].lazy = composition(f, dat[v].lazy);
  }
  void propagate(U v) {
    if (dat[v].rev) {
      U l = ch[v].left ? node_copy(ch[v].left) : 0;
      U r = ch[v].right ? node_copy(ch[v].right) : 0;
      ch[v].left = r; ch[v].right = l;
      if (ch[v].left) dat[ch[v].left].rev ^= 1;
      if (ch[v].right) dat[ch[v].right].rev ^= 1;
      dat[v].rev = 0;
    }
    if (dat[v].lazy != id()) {
      if (ch[v].left) {
        ch[v].left = node_copy(ch[v].left);
        push(ch[v].left, dat[v].lazy);
      }
      if (ch[v].right) {
        ch[v].right = node_copy(ch[v].right);
        push(ch[v].right, dat[v].lazy);
      }
      dat[v].lazy = id();
    }
  }
  // assert(weight_left(node)*DELTA>=weight_right(node));
  // assert(weight_right(node)*DELTA>=weight_left(node));
  void _build(vector<T> const &a) {
    auto dfs = [&] (auto &&dfs, U l, U r) -> U {
      U mid = (l + r) >> 1;
      U v = new_node(a[mid], id());
      if (l != mid) ch[v].left = dfs(dfs, l, mid);
      if (mid+1 != r) ch[v].right = dfs(dfs, mid+1, r);
      update(v); return v;
    };
    if (a.empty()) { root = 0; return; }
    root = dfs(dfs, 0, (U)a.size());
  }
  U _rotate_right(U x) {
    U u = x_copy(ch[x].left);
    ch[x].left = ch[u].right; ch[u].right = x;
    update(x); update(u); return u;
  }
  U _rotate_left(U x) {
    U u = x_copy(ch[x].right);
    ch[x].right = ch[u].left; ch[u].left = x;
    update(x); update(u); return u;
  }
  U _balance_left(U x) {
    propagate(ch[x].right); U u = ch[x].right;
    if (weight_left(ch[x].right) >= weight_right(ch[x].right) * GAMMA) {
      propagate(ch[u].left);
      ch[x].right = _rotate_right(u);
    }
    u = _rotate_left(x); return u;
  }
  U _balance_right(U x) {
    propagate(ch[x].left); U u = ch[x].left;
    if (weight_right(ch[x].left) >= weight_left(ch[x].left) * GAMMA) {
      propagate(ch[u].right);
      ch[x].left = _rotate_left(u);
    }
    u = _rotate_right(x); return u;
  }
  U _merge_with_root(U l, U root, U r) {
    if (weight(r) * DELTA < weight(l)) {
      propagate(l); l = node_copy(l);
      ch[l].right = _merge_with_root(ch[l].right, root, r);
      update(l);
      return weight(ch[l].left)*DELTA < weight(ch[l].right) ? _balance_left(l) : l;
    } else if (weight(l) * DELTA < weight(r)) {
      propagate(r); r = node_copy(r);
      ch[r].left = _merge_with_root(l, root, ch[r].left);
      update(r);
      return weight(ch[r].right)*DELTA < weight(ch[r].left) ? _balance_right(r) : r;
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
  pair<U, U> _split_node(U x, U k) {
    if (!x) return {0, 0};
    propagate(x);
    U lch = ch[x].left, rch = ch[x].right;
    U tmp = lch ? k-ch[lch].size : k;
    if (tmp == 0) {
      return {lch, _merge_with_root(0, x, rch)};
    } else if (tmp < 0) {
      auto [l, r] = _split_node(lch, k);
      return {l, _merge_with_root(r, x, rch)};
    } else {
      auto [l, r] = _split_node(rch, tmp-1);
      return {_merge_with_root(lch, x, l), r};
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
    auto dfs = [&] (auto &&dfs, U x, U left, U right) -> U {
      if (right <= l || r <= left) return x;
      propagate(x); U nx = node_copy(x);
      if (l <= left && right <= r) { push(nx, f); return nx; }
      U lsize = ch[ch[nx].left].size;
      if (ch[nx].left) ch[nx].left = dfs(dfs, ch[nx].left, left, left+lsize);
      if (l <= left+lsize && left+lsize < r) dat[nx].key = mapping(f, dat[nx].key);
      if (ch[nx].right) ch[nx].right = dfs(dfs, ch[nx].right, left+lsize+1, right);
      update(nx); return nx;
    }; return PLWT(dfs(dfs, root, 0, len()));
  }
  T prod(U l, U r) { if (l == r) return e();
    auto dfs = [&] (auto &&dfs, U x, U left, U right) -> T {
      if (right <= l || r <= left) return e();
      if (l <= left && right <= r) return dat[x].data;
      propagate(x);
      U lsize = ch[ch[x].left].size; T res = e();
      if (ch[x].left) res = dfs(dfs, ch[x].left, left, left+lsize);
      if (l <= left+lsize && left+lsize < r) res = op(res, dat[x].key);
      if (ch[x].right) res = op(res, dfs(dfs, ch[x].right, left+lsize+1, right));
      return res;
    }; return dfs(dfs, root, 0, len());
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
    U v = root;
    while (1) {
      propagate(v); U t = ch[ch[v].left].size;
      if (t == k) return dat[v].key;
      if (t < k) { k -= t + 1; v = ch[v].right; }
      else { v = ch[v].left; }
    }
  }
  U len() const { return ch[root].size; }
  static void rebuild(PLWT &tree) {
    reset(); vector<T> a = tree.tovector();
    tree = PLWT(a);
  }
};} // namespace ptree