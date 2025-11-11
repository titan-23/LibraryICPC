## 約数・倍数変換に関する公式

- $\phi(i) = i$ 以下であって、 $i$ と互いに素な数の個数
- $\phi(n) = n \prod_{p} {(1-\frac{1}{p})}$ ($p$ は $n$ の素因数)
- $\phi(n) \phi(m) = \phi(nm)$
- $\sum_{d\mid n}{\phi(d)} = n$
  - $(1, 2, \cdots, n)$ を約数メビウス変換することで $(\phi(1), \phi(2), \cdots, \phi(n))$ を得る
- $a^{\phi(m)}\equiv 1 \bmod{m}$ ($\gcd(a,m) = 1$)
- $(1, 0, \cdots, 0)$ を約数メビウス変換することで $(\mu(1), \mu(2), \cdots, \mu(n))$ を得る ($\zeta * \mu = I$)