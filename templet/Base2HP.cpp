#include<bits/stdc++.h>
#define val(k) T[k].val
#define ls(k)   T[k].ls
#define rs(k)   T[k].rs
#define maxn 150050
#define modu 998244353
using namespace std;
typedef long long LL;
const int N=131072,C=2,K=18;

int sz;
struct node {
    int ls,rs,val;
    node(int _val=0,int _ls=0,int _rs=0) {
        val=_val,ls=_ls,rs=_rs;
    }
}   T[N*K*K];

int subs[C][K];
int Log2[maxn];
int pw[maxn],ALL[C][maxn];

int sgn(int x)  {   return x>0?0:1; }
struct HP   {
    int rt,sgn;
    #define mid ((l+r)>>1)
    HP(int _rt=0,int _sgn=0)    {   rt=_rt,sgn=_sgn;    }
    bool comp(int k1,int k2,int l,int r) const {
        if (l==r)   return val(k1)<val(k2);
        return val(rs(k1))==val(rs(k2))?comp(ls(k1),ls(k2),l,mid):comp(rs(k1),rs(k2),mid+1,r);
    }
    bool operator < (const HP& x)  const {   return (sgn||x.sgn)^comp(rt,x.rt,1,N);  }
    int change(int &k,int l,int r,int p,int c)    {
        if (l>=p)
            if (val(k)==ALL[c^1][r-l+1])
                return k=subs[c][Log2[r-l+1]],1;
        if (l==r)   return k=subs[c^1][0],0;
        T[++sz]=T[k],k=sz;
        int w=0;
        if (p>mid||change(ls(k),l,mid,p,c))
            w=change(rs(k),mid+1,r,p,c);
        val(k)=(val(ls(k))+(LL)pw[mid-l+1]*val(rs(k)))%modu;
        return w;
    }
    inline void plus(int x) {  if (x) sgn^=change(rt,1,N,abs(x),::sgn(x)); }
    void add(int k,int l=1,int r=N) {
        if (!val(k))    return;
        if (l==r)   plus(l);
        else    add(ls(k),l,mid),add(rs(k),mid+1,r);
    }
    int travel(int k,int l,int r,int f,int pres) const {
        if (l==r)
            return (f||pres!=val(k))?putchar((val(k)^pres)+'0'),1:0;
        return travel(ls(k),l,mid,travel(rs(k),mid+1,r,f,pres),pres);
    }
};

ostream& operator << (ostream& os,HP x)  {
    int pres=x.sgn;
    if (x.sgn)    x.plus(-1),os<<"-";
    if (!x.travel(x.rt,1,N,0,pres)) os<<'0';
    return os;
}

int main()  {
}