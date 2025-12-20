struct BurningBurying {
  int n, start, goal;
  ll offset = 0;
  Dinic dn;
  BurningBurying(int n) : n(n), start(n), goal(n+1), dn(n+2) {}
  // 1変数コスト
  // iを0にしたときのコスト=zero / 1にしたとき=one
  void add_single(int i, ll zero, ll one) {
    if (zero <= one) {
      // 基本コストがzeroで、iを0から1に変えると+(one-zero)される
      offset += zero;
      dn.add(start, i, one - zero);
    } else {
      // 基本コストがoneで、iを1から0に変えると+(zero-one)される
      offset += one;
      dn.add(i, goal, zero - one);
    }
  }
  // 2変数コスト
  // (i,j)=(0,0),(0,1),(1,0),(1,1)のコストがそれぞれa,b,c,d
  void add_double(int i, int j, ll a, ll b, ll c, ll d) {
    assert(b + c >= a + d); // 大事！！！
    offset += a;
    add_single(i, 0, c - a);
    add_single(j, 0, d - c);
    dn.add(i, j, b + c - a - d);
  }
  ll solve() { return dn.max_flow(start, goal) + offset; }
};