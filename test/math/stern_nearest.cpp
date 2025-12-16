#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/math/stern_brocot_tree.cpp"

// -----------------------

#include <iostream>
#include <vector>
using namespace std;

namespace titan23 {

template<typename T>
class Fraction {
public:
    T p, q;

    Fraction(T p = 0, T q = 1) : p(p), q(q) {
        if (q == 0) {
            if (p > 0) { p = 1; q = 0; }
            else if (p < 0) { p = -1; q = 0; }
            else { p = 0; q = 0; }
        } else {
            if (q < 0) { p = -p; q = -q; }
            simplify();
        }
    }

    constexpr T internal_gcd(T a, T b) {
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (b != 0) {
            T temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    void simplify() {
        if (p == 0) {
            q = 1;
        } else {
            T g = internal_gcd(p, q);
            p /= g;
            q /= g;
        }
    }

    Fraction operator-() const { return Fraction(-p, q); }
    Fraction abs() const { return Fraction(p < 0 ? -p : p, q); }
    Fraction operator+(const Fraction& other) const { return Fraction(p * other.q + other.p * q, q * other.q); }
    Fraction operator-(const Fraction& other) const { return Fraction(p * other.q - other.p * q, q * other.q); }
    Fraction operator*(const Fraction& other) const { return Fraction(p * other.p, q * other.q); }
    Fraction operator/(const Fraction& other) const { return Fraction(p * other.q, q * other.p); }
    Fraction& operator+=(const Fraction& other) { *this = *this + other; return *this; }
    Fraction& operator-=(const Fraction& other) { *this = *this - other; return *this; }
    Fraction& operator*=(const Fraction& other) { *this = *this * other; return *this; }
    Fraction& operator/=(const Fraction& other) { *this = *this / other; return *this; }
    bool operator<(const Fraction& other) const { return p * other.q < other.p * q; }
    bool operator>(const Fraction& other) const { return other < *this; }
    bool operator<=(const Fraction& other) const { return !(*this > other); }
    bool operator>=(const Fraction& other) const { return !(*this < other); }
    bool operator==(const Fraction& other) const { return p == other.p && q == other.q; }
    bool operator!=(const Fraction& other) const { return !(*this == other); }
    explicit operator double() const { return (double)p / q; }
    explicit operator long double() const { return (long double)p / q; }
    friend ostream& operator<<(ostream& os, const Fraction& f) {
        os << f.p << "/" << f.q;
        return os;
    }
};
} // namespace titan23

#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;
using T = mp::cpp_int;
// using T = __int128_t;

void solve() {
    string r; cin >> r;
    ll n; cin >> n;
    T p = 0, q = 1;
    for (int i = 2; i < r.size(); ++i) {
        p *= 10;
        p += r[i]-'0';
        q *= 10;
    }
    SternBrocotTree<T> SBT;
    using Node = SternBrocotTree<T>::Node;

    Node x = SBT.get_node(1, 7);

    titan23::Fraction<T> RF(p, q);
    titan23::Fraction<T> minF(1, 1);
    auto g = [&] (titan23::Fraction<T> F) {
        if ((F-RF).abs() < (minF-RF).abs()) {
        minF = F;
        } else if ((F-RF).abs() == (minF-RF).abs() && F < minF) {
            minF = F;
        }
    };
    if (false) {
        auto f = [&] (T num, T den) -> bool {
            titan23::Fraction<T> P(num, den);
            return P <= RF;
        };
        Node ans = SBT.binary_search(f, n);
        auto [k1, k2] = ans.range();
        titan23::Fraction<T> K1(k1.first, k1.second);
        titan23::Fraction<T> K2(k2.first, k2.second);
        g(K1);
        g(K2);
    }
    if (true) {
        auto f = [&] (T num, T den) -> bool {
            titan23::Fraction<T> P(num, den);
            return P >= RF;
        };
        Node ans = SBT.binary_search(f, n);
        auto [k1, k2] = ans.range();
        titan23::Fraction<T> K1(k1.first, k1.second);
        titan23::Fraction<T> K2(k2.first, k2.second);
        g(K1);
        g(K2);
    }
    cout << minF.p << " " << minF.q << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    // int t; cin >> t;
    int t = 1;
    rep(tdx, t) {
        solve();
    }

    return 0;
}
