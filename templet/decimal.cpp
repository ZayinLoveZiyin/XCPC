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
ostream& operator<<(ostream& os, const multiset<T>& s) {
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
#define dbg(args...) \
  cout << #args << " = ", dbgr(cout, ##args)  // cout can be changed

#define maxn 30050
#define modu 998244353
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

pii operator-(const pii& p) { return mp(-p.first, p.second); }

// #define original_double
#ifndef original_double

namespace decimal {

struct __float_hp {
  static const int prec = 12;
  static const int output_prec = 12;
  static const int base = 1e9;
  static const int width = 9;

  int sgn = 1;
  vector<LL> digits;
  __float_hp() : sgn(1), digits(prec, 0) {}
  __float_hp(int x) : digits(prec, 0) {
    sgn = (x >= 0 ? 1 : -1);
    x = abs(x);
    for (; x; x /= base) digits.push_back(x % base);
  }
  __float_hp(LL x) : digits(prec, 0) {
    sgn = (x >= 0 ? 1 : -1);
    x = abs(x);
    for (; x; x /= base) digits.push_back(x % base);
  }
  __float_hp(double x) : digits(prec, 0) {
    sgn = (x >= 0 ? 1 : -1);
    x = abs(x);
    double t = x - (LL)x;
    for (int i = prec - 1; i >= 0; --i) {
      t *= base;
      digits[i] = (LL)t;
      t -= digits[i];
    }
    for (LL t = (LL)x; t; t /= base) digits.push_back(t % base);
  }
  __float_hp(int _sgn, vector<LL> _digits) : sgn(_sgn), digits(_digits) {
    normalize(digits);
  }
  __float_hp(string s) {
    if (s[0] == '-') {
      sgn = -1;
      s.erase(s.begin());
    }
    int dot_pos = s.find('.');
    if (!~dot_pos) {
      s += string(width * prec, '0');
    } else {
      s.erase(s.begin() + dot_pos);
      s.resize(dot_pos + width * prec, '0');
    }
    for (int i = s.size() - 1; i >= 0;) {
      int l = min(i + 1, width);
      digits.push_back(std::stoi(s.substr(i - l + 1, l)));
      i -= l;
    }
  }
  __float_hp(const char* s) : __float_hp(string(s)) {}
  __float_hp operator-() const { return __float_hp(-sgn, digits); }

  string to_string() const {
    stringstream os;
    if (sgn == -1) os << "-";
    if (size() == prec)
      os << "0";
    else {
      os << digits.back();
      for (int i = digits.size() - 2; i >= prec; --i)
        os << setfill('0') << setw(width) << digits[i] << setfill(' ');
    }
    if (prec) os << ".";
    for (int i = __float_hp::prec - 1; i >= 0; --i)
      os << setfill('0') << setw(width) << digits[i] << setfill(' ');
    return os.str();
  }

  static void cut_leading_zero(vector<LL>& digits) {
    while (digits.size() > prec && !digits.back()) digits.pop_back();
  }
  static void calc_carry(vector<LL>& digits) {
    for (int i = 0; i < digits.size(); ++i) {
      if (digits[i] < 0) {
        if (i == digits.size() - 1) digits.push_back(0);
        digits[i + 1] -= (base - 1 - digits[i]) / base;
        digits[i] = (digits[i] % base + base) % base;
      }
      if (digits[i] >= base) {
        if (i == digits.size() - 1) digits.push_back(0);
        digits[i + 1] += digits[i] / base;
        digits[i] %= base;
      }
    }
  }
  static void normalize(vector<LL>& digits) {
    calc_carry(digits);
    cut_leading_zero(digits);
  }

  size_t size() const { return digits.size(); }
  LL operator[](const int& i) const { return digits[i]; }
  LL& operator[](const int& i) { return digits[i]; }

  // operation

  friend __float_hp operator+(const __float_hp& a, const __float_hp& b);
  friend __float_hp operator-(const __float_hp& a, const __float_hp& b);
  friend __float_hp operator*(const __float_hp& a, const __float_hp& b);
  friend __float_hp operator*(const __float_hp& a, LL x);
  friend __float_hp operator/(const __float_hp& a, LL x);

  __float_hp& operator+=(const __float_hp& b) {
    *this = *this + b;
    return *this;
  }
  __float_hp& operator-=(const __float_hp& b) {
    *this = *this - b;
    return *this;
  }
  __float_hp& operator*=(const __float_hp& b) {
    *this = *this * b;
    return *this;
  }
  __float_hp& operator*=(LL x) {
    *this = *this * x;
    return *this;
  }
  __float_hp& operator/=(LL x) {
    *this = *this / x;
    return *this;
  }
};

ostream& operator<<(ostream& os, const __float_hp& x) {
  string xs = x.to_string();
  xs.resize(xs.size() - __float_hp::prec * __float_hp::width +
            __float_hp::output_prec);
  os << xs;
  return os;
}

bool operator<(const __float_hp& a, const __float_hp& b) {
  if (a.sgn != b.sgn) return a.sgn < b.sgn;
  if (a.size() != b.size()) return (a.size() < b.size()) ^ (!~a.sgn);
  for (int i = a.size() - 1; i >= 0; --i)
    if (a[i] != b[i]) return (a[i] < b[i]) ^ (!~a.sgn);
  return 0;
}
bool operator<=(const __float_hp& a, const __float_hp& b) {
  if (a.sgn != b.sgn) return a.sgn < b.sgn;
  if (a.size() != b.size()) return (a.size() < b.size()) ^ (!~a.sgn);
  for (int i = a.size() - 1; i >= 0; --i)
    if (a[i] != b[i]) return (a[i] < b[i]) ^ (!~a.sgn);
  return 1;
}
bool operator==(const __float_hp& a, const __float_hp& b) {
  if (a.sgn != b.sgn) return 0;
  if (a.size() != b.size()) return 0;
  for (int i = 0; i < a.size(); ++i)
    if (a[i] != b[i]) return 0;
  return 1;
}
bool operator>(const __float_hp& a, const __float_hp& b) { return b < a; }
bool operator>=(const __float_hp& a, const __float_hp& b) { return b <= a; }
bool operator!=(const __float_hp& a, const __float_hp& b) { return !(a == b); }

__float_hp operator+(const __float_hp& a, const __float_hp& b) {
  if (a.sgn == b.sgn) {
    __float_hp ans;
    ans.sgn = a.sgn;
    ans.digits.resize(max(a.digits.size(), b.digits.size()) + 1);
    for (int i = 0; i < ans.digits.size(); ++i) {
      if (i < a.digits.size()) ans[i] += a[i];
      if (i < b.digits.size()) ans[i] += b[i];
    }
    __float_hp::normalize(ans.digits);
    return ans;
  } else if (a.sgn == 1 && b.sgn == -1)
    return a - (-b);
  else if (a.sgn == -1 && b.sgn == 1)
    return b - (-a);
}
__float_hp operator-(const __float_hp& a, const __float_hp& b) {
  if (a.sgn == 1 && b.sgn == 1) {
    if (a < b) return -(b - a);
    __float_hp ans;
    ans.sgn = 1;
    ans.digits.resize(max(a.digits.size(), b.digits.size()));
    for (int i = 0; i < ans.digits.size(); ++i) {
      if (i < a.digits.size()) ans[i] += a[i];
      if (i < b.digits.size()) ans[i] -= b[i];
    }
    __float_hp::normalize(ans.digits);
    return ans;
  } else if (a.sgn == 1 && b.sgn == -1)
    return a + (-b);
  else if (a.sgn == -1 && b.sgn == -1)
    return (-b) - (-a);
  else if (a.sgn == -1 && b.sgn == 1)
    return -(b + (-a));
}
__float_hp operator*(const __float_hp& a, const __float_hp& b) {
  vector<LL> ans(a.size() + b.size());
  for (int i = 0; i < a.size(); ++i)
    if (a[i])
      for (int j = 0; j < b.size(); ++j) ans[i + j] += a[i] * b[j];
  __float_hp::calc_carry(ans);
  for (int i = 0; i + __float_hp::prec < ans.size(); ++i)
    ans[i] = ans[i + __float_hp::prec];
  ans.resize(ans.size() - __float_hp::prec);
  return __float_hp(a.sgn * b.sgn, ans);
}

__float_hp operator*(const __float_hp& a, LL x) {
  __float_hp ans = a;
  if (x < 0) ans.sgn = -ans.sgn, x = -x;
  for (LL& d : ans.digits) d *= x;
  __float_hp::normalize(ans.digits);
  return ans;
}
__float_hp operator/(const __float_hp& a, LL x) {
  // cout<<"to calc:"<<a<<" "<<x<<endl;
  __float_hp ans = a;
  if (x < 0) ans.sgn = -ans.sgn, x = -x;
  __int128 t = 0;
  for (int i = ans.size() - 1; i >= 0; --i) {
    t = t * __float_hp::base + ans[i];
    ans[i] = t / x;
    t %= x;
  }
  ans.normalize(ans.digits);
  return ans;
}
}  // namespace decimal

#else

namespace decimal {
using __float_hp = double;
}

#endif

int main() {
  decimal::__float_hp a = "12.345451457584312345";
  cout << a << endl;
  return 0;
}
/*README
1. Is modu correct?
2. Is maxn correct?
*/