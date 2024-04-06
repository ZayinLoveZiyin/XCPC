#include <bits/stdc++.h>
using namespace std;

#define mp make_pair
#define mt make_tuple
#define rep(i, begin, end)                                                                      \
  for (__typeof(begin) i = (begin), _end = (end), _step = 1 - 2 * ((begin) > (end)); i != _end; \
       i += _step)

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

void printr(ostream& os) { os << '\n'; }
template <class T, class... Args>
void printr(ostream& os, const T& t, const Args&... args) {
  os << t;
  if (sizeof...(args)) os << " ";
  printr(os, args...);
}
#define print(args...) printr(cout, ##args)

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
#define dbg(args...) cout << #args << " = ", dbgr(cout, ##args)  // cout can be changed

#define maxn 50
#define modu 1000002012LL
typedef long long LL;
typedef pair<int, int> pii;
typedef pair<LL, LL> pll;

// COMMON FUNCTION
// min
template <class T>
T min(const vector<T>& v) {
  return *min_element(v.begin(), v.end());
}
// max
template <class T>
T max(const vector<T>& v) {
  return *max_element(v.begin(), v.end());
}

LL pw(LL a, LL k = modu - 2) {
  LL ans = 1;
  for (a %= modu; k; k >>= 1) {
    if (k & 1) ans = ans * a % modu;
    a = a * a % modu;
  }
  return ans;
}

// #define original_int
#ifndef original_int
namespace integer {

struct __int_hp {
  static const int prec = 0;
  static const int base = 1e9;
  static const int width = 9;

  int sgn;
  std::vector<long long> digits;
  __int_hp() : sgn(1), digits(prec, 0) {}
  __int_hp(int x) : digits(prec, 0) {
    sgn = (x >= 0 ? 1 : -1);
    x = abs(x);
    for (; x; x /= base) digits.push_back(x % base);
  }
  __int_hp(long long x) : digits(prec, 0) {
    sgn = (x >= 0 ? 1 : -1);
    x = abs(x);
    for (; x; x /= base) digits.push_back(x % base);
  }
  __int_hp(const char* str) : __int_hp(std::string(str)) {}
  __int_hp(std::string str) : digits(prec, 0) {
    if (str[0] == '-') {
      sgn = -1;
      str.erase(str.begin());
    } else if (str[0] == '+') {
      sgn = 1;
      str.erase(str.begin());
    } else {
      sgn = 1;
    }
    for (int r = str.size() - 1; r >= 0; r -= width) {
      int l = std::max(0, r - width + 1);
      digits.push_back(std::stoi(str.substr(l, r - l + 1)));
    }
  }
  __int_hp(int _sgn, std::vector<long long> _digits) : sgn(_sgn), digits(_digits) { normalize(); }
  __int_hp operator-() const { return __int_hp(-sgn, digits); }
  void cut_leading_zero() {
    while (digits.size() > prec && !digits.back()) digits.pop_back();
  }
  void normalize() {
    for (int i = 0; i < digits.size(); ++i) {
      if (digits[i] < 0) {
        if (i == digits.size() - 1) digits.push_back(0);
        digits[i + 1] -= (base - 1 - digits[i]) / base;
        digits[i] = digits[i] % base + base;
      }
      if (digits[i] >= base) {
        if (i == digits.size() - 1) digits.push_back(0);
        digits[i + 1] += digits[i] / base;
        digits[i] %= base;
      }
    }
    cut_leading_zero();
  }

  size_t size() const { return digits.size(); }
  long long operator[](const int& i) const { return digits[i]; }
  long long& operator[](const int& i) { return digits[i]; }

  // operation

  friend __int_hp operator+(const __int_hp& a, const __int_hp& b);
  friend __int_hp operator-(const __int_hp& a, const __int_hp& b);
  friend __int_hp operator*(const __int_hp& a, const long long x);
  friend __int_hp operator/(const __int_hp& a, const long long x);
  friend __int_hp operator%(const __int_hp& a, const long long x);

  __int_hp& operator+=(const __int_hp& b) { return *this = *this + b; }
  __int_hp& operator-=(const __int_hp& b) { return *this = *this - b; }
  __int_hp& operator*=(const long long& b) { return *this = *this * b; }
  __int_hp& operator/=(const long long& b) { return *this = *this / b; }
  __int_hp& operator%=(const long long& b) { return *this = *this % b; }

