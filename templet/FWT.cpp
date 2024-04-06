#include <bits/stdc++.h>
#define modu 1000000009
using namespace std;
typedef long long LL;

// x+y*w ( w^3-1=(w-1)(w^2+w+1)=0 )
struct cd {
  LL x, y;
  cd(LL _x = 0, LL _y = 0) {
    x = (_x % modu + modu) % modu;
    y = (_y % modu + modu) % modu;
  }
  cd operator+(const cd& C) const { return cd(x + C.x, y + C.y); }
  cd operator-(const cd& C) const { return cd(x - C.x, y - C.y); }
  cd operator*(const cd& C) const {
    return cd(x * C.x - y * C.y, x * C.y + y * C.x - y * C.y);
  }
};
ostream& operator<<(ostream& os, const cd& C) {
  os << "(" << C.x << "," << C.y << ")";
}
void add(cd& a, cd b) { a = a + b; }

LL pw(LL a) {
  LL ans = 1;
  for (int k = modu - 2; k; k >>= 1) {
    if (k & 1) ans = ans * a % modu;
    a = a * a % modu;
  }
  return ans;
}

struct FWT {
  static const int N = 6561;
  static cd w1(const cd& C) { return cd(-C.y, C.x - C.y); }
  static cd w2(const cd& C) { return cd(C.y - C.x, -C.x); }
  void DFT(cd* A) {
    for (int len = 3; len <= N; len *= 3) {
      int m = len / 3;
      for (cd* p = A; p != A + N; p += len)
        for (int i = 0; i < m; ++i) {
          cd &x = p[i], &y = p[i + m], &z = p[i + 2 * m];
          tie(x, y, z) =
              make_tuple(x + y + z, x + w1(y) + w2(z), x + w2(y) + w1(z));
        }
    }
  }
  void IDFT(cd* A) {
    for (int len = 3; len <= N; len *= 3) {
      int m = len / 3;
      for (cd* p = A; p != A + N; p += len)
        for (int i = 0; i < m; ++i) {
          cd &x = p[i], &y = p[i + m], &z = p[i + 2 * m];
          tie(x, y, z) =
              make_tuple(x + y + z, x + w2(y) + w1(z), x + w1(y) + w2(z));
        }
    }
    for (int i = 0; i < N; ++i) A[i] = A[i] * pw(N);
  }
} T;

for (int len = 2; len <= n; len <<= 2) {
  int m = len >> 1;
  for (int* p = A; p != A + n; p += len)
    for (int i = 0; i < m; ++i) {
      int u = p[i], v = p[i + m];
      p[i] = (u + v);
      p[i + m] = (u - v);
    }
}

#include <bits/stdc++.h>
using namespace std;

#define mp make_pair
#define mt make_tuple
#define rep(i, begin, end)                                \
  for (__typeof(begin) i = (begin), _end = (end),         \
                       _step = 1 - 2 * ((begin) > (end)); \
       i != _end; i += _step)

// OUTPUT
template <class a, class b>
ostream& operator<<(ostream& os, const pair<a, b>& p) {
  os << "(" << p.first << "," << p.second << ")";
  return os;
}
template <class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
  os << "[";
  if (!v.size())
    os << "]";
  else
    for (int i = 0; i < v.size(); ++i) os << v[i] << ",]"[i == v.size() - 1];
  return os;
}
template <class T>
ostream& operator<<(ostream& os, const set<T>& s) {
  os << "{";
  if (!s.size())
    os << "}";
  else
    for (auto x : s) os << x << ",}"[x == *s.rbegin()];
  return os;
}

void printr(ostream& os) { os << endl; }
template <class T, class... Args>
void printr(ostream& os, const T& t, const Args&... args) {
  os << t;
  if (sizeof...(args)) os << " ";
  printr(os, args...);
}
template <class... Args>
void print(const Args&... args) {
  printr(cout, args...);
}

// INPUT
template <class T>
void read(T& x) {
  cin >> x;
}
template <class T, class... Args>
void read(T& t, Args&... args) {
  read(t), read(args...);
}
template <class T>
void readarray(T* A, int n) {
  rep(i, 0, n) read(A[i]);
}

// DEBUG
void dbgr(ostream& os) { os << endl; }
template <class T, class... Args>
void dbgr(ostream& os, const T& t, const Args&... args) {
  os << t;
  if (sizeof...(args)) os << ", ";
  dbgr(os, args...);
}
#define dbg(args...) \
  cout << #args << " = ", dbgr(cout, ##args)  // cout can be changed

#define maxs 4000050
#define maxn 22
#define modu 998244353
typedef long long LL;
typedef pair<int, int> pii;
typedef pair<LL, LL> pll;

// COMMON FUNCTION
// random LL
LL randll(LL a, LL b) {
  uniform_int_distribution<int> u(a, b);
  static mt19937 e(time(0));
  return u(e);
}

void add(LL& a, LL b) { a = (a + b) % modu; }
void sub(LL& a, LL b) { a = (a - b % modu + modu) % modu; }

namespace NumberTheory {
LL F[maxn], I[maxn], FI[maxn];
void init() {
  F[0] = I[0] = I[1] = FI[0] = 1;
  for (int i = 1; i < maxn; ++i) F[i] = F[i - 1] * i % modu;
  for (int i = 2; i < maxn; ++i) I[i] = modu - modu / i * I[modu % i] % modu;
  for (int i = 1; i < maxn; ++i) FI[i] = FI[i - 1] * I[i] % modu;
  for (int i = 0; i < maxn; ++i) assert(F[i] * FI[i] % modu == 1);
}
LL C(LL n, LL m) { return n < m ? 0 : F[n] * FI[m] % modu * FI[n - m] % modu; }
LL pw(LL a, LL k = modu - 2) {
  LL ans = 1;
  for (a %= modu; k; k >>= 1) {
    if (k & 1) ans = ans * a % modu;
    a = a * a % modu;
  }
  return ans;
}
}  // namespace NumberTheory

