// https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=11044734#2
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/gomi/seg2D_light.cpp"

int op(int s, int t) { return s + t; }
int e() { return 0; }

void solve() {
    int n; cin >> n;
    int M = 1001;
    Seg2DLight<int, op, e> seg(M, M);
    vector<vector<int>> A(M, vector<int>(M, 1));
    seg.build(A);
    rep(i, M) rep(j, M) {
        seg.set(i, j, 0);
    }

    rep(i, n) {
        int a, b, c, d; cin >> a >> b >> c >> d;
        seg.set(a, b, seg.get(a, b) + 1);
        seg.set(a, d, seg.get(a, d) - 1);
        seg.set(c, b, seg.get(c, b) - 1);
        seg.set(c, d, seg.get(c, d) + 1);
    }
    int ans = 0;
    rep(i, M) rep(j, M) {
        chmax(ans, seg.prod(0, i+1, 0, j+1));
    }
    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
