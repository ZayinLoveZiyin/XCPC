#include <bits/stdc++.h>
#define modu 998244353
using namespace std;
typedef long long LL;

namespace polynomial {

void add(LL& a, LL b) { a = (a + b) % modu; }
void sub(LL& a, LL b) { a = (a - b % modu + modu) % modu; }
void mul(LL& a, LL b) { a = a * b % modu; }
LL sgn(LL a) { return (a & 1) ? modu - 1 : 1; }

LL pw(LL a, LL k = modu - 2) {
  LL ans = 1;
  for (; k; k >>= 1) {
    if (k & 1) ans = ans * a % modu;
    a = a * a % modu;
  }
  return ans;
}

struct poly : vector<LL> {
  using vector::vector;
  poly& normalize() {
    while (size() && !back()) pop_back();
    return *this;
  }

  int deg() const { return size() - 1; }
  void init(int n) {
    resize(n);
    for (auto& x : *this) cin >> x;
    normalize();
  }

  poly operator-() const {
    poly ans = *this;
    for (auto& x : ans) x = (modu - x) % modu;
    return ans;
  }

  LL operator()(const LL& x) const {
    LL ans = 0;
    for (int i = size() - 1; i >= 0; --i) ans = (ans * x + at(i)) % modu;
    return ans;
  }
};

poly operator"" _x(unsigned long long a) { return poly{0, ((LL)a % modu + modu) % modu}; }

ostream& operator<<(ostream& os, poly a) {
  os << "(";
  for (auto x : a) os << x << " ";
  os << ")";
  return os;
}
istream& operator>>(istream& is, poly& a) {
  int n;
  is >> n;
  a.resize(n);
  for (auto& x : a) is >> x;
  return is;
}

namespace FFT {
#define maxn 1000050
int N, rev[maxn];
LL I, w[maxn];
inline void init(int n) {
  for (N = 1; N < n; N <<= 1)
    ;
  I = pw(N);
  assert(N < maxn);

  rev[0] = 0, rev[1] = N >> 1;
  for (int i = 2; i < N; ++i) rev[i] = rev[i >> 1] >> 1 | rev[i & 1];

  w[0] = 1, w[1] = pw(3, (modu - 1) / N);
  for (int i = 2; i < N; ++i) w[i] = w[i - 1] * w[1] % modu;
}
inline void DFT(LL* A) {
  for (int i = 0; i < N; ++i)
    if (i < rev[i]) swap(A[i], A[rev[i]]);
  for (int len = 2; len <= N; len <<= 1) {
    int m = len >> 1;
    for (LL* p = A; p != A + N; p += len)
      for (int i = 0; i < m; ++i) {
        LL u = p[i], v = p[i + m] * w[N / len * i] % modu;
        p[i] = (u + v) % modu;
        p[i + m] = (u - v + modu) % modu;
      }
    for (int i = 0; i < N; ++i) cout << A[i] << " ";
    cout << endl;
  }
}
inline void IDFT(LL* A) {
  DFT(A);
  reverse(A + 1, A + N);
  for (int i = 0; i < N; ++i) mul(A[i], I);
}
}  // namespace FFT

inline poly operator+(const poly& a, const poly& b) {
  poly ans(max(a.size(), b.size()));
  for (int i = 0; i < ans.size(); ++i) {
    if (i < a.size()) add(ans[i], a[i]);
    if (i < b.size()) add(ans[i], b[i]);
  }
  return ans.normalize();
}
inline poly operator-(const poly& a, const poly& b) {
  poly ans(max(a.size(), b.size()));
  for (int i = 0; i < ans.size(); ++i) {
    if (i < a.size()) add(ans[i], a[i]);
    if (i < b.size()) sub(ans[i], b[i]);
  }
  return ans.normalize();
}

const int lim = 300;
inline poly operator*(poly a, poly b) {
  if (a.size() < lim && b.size() < lim) {
    poly ans(a.size() + b.size());
    for (int i = 0; i < a.size(); ++i)
      for (int j = 0; j < b.size(); ++j) add(ans[i + j], a[i] * b[j]);
    return ans.normalize();
  } else {
    FFT::init(a.size() + b.size());
    a.resize(FFT::N), FFT::DFT(&a[0]);
    b.resize(FFT::N), FFT::DFT(&b[0]);
    for (int i = 0; i < FFT::N; ++i) mul(a[i], b[i]);
    FFT::IDFT(&a[0]);
    return a.normalize();
  }
}

inline poly operator*(poly a, LL y) {
  for (auto& x : a) mul(x, y);
  return a;
}
inline poly operator/(poly a, LL y) { return a * pw(y); }
inline poly operator%(poly a, int k) {
  a.resize(k);
  return a;
}

inline poly operator~(poly a) {  // inv mod x^[a.size()+1]
  poly ans = {pw(a[0])};
  for (int len = 2; len < 2 * a.size(); len <<= 1) {
    poly ta = a % len;
    ans = (poly{2} - ans * ta % len) * ans;
    ans.resize(len);
  }
  return ans % a.size();
}

inline poly operator/(poly a, poly b) {
  assert(b.size());
  int len = a.size() - b.size() + 1;
  if (len <= 0) return poly{};
  reverse(a.begin(), a.end());
  reverse(b.begin(), b.end());
  b.resize(len);
  poly ans = a * (~b) % len;
  reverse(ans.begin(), ans.end());
  return ans.normalize();
}

inline poly operator%(poly a, poly b) { return (a - a / b * b).normalize(); }

inline pair<poly, poly> divmod(poly a, poly b) {
  poly d = a / b;
  return make_pair(d, (a - d * b).normalize());
}

inline poly sqrt(poly a) {  // a0=1,2*len space
  poly ans = {1};
  for (int len = 2; len < 2 * a.size(); len <<= 1) {
    poly ta = a % len;
    ans.resize(len);
    ans = (ans / 2 + ta / 2 * ~ans) % len;
  }
  return ans % a.size();
}

inline poly d(poly a) {
  poly ans(a.size() - 1);
  for (int i = 1; i < a.size(); ++i) ans[i - 1] = a[i] * i % modu;
  return ans;
}
inline poly I(poly a) {
  poly ans(a.size() + 1);
  for (int i = 0; i < a.size(); ++i) ans[i + 1] = a[i] * pw(i + 1) % modu;
  return ans;
}

inline poly ln(poly a) {  // a0=1
  return I(d(a) * ~a) % a.size();
}

inline poly exp(poly a) {  // a0=0
  poly ans = {1};
  for (int len = 2; len < 2 * a.size(); len <<= 1) {
    poly ta = a % len;
    ans.resize(len);
    ans = ans * (poly{1} - ln(ans) + ta) % len;
  }
  return ans % a.size();
}

inline poly operator^(poly a, LL k) {  // a0!=0
  return exp(ln(a / a[0]) * k) * pw(a[0], k);
}

inline poly operator<<(poly a, int k) {
  poly ans(a.size() + k);
  for (int i = 0; i < a.size(); ++i) ans[i + k] = a[i];
  return ans;
}
inline poly operator>>(poly a, int k) {
  if (a.size() <= k) return poly{};
  poly ans(a.size() - k);
  for (int i = k; i < a.size(); ++i) ans[i - k] = a[i];
  return ans;
}

inline vector<LL> evaluate(const poly& p, const vector<LL>& x) {
  function<poly(int, int)> multi = [&](int l, int r) -> poly {
    if (l == r) return poly{modu - x[l], 1};
    int mid = (l + r) >> 1;
    return multi(l, mid) * multi(mid + 1, r);
  };

  vector<LL> y(x.size());
  function<void(const poly&, int, int)> calc = [&](const poly& p, int l, int r) {
    if (r - l + 1 <= lim)
      for (int i = l; i <= r; ++i) y[i] = p(x[i]);
    else {
      int mid = (l + r) >> 1;
      poly L = multi(l, mid), R = multi(mid + 1, r);
      calc(p % L, l, mid), calc(p % R, mid + 1, r);
    }
  };
  calc(p, 0, x.size() - 1);
  return y;
}

inline poly interpolate(const vector<LL>& x, const vector<LL>& y) {
  function<poly(int, int)> multi = [&](int l, int r) -> poly {
    if (l == r) return poly{-x[l], 1};
    int mid = (l + r) >> 1;
    return multi(l, mid) * multi(mid + 1, r);
  };
  poly g = multi(0, x.size() - 1);

  auto c = evaluate(d(g), x);
  for (int i = 0; i < x.size(); ++i) c[i] = y[i] * pw(c[i]) % modu;

  function<poly(int, int)> calc = [&](int l, int r) -> poly {
    if (r - l + 1 <= lim) {
      poly ans0{1}, ans1;
      for (int i = l; i <= r; ++i) {
        poly p = poly{modu - x[i], 1};
        ans1 = ans1 * p + ans0 * c[i];
        ans0 = ans0 * p;
      }
      return ans1;
    } else {
      int mid = (l + r) >> 1;
      poly L = multi(l, mid), R = multi(mid + 1, r);
      return calc(l, mid) * R + calc(mid + 1, r) * L;
    }
  };
  return calc(0, x.size() - 1);
}

// gcd

struct polyMat {
  poly a0, a1, b0, b1;
  /* a0 a1
     b1 b1 */
  polyMat(const poly& a0 = {}, const poly& a1 = {}, const poly& b0 = {}, const poly& b1 = {})
      : a0(a0), a1(a1), b0(b0), b1(b1) {}
  bool operator==(const polyMat& M) const {
    return a0 == M.a0 && a1 == M.a1 && b0 == M.b0 && b1 == M.b1;
  }
};

const polyMat swapMat = polyMat({}, {1}, {1}, {});
const polyMat identityMat = polyMat({1}, {}, {}, {1});

struct polyVec {
  poly a, b;
  polyVec(const poly& a = {}, const poly& b = {}) : a(a), b(b) {}
};

ostream& operator<<(ostream& os, const polyVec& V) {
  os << "V[ " << V.a << ", " << V.b << " ]";
  return os;
}
ostream& operator<<(ostream& os, const polyMat& M) {
  os << "M[ " << M.a0 << ", " << M.a1 << endl << "   " << M.b0 << ", " << M.b1 << " ]";
  return os;
}

polyMat operator*(const polyMat& A, const polyMat& B) {
  return polyMat(A.a0 * B.a0 + A.a1 * B.b0,
                 A.a0 * B.a1 + A.a1 * B.b1,
                 A.b0 * B.a0 + A.b1 * B.b0,
                 A.b0 * B.a1 + A.b1 * B.b1);
}
polyVec operator*(const polyMat& M, const polyVec& V) {
  return polyVec(M.a0 * V.a + M.a1 * V.b, M.b0 * V.a + M.b1 * V.b);
}

inline polyMat hgcd(const poly& a, const poly& b) {
  assert(a.size() > b.size());
  int m = (a.deg() + 1) >> 1, n = b.deg();
  if (n < m) return identityMat;

  polyMat R = hgcd(a >> m, b >> m);
  polyVec V = R * polyVec(a, b);
  if (V.b.deg() < m) return R;

  auto q = divmod(V.a, V.b);
  polyMat Q({}, {1}, {1}, -q.first);
  int k = (m << 1) - V.b.deg();
  return hgcd(V.b >> k, q.second >> k) * Q * R;
}

inline polyMat cogcd(const poly& a, const poly& b) {
  if (!a.size()) return swapMat;
  if (!b.size()) return identityMat;

  if (a.size() <= b.size()) {
    auto q = divmod(b, a);
    polyMat Q({1}, {}, -q.first, {1});
    return cogcd(a, q.second) * Q;
  } else {
    polyMat M = hgcd(a, b);
    polyVec V = M * polyVec(a, b);
    return cogcd(V.b, V.a) * swapMat * M;
  }
};

inline poly gcd(const poly& a, const poly& b) {
  polyMat M = cogcd(a, b);
  polyVec V = M * polyVec(a, b);
  assert(V.b == poly{});
  return V.a;
}
inline poly exgcd(const poly& a, const poly& b, poly& x, poly& y) {
  polyMat M = cogcd(a, b);
  polyVec V = M * polyVec(a, b);
  assert(V.b == poly{});
  tie(x, y) = make_pair(M.a0, M.a1);
  return V.a;
}

// resultant:
// let A=an*\prod[i=1...n](x-ai)
// let B=bm*\prod[i=1...n](x-bj)
/*R(A,B)=bm^n*\prod A(bj)
        =bm^n*an^m*\prod_{ij}(bj-ai)
        =(-1)^(nm)*an^m*\prod B(ai)
*/
/* propertiy
    1. R(A,B)=(-1)^(nm)*R(B,A)
    2. R(A,B)=an^m*bm^n if n==0 or m==0
    3. R(A-CB,B)=R(A,B) if bm==1   -> R(A,B)=R(A%B,B)
*/
inline LL resultant(const poly& a, const poly& b) {
  if (!a.size() || !b.size()) return 0;
  if (a.size() == 1 || b.size() == 1) return pw(a.back(), b.deg()) * pw(b.back(), a.deg()) % modu;
  poly t = b / b.back();
  return sgn(a.deg() * b.deg()) * resultant(t, a % t) % modu * pw(b.back(), a.deg()) % modu;
}
}  // namespace polynomial

using namespace polynomial;

int main() {
  poly a = {1, 2, 3, 4}, b;
  FFT::init(4);
  FFT::DFT(&a[0]);
  return 0;
  int k;
  cin >> a >> b;
  cout << a + b << endl;
  cout << a - b << endl;
  cout << a * b << endl;
  cout << a / b << endl;
  cout << a % b << endl;
  cout << (~a) << endl;
  cout << sqrt(a) << endl;
  cout << ln(a) << endl;
  cout << exp(a) << endl;
  cout << (a ^ k) << endl;
  cout << gcd(a, b) << endl;
  cout << resultant(a, b) << endl;
}