#include<bits/stdc++.h>
#define maxn 10050
#define maxc 26
using namespace std;
typedef long long LL;

struct Bits
{
    static const int w=62;
    LL N,s[maxn];

    Bits(int n=maxn)    {
        N=n;
        memset(s,0,sizeof(s));
    }
    void set(int p) {   s[p/w]|=1LL<<(p%w); }
    int count() {
        int cnt=0;
        for (int i=0;i<N;++i)
            cnt+=__builtin_popcountll(s[i]);
        return cnt;
    }

    Bits operator - (const Bits& A) const   {
        Bits B;
        for (int i=0;i<N;++i)   B.s[i]=s[i]-A.s[i];
        for (int i=0;i<N-1;++i)
            if (B.s[i]<0)
                B.s[i]+=1LL<<w,--B.s[i+1];
        return B;
    }
    Bits operator | (const Bits& A) const   {
        Bits B;
        for (int i=0;i<N;++i)
            B.s[i]=s[i]|A.s[i];
        return B;
    }
    Bits operator & (const Bits& A) const   {
        Bits B;
        for (int i=0;i<N;++i)
            B.s[i]=s[i]&A.s[i];
        return B;
    }
    Bits operator ^ (const Bits& A) const   {
        Bits B;
        for (int i=0;i<N;++i)
            B.s[i]=s[i]^A.s[i];
        return B;
    }
    Bits shl()  {
        Bits B=*this;
        LL o=1;
        for (int i=0;i<N;++i)   {
            int t=B.s[i]>>(w-1)&1;
            B.s[i]=(B.s[i]<<1|o)&(~(1LL<<w)),o=t;
        }
        return B;
    }
};

ostream& operator << (ostream& os,const Bits& B)    {
    for (int i=0;i<B.N;++i)
        for (int j=0;j<Bits::w;++j)
            os<<(B.s[i]>>j&1);
    return os;
}

int n,m;
char s[maxn],t[maxn];

Bits cur[maxn];

void init()
{
    scanf("%d%d",&n,&m);
    scanf("%s%s",s,t);
    for (int i=0;t[i];++i)
        cur[t[i]-'A'].set(i);
}

Bits dp[2];

void solve()
{
    int now=0;
    for (int i=0;i<n;++i)
    {
        now^=1;
        Bits x=dp[now^1]|cur[s[i]-'A'];
        dp[now]=x&((x-dp[now^1].shl())^x);
    }
    printf("%d\n",dp[now].count());
}

int main()
{
    init();
    solve();
    return 0;
}