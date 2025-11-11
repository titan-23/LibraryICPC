Random trnd;
template <class T, T (*op)(T, T), T (*e)(), class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()>
struct LazyRBST {
  struct Node;
  using NP = Node*;
  using RBST = LazyRBST<T, op, e, F, mapping, composition, id>;
  stack<NP> pathL, pathR; NP root;
  struct Node {
    NP left, right; T key, data; F lazy;
    bool rev; int size;
    Node(T key, F lazy) : left(nullptr), right(nullptr), key(key), data(key), lazy(lazy), rev(false), size(1) {}
    void update() {
      size = 1; data = key;
      if (left) { size += left->size; data = op(left->data, data); }
      if (right) { size += right->size; data = op(data, right->data); }
    }
    void push(F f) {
      key = mapping(f, key); data = mapping(f, data);
      if (left || right) lazy = composition(f, lazy);
    }
    void propagate() {
      if (rev) { swap(left, right);
        if (left) left->rev ^= 1;
        if (right) right->rev ^= 1;
        rev = 0;
      }
      if (left) left->push(lazy);
      if (right) right->push(lazy);
      lazy = id();
    }
  };
  void _build(vector<T> const &a) {
    auto build = [&] (auto &&build, int l, int r) -> NP {
      int mid = (l + r) >> 1;
      NP node = new Node(a[mid], id());
      if (l != mid) node->left = build(build, l, mid);
      if (mid+1 != r) node->right = build(build, mid+1, r);
      node->update(); return node;
    };
    if (a.empty()) { this->root = nullptr; return; }
    this->root = build(build, 0, (int)a.size());
  }
  void _update_lr(NP l, NP r) {
    l->size += r->size; l->data = op(l->data, r->data);
  }
  NP _merge_node(NP l, NP r) {
    NP root = nullptr, r_root = nullptr;
    int d = -1;
    while (l && r) { // 乱数を始めに列挙しておくのも手
      int nd = trnd.randint(0, l->size+r->size-1) < l->size;
      NP node = nd ? l : r;
      node->propagate();
      if (!root) { r_root = node; }
      else if (d == 0) { root->left = node; }
      else { root->right = node; }
      root = node; d = nd;
      if (d) { _update_lr(l, r); l = l->right; }
      else   { _update_lr(r, l); r = r->left; }
    }
    if (!root) return l ? l : r;
    (d ? root->right : root->left) = l ? l : r;
    return r_root;
  }
  pair<NP, NP> _split_node(NP node, int k) {
    while (node) {
      node->propagate();
      int s = node->left ? k-node->left->size : k;
      if (s <= 0) { pathR.emplace(node); node = node->left; }
      else { k = s-1; pathL.emplace(node);  node = node->right; }
    }
    NP l = nullptr, r = nullptr;
    while (!pathL.empty()) {
      NP node = pathL.top(); pathL.pop();
      node->right = l; l = node; l->update();
    }
    while (!pathR.empty()) {
      NP node = pathR.top(); pathR.pop();
      node->left = r; r = node; r->update();
    }
    return {l, r};
  }
  LazyRBST(NP node) : root(node) {}
public:
  LazyRBST() : root(nullptr) {}
  LazyRBST(vector<T> a) : root(nullptr) { _build(a); }
  void merge(RBST &other) { root = _merge_node(root, other.root); }
  pair<RBST, RBST> split(int k) { auto [l, r] = _split_node(root, k); return {RBST(l), RBST(r)}; }
  void apply(int l, int r, F f) {
    if (l == r) return;
    auto [a, bc] = split(l); auto [b, c] = bc.split(r - l);
    if (b.root) b.root->push(f);
    root = _merge_node(_merge_node(a.root, b.root), c.root);
  }
  T all_prod() const { return this->root ? this->root->data : e(); }
  T prod(int l, int r) {
    if (l == r) return e();
    auto [a, bc] = split(l); auto [b, c] = bc.split(r - l);
    T res = b.all_prod();
    root = _merge_node(_merge_node(a.root, b.root), c.root);
    return res;
  }
  void insert(int k, T key) {
    auto [s, t] = _split_node(root, k);
    NP node = new Node(key, id());
    root = _merge_node(_merge_node(s, node), t);
  }
  T pop(int k) {
    auto [s, t] = _split_node(root, k + 1);
    auto [l, tmp] = _split_node(s, s->size - 1);
    T res = tmp->key; root = _merge_node(l, t); return res;
  }
  void reverse(int l, int r) {
    if (l == r) return;
    auto [s2, t] = _split_node(root, r);
    auto [u, s] = _split_node(s2, l);
    s->rev ^= 1; s->propagate();
    root = _merge_node(_merge_node(u, s), t);
  }
  T get(int k) {
    NP node = root;
    while (1) { node->propagate();
      int t = node->left ? node->left->size : 0;
      if (t == k) { return node->key; }
      if (t < k) { k -= t + 1; node = node->right; }
      else { node = node->left; }
    }
  }
  void set(int k, T key) {
    NP node = root;
    while (1) { node->propagate(); pathL.emplace(node);
      int t = node->left ? node->left->size : 0;
      if (t == k) { node->key = key; break; }
      if (t < k) { k -= t + 1; node = node->right; }
      else { node = node->left; }
    }
    while (!pathL.empty()) { pathL.top()->update(); pathL.pop(); }
  }
  int len() const { return root ? root->size : 0; }
};