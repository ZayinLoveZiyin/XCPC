#include<bits/stdc++.h>
#define maxn 200050
#define modu 998244353
using namespace std;
typedef long long LL;

LL pw(LL a,LL k=modu-2) {
    LL ans=1;
    for (;k;k>>=1) {
        if (k&1)
            ans=ans*a%modu;
        a=a*a%modu;
    }
    return ans;
}

namespace NTT   {
    int rev[maxn],N;
    LL w[maxn],I;
    void init(int n) {
        for (N=1;N<=n;N<<=1); I=pw(N);
        
        rev[0]=0,rev[1]=N>>1;
        for (int i=2;i<N;++i) rev[i]=rev[i>>1]>>1|rev[i&1];

        w[0]=1,w[1]=pw(3,(modu-1)/N);
        for (int i=2;i<N;++i) w[i]=w[i-1]*w[1]%modu;
    }

    void DFT(LL *A) {
        for (int i=0;i<N;++i)
            if (i<rev[i])
                swap(A[i],A[rev[i]]);
        
        for (int len=2;len<=N;len<<=1)  {
            int m=len>>1;
            for (LL *p=A;p!=A+N;p+=len)
                for (int i=0;i<m;++i)   {
                    LL u=p[i],v=p[i+m]*w[N/len*i]%modu;
                    p[i]=(u+v)%modu;
                    p[i+m]=(u-v+modu)%modu;
                }
        }
    }

    void IDFT(LL *A)    {
        DFT(A);
        reverse(A+1,A+N);
        for (int i=0;i<N;++i)
            A[i]=A[i]*I%modu;
    }

    LL TA[maxn],TB[maxn],TC[maxn];
    void convolute(LL *A,LL *B,LL *C)   {
        for (int i=0;i<N;++i) TA[i]=TB[i]=0;
        for (int i=0;i<n;++i) TA[i]=A[i];
        for (int i=0;i<n;++i) TB[i]=B[i];
        NTT::DFT(TA);
        NTT::DFT(TB);
        for (int i=0;i<N;++i) TC[i]=TA[i]*TB[i]%modu;
        NTT::IDFT(TC);

        for (int i=0;i<n;++i)   {
            C[i]=0;
            for (int j=i;j<N;j+=n)
                C[i]=(C[i]+TC[j])%modu;
        }
    }
}

int main()
{
    return 0;
}