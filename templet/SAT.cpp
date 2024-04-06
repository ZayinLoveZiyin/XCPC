#include<bits/stdc++.h>
#define maxn 200050
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;

int n;

namespace SAT {
    int s[maxn];

    const LL base1=31,modu1=998244353;
    const LL base2=131,modu2=1e9+7;

    LL pw1[maxn],h1[maxn];
    LL pw2[maxn],h2[maxn];

    void init() {
        n=0;
        for (int i=pw1[0]=1;i<maxn;++i) pw1[i]=pw1[i-1]*base1%modu1;
        for (int i=pw2[0]=1;i<maxn;++i) pw2[i]=pw2[i-1]*base2%modu2;
    }

    pair<LL,LL> gethash(int l,int r)    {
        return make_pair(
            (h1[r]+(modu1-h1[l-1])*pw1[r-l+1])%modu1,
            (h2[r]+(modu2-h2[l-1])*pw2[r-l+1])%modu2);
    }

    int LCP(int i,int j)    {
        int l=0,r=min(i,j);
        while (l<r) {
            int mid=(l+r+1)>>1;
            if (gethash(i-mid+1,i)==gethash(j-mid+1,j))
                l=mid;
            else
                r=mid-1;
        }
        return l;
    }

    ULL val[maxn];

    bool compare(int i,int j)   {
        if (s[i]!=s[j]) return s[i]<s[j];
        return val[i-1]<val[j-1];
    }
    
    int rnd[maxn];
    int rt,sz[maxn];
    int fa[maxn],ls[maxn],rs[maxn];

    void newnode(int n,ULL v)  {
        static uniform_int_distribution<int> u(1,1e9);
        static default_random_engine e(rand());

        rnd[n]=u(e);
        sz[n]=1;
        fa[n]=ls[n]=rs[n]=0;
        val[n]=v;
    }

    bool isRightChild(int k)  {
        return rs[fa[k]]==k;
    }

    int leftmost(int k) {
        while (ls[k]) k=ls[k];
        return k;
    }

    int rightmost(int k) {
        while (rs[k]) k=rs[k];
        return k;
    }

    void update(int k)  {
        fa[k]=0;
        fa[ls[k]]=fa[rs[k]]=k;
        sz[k]=sz[ls[k]]+sz[rs[k]]+1;
    }

    int merge(int x,int y)  {
        if (!x||!y) return x|y;
        int k;
        if (rnd[x]<rnd[y])
            rs[k=x]=merge(rs[x],y);
        else
            ls[k=y]=merge(x,ls[y]);
        return update(k),k;
    }

    void spilt(int k,int i,int &x,int &y) {
        if (!k) return x=y=0,void(0);
        if (compare(k,i))
            spilt(rs[x=k],i,rs[k],y);
        else
            spilt(ls[y=k],i,x,ls[k]);
        update(k);
    }

    void spilt_sz(int k,int s,int &x,int &y) {
        if (!s) return x=0,y=k,void(0);
        if (sz[ls[k]]<s) 
            spilt_sz(rs[x=k],s-sz[ls[k]]-1,rs[k],y);
        else 
            spilt_sz(ls[y=k],s,x,ls[k]);
        update(k);
    }

    string prefix(int k)    {
        string str;
        for (;k;--k)
            str+=char(s[k]+'A');
        return str;
    }

    void travel(int k=rt) {
        if (!k) return ;
        travel(ls[k]);
        cout<<k<<"("<<prefix(k)<<") ";
        travel(rs[k]);
    }

    void print() {
        cout<<"sa:"; travel(); cout<<endl;
        // for (int i=1;i<=n;++i) cout<<i<<":"<<ls[i]<<" "<<rs[i]<<endl;
    }

    inline int rk(int i)   {
        int res=1+sz[ls[i]];
        for (;fa[i];i=fa[i])
            if (isRightChild(i))
                res+=1+sz[ls[fa[i]]];
        return res;
    }

    inline int sa(int i)   {
        if (i<=0||i>n) return 0;

        int k=rt;
        while (1)   {
            if (i==sz[ls[k]]+1) return k;
            if (sz[ls[k]]>=i) 
                k=ls[k];
            else
                i-=sz[ls[k]]+1,k=rs[k];
        }
    }

    inline ULL half(ULL a,ULL b)   {
        return (a>>1)+(b>>1)+(a&b&1);
    }

    void push_front(int c)  {
        // cout<<"push_front:"<<c<<endl;
        s[++n]=c;
        h1[n]=(h1[n-1]*base1+c)%modu1;
        h2[n]=(h2[n-1]*base2+c)%modu2;

        int l,r,t;
        spilt(rt,n,l,r);

        ULL a=0,b=-1;
        // cout<<"l r:"<<l<<" "<<r<<endl;
        // cerr<<"a b:"<<a<<" "<<b<<endl;
        if (t=rightmost(l)) a=val[t];
        if (t=leftmost(r)) b=val[t];

        newnode(n,half(a,b));
        rt=merge(l,merge(n,r));
    }

    void pop_front()    {
        int s=rk(n);
        int l,mid,r;
        spilt_sz(rt,s,l,r);
        spilt_sz(l,s-1,l,mid);
        rt=merge(l,r);
        --n;
    }
}