template <class T, T (*op)(T, T), T (*e)(), class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()>
class EulerTourTree {
private:
  struct Node;
  using NP = Node*;
  int n;
  vector<NP> ptrV;
  unordered_map<ll, NP> ptrE;
  struct Node {
    T key, data; F lazy; NP par, left, right;
    Node() {}
    Node(T key, F lazy) : key(key), data(key), lazy(lazy), par(nullptr), left(nullptr), right(nullptr) {}
  };
  void _init_build(vector<T> &a) {
    ptrV.resize(n);
    rep(i, n) ptrV[i] = new Node(a[i], id());
  }
  NP _popleft(NP v) {
    v = _left_splay(v);
    if (v->right) v->right->par = nullptr;
    return v->right;
  }
  NP _pop(NP v) {
    v = _right_splay(v);
    if (v->left) v->left->par = nullptr;
    return v->left;
  }
  pair<NP, NP> _split_left(NP v) {
    _splay(v);
    NP x = v, y = v->right;
    if (y) y->par = nullptr;
    x->right = nullptr;
    _update(x);
    return make_pair(x, y);
  }
  pair<NP, NP> _split_right(NP v) {
    _splay(v);
    NP x = v->left, y = v;
    if (x) x->par = nullptr;
    y->left = nullptr;
    _update(y);
    return make_pair(x, y);
  }
  void _merge(NP u, NP v) {
    if ((!u) || (!v)) return;
    u = _right_splay(u);
    _splay(v);
    u->right = v; v->par = u;
    _update(u);
  }
  void _rotate(const NP node) {
    const NP pnode = node->par;
    const NP gnode = pnode->par;
    _propagate(pnode); _propagate(node);
    if (gnode) {
      if (gnode->left == pnode) gnode->left = node;
      else if (gnode->right == pnode) gnode->right = node;
    }
    node->par = gnode;
    if (pnode->left == node) {
      pnode->left = node->right;
      if (node->right) node->right->par = pnode;
      node->right = pnode;
    } else {
      pnode->right = node->left;
      if (node->left) node->left->par = pnode;
      node->left = pnode;
    }
    pnode->par = node; _update(pnode); _update(node);
  }
  void _splay(NP node) {
    _propagate(node);
    while (node->par && node->par->par) {
      NP pnode = node->par, gnode = pnode->par;
      _propagate(gnode); _propagate(pnode); _propagate(node);
      if ((node->par->par->left == node->par) == (node->par->left == node)) _rotate(node->par);
      else _rotate(node);
      _rotate(node);
    }
    if (node->par) _rotate(node);
    _propagate(node);
  }
  NP _left_splay(NP node) {
    _splay(node);
    while (node->left) node = node->left;
    _splay(node); return node;
  }
  NP _right_splay(NP node) {
    _splay(node);
    while (node->right) node = node->right;
    _splay(node); return node;
  }
  void _push(NP node, F f) {
    if (!node) return;
    node->key = mapping(f, node->key);
    node->data = mapping(f, node->data);
    node->lazy = composition(f, node->lazy);
  }
  void _propagate(NP node) {
    if ((!node) || node->lazy == id()) return;
    _push(node->left, node->lazy);
    _push(node->right, node->lazy);
    node->lazy = id();
  }
  void _update(NP node) {
    _propagate(node->left); _propagate(node->right);
    node->data = node->key;
    if (node->left)  node->data = op(node->left->data, node->data);
    if (node->right) node->data = op(node->data, node->right->data);
  }
public:
  EulerTourTree(vector<T> a) : n(a.size()) { _init_build(a); }
  void build(vector<vector<int>> &G) {
    vector<int> seen(n, 0); vector<ll> a; vector<NP> pool;
    auto dfs = [&] (auto &&dfs, int v, int p) -> void {
      a.emplace_back((ll)v*n+v);
      for (const int &x: G[v]) if (x != p) {
        a.emplace_back((ll)v*n+x);
        dfs(dfs, x, v);
        a.emplace_back((ll)x*n+v);
      }
    };
    auto rec = [&] (auto &&rec, int l, int r) -> NP {
      int mid = (l + r) >> 1;
      int u = a[mid]/n, v = a[mid]%n;
      NP node;
      if (u == v) { node = ptrV[u]; seen[u] = 1;
      } else { node = new Node(e(), id()); ptrE[a[mid]] = node; }
      if (l != mid) { node->left = rec(rec, l, mid); node->left->par = node; }
      if (mid+1 != r) { node->right = rec(rec, mid+1, r); node->right->par = node; }
      _update(node); return node;
    };
    rep(root, n) if (!seen[root]) {
      a.clear(); dfs(dfs, root, -1); rec(rec, 0, a.size());
    }
  }
  void link(int u, int v) {
    reroot(u); reroot(v);
    NP uv_node = new Node(e(), id()), vu_node = new Node(e(), id());
    ptrE[(ll)u*n+v] = uv_node; ptrE[(ll)v*n+u] = vu_node;
    NP u_node = ptrV[u], v_node = ptrV[v];
    _merge(u_node, uv_node); _merge(uv_node, v_node); _merge(v_node, vu_node);
  }
  void cut(int u, int v) {
    reroot(v); reroot(u);
    NP uv_node = ptrE[(ll)u*n+v], vu_node = ptrE[(ll)v*n+u];
    ptrE.erase((ll)u*n+v); ptrE.erase((ll)v*n+u);
    NP a, c, _;
    tie(a, _) = _split_left(uv_node);
    tie(_, c) = _split_right(vu_node);
    a = _pop(a); c = _popleft(c); _merge(a, c);
  }
  bool merge(int u, int v) {
    if (same(u, v)) return false;
    link(u, v); return true;
  }
  bool split(int u, int v) {
    if (ptrE.find((ll)u*n+v) == ptrE.end() || ptrE.find((ll)v*n+u) == ptrE.end()) return false;
    cut(u, v); return true;
  }
  NP leader(int v) { return _left_splay(ptrV[v]); }
  void reroot(int v) {
    NP node = ptrV[v];
    auto[x, y] = _split_right(node);
    _merge(y, x); _splay(node);
  }
  bool same(int u, int v) {
    NP u_node = ptrV[u], v_node = ptrV[v];
    _splay(u_node); _splay(v_node);
    return (u_node->par != nullptr || u_node == v_node);
  }
  void subtree_apply(int v, int p, F f) {
    NP v_node = ptrV[v]; reroot(v);
    if (p == -1) {_splay(v_node); _push(v_node, f); return; }
    reroot(p); NP a, b, d;
    tie(a, b) = _split_right(ptrE[(ll)p*n+v]);
    tie(b, d) = _split_left(ptrE[(ll)v*n+p]);
    _splay(v_node); _push(v_node, f); _propagate(v_node);
    _merge(a, b); _merge(b, d);
  }
  T subtree_sum(int v, int p) {
    NP v_node = ptrV[v];
    reroot(v);
    if (p == -1) { _splay(v_node); return v_node->data; }
    reroot(p); NP a, b, d;
    tie(a, b) = _split_right(ptrE[(ll)p*n+v]);
    tie(b, d) = _split_left(ptrE[(ll)v*n+p]);
    _splay(v_node); T res = v_node->data;
    _merge(a, b); _merge(b, d); return res;
  }
  T get_vertex(int v) {
    NP node = ptrV[v]; _splay(node); return node->key;
  }
  void set_vertex(int v, T val) {
    NP node = ptrV[v]; _splay(node); node->key = val; _update(node);
  }
};