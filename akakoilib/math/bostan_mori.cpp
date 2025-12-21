/// @brief [x^n] P(x)/Q(x) を求める / O(d log d log n)
mint bostan_mori(const vector<mint> &P, const vector<mint> &Q, ll n) {
  if (P.empty()) return 0;
  if (n == 0) return P[0] / Q[0];
  vector<mint> Qm(Q.size());
  rep(i, Q.size()) Qm[i] = (i&1) ? -Q[i] : Q[i];
  vector<mint> V = Convolve(Q, Qm);
  vector<mint> U = Convolve(P, Qm);
  vector<mint> nP, nQ;
  for(int i = n%2; i < U.size(); i += 2) nP.push_back(U[i]);
  for(int i = 0; i < V.size(); i += 2) nQ.push_back(V[i]);
  return bostan_mori(nP, nQ, n/2);
}