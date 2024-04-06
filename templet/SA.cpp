#include <bits/stdc++.h>
#define maxh 20
#define maxn 500050
using namespace std;

struct SA {
  int sa[maxn], rk[maxn];

  int Log2[maxn];
  int ST[maxh][maxn], *const h = ST[0];

  void build(char *s, int n) {  // no zero
    static int ts[2][maxn], sum[maxn];

    int *x = ts[0], *y = ts[1], m = 256;  // attention!
    for (int i = 1; i <= m; ++i) sum[i] = 0;
    for (int i = 1; i <= n; ++i) ++sum[x[i] = s[i]];
    for (int i = 1; i <= m; ++i) sum[i] += sum[i - 1];
    for (int i = n; i; --i) sa[sum[s[i]]--] = i;

    for (int k = 1; k <= n; k <<= 1) {
      int p = 0;
      for (int i = n; i > n - k; --i) y[++p] = i;
      for (int i = 1; i <= n; ++i)
        if (sa[i] > k) y[++p] = sa[i] - k;
      for (int i = 1; i <= m; ++i) sum[i] = 0;
      for (int i = 1; i <= n; ++i) ++sum[x[y[i]]];
      for (int i = 1; i <= m; ++i) sum[i] += sum[i - 1];
      for (int i = n; i; --i) sa[sum[x[y[i]]]--] = y[i];
      swap(x, y);
      m = x[sa[1]] = 1;
      for (int i = 2; i <= n; ++i)
        x[sa[i]] = (y[sa[i]] == y[sa[i - 1]] && y[sa[i] + k] == y[sa[i - 1] + k]
                        ? m
                        : ++m);
      if (m >= n) break;
    }

    for (int i = 1; i <= n; ++i) rk[sa[i]] = i;
    for (int i = 1, k = 0; i <= n; ++i) {
      if (k) --k;
      int j = sa[rk[i] - 1];
      while (s[i + k] == s[j + k]) ++k;
      h[rk[i]] = k;
    }

    Log2[0] = -1;
    for (int i = 1; i <= n; ++i) Log2[i] = Log2[i >> 1] + 1;
    for (int j = 1; j <= Log2[n]; ++j)
      for (int i = 1; i + (1 << j) - 1 <= n; ++i)
        ST[j][i] = min(ST[j - 1][i], ST[j - 1][i + (1 << j - 1)]);
  }
  int lcp(int i, int j) {
    if (i == j) return maxn;
    int l = rk[i], r = rk[j];
    if (l > r) swap(l, r);
    ++l;
    int t = Log2[r - l + 1];
    return min(ST[t][l], ST[t][r - (1 << t) + 1]);
  }
} S;