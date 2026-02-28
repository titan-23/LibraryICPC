#include <iostream>
#include <vector>

using namespace std;
using ull = unsigned long long;

int main() {
    int n, m;
    ull mod = 938234350;
    if (!(cin >> n >> m)) return 0;

    vector<ull> a(n), b(m);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < m; i++) cin >> b[i];

    vector<ull> res(n + m - 1, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // __int128 で中間結果を保持して mod を取る
            unsigned __int128 prod = (unsigned __int128)a[i] * b[j];
            res[i + j] = (ull)(( (unsigned __int128)res[i + j] + prod ) % mod);
        }
    }

    for (int i = 0; i < n + m - 1; i++) {
        cout << res[i] << " ";
    }
    cout << endl;

    return 0;
}