namespace SetSeries {

void print(LL* a, int n) {
  for (int i = 0; i <= n; ++i) cout << a[i] << " ";
  cout << endl;
}

void FMT(LL A[maxs][maxn], int n) {
  for (int i = 0; i < n; ++i)
    for (int s = 0; s < (1 << n); ++s)
      if (s >> i & 1)
        for (int j = 0; j <= n; ++j)
          A[s][j] = (A[s][j] + A[s ^ (1 << i)][j]) % modu;
}
void IFMT(LL A[maxs][maxn], int n) {
  for (int i = 0; i < n; ++i)
    for (int s = 0; s < (1 << n); ++s)
      if (s >> i & 1)
        for (int j = 0; j <= n; ++j)
          A[s][j] = (A[s][j] - A[s ^ (1 << i)][j] + modu) % modu;
}

void convolute(LL* a, LL* b, LL* c, int n) {
  static LL tmp[maxn];
  for (int i = 0; i <= n; ++i) {
    tmp[i] = 0;
    for (int j = 0; j <= i; ++j) tmp[i] = (tmp[i] + a[j] * b[i - j]) % modu;
  }
  for (int i = 0; i <= n; ++i) c[i] = tmp[i];
}

void exp(LL* a, int n) {  // exp(a)=\sum[i>=0] a^i/i!, guranteed a[0]=0
  static LL tmp[maxn];
  for (int i = 0; i <= n; ++i) tmp[i] = 0;
  for (int i = tmp[0] = 1; i <= n; ++i) {
    for (int j = 0; j <= i; ++j)
      tmp[i] = (tmp[i] + j * a[j] % modu * tmp[i - j]) % modu;
    tmp[i] = tmp[i] * NumberTheory::I[i] % modu;
  }
  for (int i = 0; i <= n; ++i) a[i] = tmp[i];
}

void ln(LL* a, int n) {  // ln(1+a)=\sum[i>0](-1)^{i+1}a^i/i, gureeted a[0]=0
  static LL tmp[maxn];
  for (int i = 0; i <= n; ++i) tmp[i] = 0;
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j < i; ++j)
      tmp[i] = (tmp[i] + j * a[i - j] % modu * tmp[j]) % modu;
    tmp[i] = (a[i] - tmp[i] * NumberTheory::I[i] % modu + modu) % modu;
  }
  for (int i = 0; i <= n; ++i) a[i] = tmp[i];
}

void powk(LL* a, int n, LL k) {
  int lb = 0;
  while (lb <= n && !a[lb]) ++lb;
  if (lb > n || lb * k > n) {
    for (int i = 0; i <= n; ++i) a[i] = 0;
    return;
  }
  LL inv = NumberTheory::pw(a[lb]), alb_k = NumberTheory::pw(a[lb], k);
  for (int i = 0; i <= n; ++i)
    a[i] = (i + lb <= n ? a[i + lb] * inv % modu : 0);

  ln(a, n - lb);  // ignore a[0] though it has value.
  for (int i = 0; i <= n; ++i) a[i] = a[i] * k % modu;
  exp(a, n - lb);
  for (int i = n; i >= 0; --i)
    a[i] = (i - lb * k >= 0 ? a[i - lb * k] * alb_k % modu : 0);
}
void expk(LL a[maxn], int n, LL k) {  // expk(a)=\sum[0<=i<=k] a^i/i!
  static LL tmp[maxn], tmp2[maxn];
  for (int i = 0; i <= n; ++i) tmp[i] = tmp2[i] = 0;
  for (int i = 0; i <= n; ++i) tmp[i] = a[i];
  powk(tmp, n, k);
  for (int i = 0; i <= n; ++i) tmp[i] = tmp[i] * NumberTheory::FI[k] % modu;

  for (int i = 0; i <= k; ++i)
    tmp2[0] =
        (tmp2[0] + NumberTheory::pw(a[0], i) / NumberTheory::FI[i]) % modu;
  for (int i = 1; i <= n; ++i) {
    for (int j = 0; j < i; ++j)
      tmp2[i] =
          (tmp2[i] + (i - j) * a[i - j] % modu * (tmp2[j] - tmp[j] + modu)) %
          modu;
    tmp2[i] = tmp2[i] * NumberTheory::I[i] % modu;
  }
  for (int i = 0; i <= n; ++i) a[i] = tmp2[i];
}

void dosomthing(LL A[maxs][maxn], int n, function<void(LL*, int)> func) {
  FMT(A, n);
  // dbg("????");
  for (int s = 0; s < (1 << n); ++s) {
    // dbg(s);
    // print(A[s],n);
    func(A[s], n);
  }
  IFMT(A, n);
}
}  // namespace SetSeries

int n, m, k;
LL F[maxs][maxn], G[maxs][maxn];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0), cout.tie(0);
  NumberTheory::init();

  read(n, m, k);
  while (m--) {
    int s;
    read(s);
    ++F[s][__builtin_popcount(s)];
  }
  SetSeries::dosomthing(F, n,
                        [&](LL a[maxn], int n) { SetSeries::expk(a, n, k); });

  LL ans = 0;
  for (int i = 0; i <= n; ++i) ans = (ans + F[(1 << n) - 1][i]) % modu;
  cout << F[(1 << n) - 1][n] << endl;
  return 0;
}
/*README
1. Is modu correct?
2. Is maxn correct?
*/