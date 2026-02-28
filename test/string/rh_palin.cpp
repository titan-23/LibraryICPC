// https://judge.yosupo.jp/submission/356428
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/string/rolling_hash.cpp"

// -----------------------

void solve() {
    string S;
    cin >> S;
    int n = S.size();

    string revS = S;
    reverse(revS.begin(), revS.end());

    // 正方向と逆方向のハッシュを構築
    RollingHash rh(S);
    RollingHash rev_rh(revS);

    // 閉区間 S[l..r] が回文かどうかを判定する関数
    auto is_pal = [&](int l, int r) {
        int rev_l = n - 1 - r;
        int rev_r = n - 1 - l;
        // get(l, r) は半開区間 [l, r) を要求するため +1 する
        return rh.get(l, r + 1) == rev_rh.get(rev_l, rev_r + 1);
    };

    // 中心位置 c は 0 から 2N-2 まで
    for (int c = 0; c < 2 * n - 1; ++c) {
        if (c % 2 == 0) {
            // 中心が文字 S[i] の上にある場合 (奇数長回文)
            int i = c / 2;
            int ok = 0, ng = min(i, n - 1 - i) + 1;
            while (ng - ok > 1) {
                int mid = (ok + ng) / 2;
                if (is_pal(i - mid, i + mid)) ok = mid;
                else ng = mid;
            }
            cout << 2 * ok + 1 << (c == 2 * n - 2 ? "" : " ");
        } else {
            // 中心が文字 S[i] と S[i+1] の間にある場合 (偶数長回文)
            int i = c / 2;
            int ok = 0, ng = min(i + 1, n - 1 - i) + 1;
            while (ng - ok > 1) {
                int mid = (ok + ng) / 2;
                if (is_pal(i - mid + 1, i + mid)) ok = mid;
                else ng = mid;
            }
            cout << 2 * ok << (c == 2 * n - 2 ? "" : " ");
        }
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    solve();

    return 0;
}