#include <bits/stdc++.h>
#define maxc 26
#define maxn 300050
using namespace std;

int sz, last;
int fa[maxn];
int len[maxn];
int ch[maxn][maxc];

int newnode(int l = 0) {
  fa[sz] = -1;
  len[sz] = l;
  return sz++;
}

void extend(int c) {
  int k = last, leaf = newnode(len[k] + 1);
  for (; ~k && !ch[k][c]; k = fa[k]) ch[k][c] = leaf;
  if (!~k)
    fa[leaf] = 0;
  else {
    int p = ch[k][c];
    if (len[p] == len[k] + 1)
      fa[leaf] = p;
    else {
      int np = newnode(len[k] + 1);
      memcpy(ch[np], ch[p], sizeof(ch[p]));
      fa[np] = fa[p];
      fa[p] = fa[leaf] = np;
      for (; ~k && ch[k][c] == p; k = fa[k]) ch[k][c] = np;
    }
  }
  last = leaf;
}

void travel(int k, string s) {
  cout << k << ":" << s << endl;
  for (int c = 0; c < maxc; ++c)
    if (ch[k][c]) travel(ch[k][c], s + (char)(c + 'a'));
}

int main() {
  string s;
  cin >> s;

  last = newnode();
  for (int i = 0; i < s.size(); ++i) {
    int w = s[i] - 'a';
    if (ch[last][w] && len[ch[last][w]] == i + 1)
      last = ch[last][w];
    else
      extend(w);
  }

  travel(0, "");

  for (int k = 0; k < sz; ++k)
    for (int c = 0; c < maxc; ++c)
      if (ch[k][c]) cout << k << " -> " << ch[k][c] << endl;
  return 0;
}