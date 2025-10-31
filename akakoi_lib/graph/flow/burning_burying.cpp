struct BurningBurying {
    int n, start, goal;
    ll offset = 0;
    Dinic dn;
    BurningBurying(int n) {
        this->n = n;
        start = n;
        goal = n + 1;
        dn = Dinic(n + 2);
    }
    // 0,1
    void add_single(int i, ll zero, ll one) {
        if (zero <= one) {
            // 基本コストがで、をからに変えると zeroi01+one-される zero
            offset += zero;
            dn.add(start, i, one - zero);
        } else {
            // 基本コストがで、をからに変えると onei10-one+される zero
            offset += one;
            dn.add(i, goal, zero - one);
            }
        }
        //(0,0),(0,1),(1,0),(1,1)
    void add_double(int i, int j, ll a, ll b, ll c, ll d) {
        assert(b + c >= a + d); // 大事！！！
        offset += a;
        add_single(i, 0, c - a);
        add_single(j, 0, d - c);
        dn.add(i, j, b + c - a - d);
    }
    ll solve() { return dn.max_flow(start, goal) + offset; }
};