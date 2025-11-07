template <class T, T (*op)(T, T), T (*e)(),
    class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()>
class EulerTourTree {
private:
  struct Node;
  using NodePtr = Node*;
  int n, group_numbers;
  vector<NodePtr> ptr_vertex;
  unordered_map<long long, NodePtr> ptr_edge;
  struct Node {
    T key, data;
    F lazy;
    NodePtr par, left, right;
    Node() {}
    Node(T key, F lazy) :
      key(key), data(key), lazy(lazy),
      par(nullptr), left(nullptr), right(nullptr) {}
  };
  void _init_build(vector<T> &a) {
    ptr_vertex.resize(n);
    for (int i = 0; i < n; ++i) {
      ptr_vertex[i] = new Node(a[i], id());
    }
  }
  NodePtr _popleft(NodePtr v) {
    v = _left_splay(v);
    if (v->right) v->right->par = nullptr;
    return v->right;
  }
  NodePtr _pop(NodePtr v) {
    v = _right_splay(v);
    if (v->left) v->left->par = nullptr;
    return v->left;
  }
  pair<NodePtr, NodePtr> _split_left(NodePtr v) {
    _splay(v);
    NodePtr x = v, y = v->right;
    if (y) y->par = nullptr;
    x->right = nullptr;
    _update(x);
    return make_pair(x, y);
  }
  pair<NodePtr, NodePtr> _split_right(NodePtr v) {
    _splay(v);
    NodePtr x = v->left, y = v;
    if (x) x->par = nullptr;
    y->left = nullptr;
    _update(y);
    return make_pair(x, y);
  }
  void _merge(NodePtr u, NodePtr v) {
    if ((!u) || (!v)) return;
    u = _right_splay(u);
    _splay(v);
    u->right = v;
    v->par = u;
    _update(u);
  }
  void _rotate(const NodePtr node) {
    const NodePtr pnode = node->par;
    const NodePtr gnode = pnode->par;
    _propagate(pnode);
    _propagate(node);
    if (gnode) {
      if (gnode->left == pnode) {
        gnode->left = node;
      } else if (gnode->right == pnode) {
        gnode->right = node;
      }
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
    pnode->par = node;
    _update(pnode);
    _update(node);
  }
  void _splay(NodePtr node) {
    _propagate(node);
    while (node->par && node->par->par) {
      NodePtr pnode = node->par, gnode = pnode->par;
      _propagate(gnode);
      _propagate(pnode);
      _propagate(node);
      if ((node->par->par->left == node->par) == (node->par->left == node)) {
        _rotate(node->par);
      } else {
        _rotate(node);
      }
      _rotate(node);
    }
    if (node->par) _rotate(node);
    _propagate(node);
  }
  NodePtr _left_splay(NodePtr node) {
    _splay(node);
    while (node->left) node = node->left;
    _splay(node);
    return node;
  }
  NodePtr _right_splay(NodePtr node) {
    _splay(node);
    while (node->right) node = node->right;
    _splay(node);
    return node;
  }
  void _push(NodePtr node, F f) {
    if (!node) return;
    node->key = mapping(f, node->key);
    node->data = mapping(f, node->data);
    node->lazy = composition(f, node->lazy);
  }
  void _propagate(NodePtr node) {
    if ((!node) || node->lazy == id()) return;
    _push(node->left, node->lazy);
    _push(node->right, node->lazy);
    node->lazy = id();
  }
  void _update(NodePtr node) {
    _propagate(node->left);
    _propagate(node->right);
    node->data = node->key;
    if (node->left)  node->data = op(node->left->data, node->data);
    if (node->right) node->data = op(node->data, node->right->data);
  }
public:
  EulerTourTree(int n) : n(n), group_numbers(n) {
    vector<T> a(n, e());
    _init_build(a);
  }
  EulerTourTree(vector<T> a) : n((int)a.size()), group_numbers((int)a.size()) {
    _init_build(a);
  }
  void build(vector<vector<int>> &G) {
    vector<int> seen(n, 0);
    vector<long long> a;
    vector<NodePtr> pool;
    auto dfs = [&] (auto &&dfs, int v, int p) -> void {
      a.emplace_back((long long)v*n+v);
      for (const int &x: G[v]) {
        if (x == p) continue;
        a.emplace_back((long long)v*n+x);
        dfs(dfs, x, v);
        a.emplace_back((long long)x*n+v);
      }
    };
    auto rec = [&] (auto &&rec, int l, int r) -> NodePtr {
      int mid = (l + r) >> 1;
      int u = a[mid]/n, v = a[mid]%n;
      NodePtr node;
      if (u == v) {
        node = ptr_vertex[u];
        seen[u] = 1;
      } else {
        node = new Node(e(), id());
        ptr_edge[a[mid]] = node;
      }
      if (l != mid) {
        node->left = rec(rec, l, mid);
        node->left->par = node;
      }
      if (mid+1 != r) {
        node->right = rec(rec, mid+1, r);
        node->right->par = node;
      }
      _update(node);
      return node;
    };
    for (int root = 0; root < n; ++root) {
      if (seen[root]) continue;
      a.clear();
      dfs(dfs, root, -1);
      rec(rec, 0, (int)a.size());
    }
  }
  void link(int u, int v) {
    reroot(u);
    reroot(v);
    NodePtr uv_node = new Node(e(), id());
    NodePtr vu_node = new Node(e(), id());
    ptr_edge[(long long)u*n+v] = uv_node;
    ptr_edge[(long long)v*n+u] = vu_node;
    NodePtr u_node = ptr_vertex[u];
    NodePtr v_node = ptr_vertex[v];
    _merge(u_node, uv_node);
    _merge(uv_node, v_node);
    _merge(v_node, vu_node);
    --group_numbers;
  }
  void cut(int u, int v) {
    reroot(v);
    reroot(u);
    NodePtr uv_node = ptr_edge[(long long)u*n+v];
    NodePtr vu_node = ptr_edge[(long long)v*n+u];
    ptr_edge.erase((long long)u*n+v);
    ptr_edge.erase((long long)v*n+u);
    NodePtr a, c, _;
    tie(a, _) = _split_left(uv_node);
    tie(_, c) = _split_right(vu_node);
    a = _pop(a);
    c = _popleft(c);
    _merge(a, c);
    ++group_numbers;
  }
  bool merge(int u, int v) {
    if (same(u, v)) return false;
    link(u, v);
    return true;
  }
  bool split(int u, int v) {
    if (ptr_edge.find((long long)u*n+v) == ptr_edge.end() || ptr_edge.find((long long)v*n+u) == ptr_edge.end()) {
      return false;
    }
    cut(u, v);
    return true;
  }
  NodePtr leader(int v) { return _left_splay(ptr_vertex[v]); }
  void reroot(int v) {
    NodePtr node = ptr_vertex[v];
    auto[x, y] = _split_right(node);
    _merge(y, x);
    _splay(node);
  }
  bool same(int u, int v) {
    NodePtr u_node = ptr_vertex[u];
    NodePtr v_node = ptr_vertex[v];
    _splay(u_node);
    _splay(v_node);
    return (u_node->par != nullptr || u_node == v_node);
  }
  void subtree_apply(int v, int p, F f) {
    NodePtr v_node = ptr_vertex[v];
    reroot(v);
    if (p == -1) {
      _splay(v_node);
      v_node->key = mapping(f, v_node->key);
      v_node->data = mapping(f, v_node->data);
      v_node->lazy = composition(f, v_node->lazy);
      return;
    }
    reroot(p);
    NodePtr a, b, d;
    tie(a, b) = _split_right(ptr_edge[(long long)p*n+v]);
    tie(b, d) = _split_left(ptr_edge[(long long)v*n+p]);
    _splay(v_node);
    v_node->key = mapping(f, v_node->key);
    v_node->data = mapping(f, v_node->data);
    v_node->lazy = composition(f, v_node->lazy);
    _propagate(v_node);
    _merge(a, b);
    _merge(b, d);
  }
  T subtree_sum(int v, int p) {
    NodePtr v_node = ptr_vertex[v];
    reroot(v);
    if (p == -1) {
      _splay(v_node);
      return v_node->data;
    }
    reroot(p);
    NodePtr a, b, d;
    tie(a, b) = _split_right(ptr_edge[(long long)p*n+v]);
    tie(b, d) = _split_left(ptr_edge[(long long)v*n+p]);
    _splay(v_node);
    T res = v_node->data;
    _merge(a, b);
    _merge(b, d);
    return res;
  }
  int group_count() { return group_numbers; }
  T get_vertex(int v) {
    NodePtr node = ptr_vertex[v];
    _splay(node);
    return node->key;
  }
  void set_vertex(int v, T val) {
    NodePtr node = ptr_vertex[v];
    _splay(node);
    node->key = val;
    _update(node);
  }
};