#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;
using ll = long long;

struct Random {
    mt19937_64 rng;
    Random() {
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
    }
    ll randint(ll l, ll r) {
        return uniform_int_distribution<ll>(l, r)(rng);
    }
};

int main() {
    Random rnd;
    // O(NM)のCorrectと合わせるため、小さめのN, Mに設定
    int n = rnd.randint(1, 1000);
    int m = rnd.randint(1, 1000);
    const int mod = 938234350;
    cout << n << " " << m << endl;
    for (int i = 0; i < n; i++) cout << rnd.randint(0, mod - 1) << (i == n - 1 ? "" : " ");
    cout << endl;
    for (int i = 0; i < m; i++) cout << rnd.randint(0, mod - 1) << (i == m - 1 ? "" : " ");
    cout << endl;

    return 0;
}