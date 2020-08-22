#include<bits/stdc++.h>
#define maxn 200050
#define g 3
#define P 998244353
using namespace std;
typedef long long LL;

LL pw(LL a,LL k=P-2) {
    LL ans=1;
    for (;k;k>>=1) {
        if (k&1)
            ans=ans*a%P;
        a=a*a%P;
    }
    return ans;
}

struct NTT
{
    //must guarantee that n is 2^k.
    LL t[maxn];
    void DFT(LL *A,int n)   
    {
        for (int i=1,j=0;i<n;++i) {
            for (int s=n>>1;j^=s,j<s;s>>=1);
            if (i<j)
                swap(A[i],A[j]);
        }
        for (int len=2;len<=n;len<<=1) 
        {
            int m=len>>1,_w=pw(g,(P-1)/len);
            for (int i=0,w=1;i<m;++i,w=(LL)w*_w%P)
                for (LL *p=A;p!=A+n;p+=len) 
                {
                    LL u=p[i],v=p[i+m]*w%P;
                    p[i]=(u+v)%P;
                    p[i+m]=(u-v+P)%P;
                }
        }
    }

    void IDFT(LL *A,int n)  
    {
        DFT(A,n);
        reverse(A+1,A+n);
        for (int i=0,I=pw(n);i<n;++i)
            A[i]=A[i]*I%P;
    }

    void Inv(LL *A,LL *B,int n)
    {
        if (n==1)   {
            B[0]=pw(A[0]);
            return;
        }
        Inv(A,B,n>>1);
        int N=n<<1;
        for (int i=0;i<n;++i)   t[i]=A[i];
        for (int i=n;i<N;++i)   t[i]=B[i]=0;
        DFT(t,N),DFT(B,N);
        for (int i=0;i<N;++i)
            B[i]=B[i]*(2-t[i]*B[i]%P+P)%P;
        IDFT(B,N);
        for (int i=n;i<N;++i)   B[i]=0;
    }
}   T;

int main()
{
    return 0;
}