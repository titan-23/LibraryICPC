// https://judge.yosupo.jp/submission/356431
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/string/rolling_hash.cpp"

// -----------------------

void solve() {
    string S;
    cin >> S;
    int N = S.size();

    RollingHash rh(S);

    // 0 から N-1 までのインデックス配列を用意
    vector<int> sa(N);
    iota(sa.begin(), sa.end(), 0);

    // 2つの接尾辞 S[i..] と S[j..] の辞書順比較関数
    auto cmp = [&](int i, int j) {
        if (i == j) return false;

        // 二分探索で最長共通接頭辞 (LCP) の長さを求める
        int ok = 0, ng = min(N - i, N - j) + 1;
        while (ng - ok > 1) {
            int mid = (ok + ng) / 2;
            if (rh.get(i, i + mid) == rh.get(j, j + mid)) {
                ok = mid;
            } else {
                ng = mid;
            }
        }

        int lcp = ok;

        // 片方がもう片方の接頭辞に完全に包含されている場合
        if (i + lcp == N) return true;
        if (j + lcp == N) return false;

        // LCP の次の文字で辞書順を比較
        return S[i + lcp] < S[j + lcp];
    };

    // 比較関数を用いてソート (全体で O(N log^2 N))
    sort(sa.begin(), sa.end(), cmp);

    for (int i = 0; i < N; ++i) {
        cout << sa[i] << (i == N - 1 ? "" : " ");
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