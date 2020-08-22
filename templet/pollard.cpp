#include<bits/stdc++.h>
using namespace std;
typedef long long LL;

LL R()  {
    return rand()<<15|rand();
}

LL multi(LL a,LL b,LL M)    {
    LL ans=0;
    for (;b;b>>=1)  {
        if (b&1)
            ans=(ans+a)%M;
        a=(a<<1)%M;
    }
    return ans;
}

LL pw(LL a,LL k,LL M)   {
    LL ans=1;
    for (;k;k>>=1)  {
        if (k&1)
            ans=multi(ans,a,M);
        a=multi(a,a,M);
    }
    return ans;
}

bool MillerRabin(LL n)  {
    if (~n&1)   return 0;
    LL m=n-1,c=0;
    for (;~m&1;m>>=1)   ++c;
    for (int t=1;t<=6;++t)  {
        LL x=pw(R()%(n-1)+1,m,n),y;
        for (int j=0;j<c;x=y,++j)   {
            y=multi(x,x,n);
            if (y==1&&x!=1&&x!=n-1)   return 0;
        }
        if (y!=1)   return 0;
    }
    return 1;
}
LL Pollard(LL n)    {
    
}

int main()  {
    return 0;
}