  friend std::ostream& operator<<(std::ostream& os, const __int_hp& x) {
    if (x.sgn == -1) os << "-";
    if (x.size() == __int_hp::prec)
      os << "0";
    else {
      os << x.digits.back();
      for (int i = x.size() - 2; i >= __int_hp::prec; --i)
        os << std::setfill('0') << std::setw(__int_hp::width) << x[i] << std::setfill(' ');
    }
    return os;
  }

  std::string to_string() const {
    std::stringstream os;
    os << *this;
    return os.str();
  }
};

bool operator<(const __int_hp& a, const __int_hp& b) {
  if (a.sgn != b.sgn) return a.sgn < b.sgn;
  if (a.size() != b.size()) return (a.size() < b.size()) ^ (!~a.sgn);
  for (int i = a.size() - 1; i >= 0; --i)
    if (a[i] != b[i]) return (a[i] < b[i]) ^ (!~a.sgn);
  return 0;
}
bool operator<=(const __int_hp& a, const __int_hp& b) {
  if (a.sgn != b.sgn) return a.sgn < b.sgn;
  if (a.size() != b.size()) return (a.size() < b.size()) ^ (!~a.sgn);
  for (int i = 0; i < a.size(); ++i)
    if (a[i] != b[i]) return (a[i] < b[i]) ^ (!~a.sgn);
  return 1;
}
bool operator==(const __int_hp& a, const __int_hp& b) {
  if (a.sgn != b.sgn) return 0;
  if (a.size() != b.size()) return 0;
  for (int i = 0; i < a.size(); ++i)
    if (a[i] != b[i]) return 0;
  return 1;
}
bool operator>(const __int_hp& a, const __int_hp& b) { return b < a; }
bool operator>=(const __int_hp& a, const __int_hp& b) { return b <= a; }
bool operator!=(const __int_hp& a, const __int_hp& b) { return !(a == b); }

__int_hp operator+(const __int_hp& a, const __int_hp& b) {
  if (a.sgn == b.sgn) {
    __int_hp ans;
    ans.sgn = a.sgn;
    ans.digits.resize(std::max(a.digits.size(), b.digits.size()) + 1);
    for (int i = 0; i < ans.digits.size(); ++i) {
      if (i < a.digits.size()) ans[i] += a[i];
      if (i < b.digits.size()) ans[i] += b[i];
    }
    ans.normalize();
    return ans;
  } else if (a.sgn == 1 && b.sgn == -1)
    return a - (-b);
  else if (a.sgn == -1 && b.sgn == 1)
    return b - (-a);
}
__int_hp operator-(const __int_hp& a, const __int_hp& b) {
  if (a.sgn == 1 && b.sgn == 1) {
    if (a < b) return -(b - a);
    __int_hp ans;
    ans.sgn = 1;
    ans.digits.resize(std::max(a.digits.size(), b.digits.size()));
    for (int i = 0; i < ans.digits.size(); ++i) {
      if (i < a.digits.size()) ans[i] += a[i];
      if (i < b.digits.size()) ans[i] -= b[i];
    }
    ans.normalize();
    return ans;
  } else if (a.sgn == 1 && b.sgn == -1)
    return a + (-b);
  else if (a.sgn == -1 && b.sgn == -1)
    return (-b) - (-a);
  else if (a.sgn == -1 && b.sgn == 1)
    return -(b + (-a));
}

__int_hp operator*(const __int_hp& a, long long x) {
  __int_hp ans = a;
  if (x < 0) ans.sgn = -ans.sgn, x = -x;
  for (long long& d : ans.digits) d *= x;
  ans.normalize();
  return ans;
}
__int_hp operator/(const __int_hp& a, long long x) {
  __int_hp ans = a;
  if (x < 0) ans.sgn = -ans.sgn, x = -x;
  long long t = 0;
  for (int i = ans.size() - 1; i >= 0; --i) {
    t = t * __int_hp::base + ans[i];
    ans[i] = t / x;
    t %= x;
  }
  ans.normalize();
  return ans;
}
__int_hp operator%(const __int_hp& a, long long x) { return a - (a / x) * x; }
}  // namespace integer
#else

namespace integer {
using __int_hp = __int128;
}

#endif

ostream& operator<<(ostream& os, const __int128& x) {
  const LL M = 1e18;
  if (x >= M) os << LL(x / M);
  os << LL(x % M);
  return os;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0), cout.tie(0);

  integer::__int_hp a = 10;
  cout << a * 1000 << endl;
  return 0;
}
/*README
1. Is modu correct?
2. Is maxn correct?
*/