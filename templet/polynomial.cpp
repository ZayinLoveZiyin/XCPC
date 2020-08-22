#include<bits/stdc++.h>
#define modu 998244353
using namespace std;
typedef long long LL;

void add(LL &a,LL b)    {   a=(a+b)%modu;   }
void sub(LL &a,LL b)    {   a=(a-b%modu+modu)%modu;  }
void mul(LL &a,LL b)    {   a=a*b%modu; }

LL pw(LL a,LL k=modu-2)   {
    LL ans=1;
    for (;k;k>>=1)  {
        if (k&1)
            ans=ans*a%modu;
        a=a*a%modu;
    }
    return ans;
}

namespace polynomial  {

    struct poly:vector<LL>  {
        using vector::vector;
        poly& normalize()   {
            while (size()&&!back())
                pop_back();
            return *this;
        }

        int deg() const  {   return size()-1;    }
        void init(int n) {
            resize(n);
            for (LL &x:*this)
                cin>>x;
        }
    };
    
    ostream& operator << (ostream& os,poly a)   {
        os<<"("; 
        for (LL x:a) os<<x<<" "; 
        os<<")";
        return os;
    }
    istream& operator >> (istream& is,poly& a)  {
        int n;
        is>>n;
        a.resize(n);
        for (LL &x:a) is>>x;
        return is;
    }

    namespace FFT   {
        #define maxn 1000050
        int N,rev[maxn];
        LL I,w[maxn];
        void init(int n) {
            for (N=1;N<n;N<<=1); I=pw(N);
            assert(N<maxn);

            rev[0]=0,rev[1]=N>>1;
            for (int i=2;i<N;++i)
                rev[i]=rev[i>>1]>>1|rev[i&1];
            
            w[0]=1,w[1]=pw(3,(modu-1)/N);
            for (int i=2;i<N;++i)
                w[i]=w[i-1]*w[1]%modu;
        }
        void DFT(LL *A)   {
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
                mul(A[i],I);
        }
    }

    inline poly operator + (const poly& a,const poly& b)   {
        poly ans(max(a.size(),b.size()));
        for (int i=0;i<ans.size();++i)  {
            if (i<a.size()) add(ans[i],a[i]);
            if (i<b.size()) add(ans[i],b[i]);
        }
        return ans;
    }
    inline poly operator - (const poly& a,const poly& b)   {
        poly ans(max(a.size(),b.size()));
        for (int i=0;i<ans.size();++i)  {
            if (i<a.size()) add(ans[i],a[i]);
            if (i<b.size()) sub(ans[i],b[i]);
        }
        return ans;
    }

    const int lim=100;
    inline poly operator * (poly a,poly b)   {
        if (a.size()<lim&&b.size()<lim) {
            poly ans(a.size()+b.size());
            for (int i=0;i<a.size();++i)
                for (int j=0;j<b.size();++j)
                    add(ans[i+j],a[i]*b[j]);
            return ans.normalize();
        } else  {
            FFT::init(a.size()+b.size());
            a.resize(FFT::N),FFT::DFT(&a[0]);
            b.resize(FFT::N),FFT::DFT(&b[0]);
            for (int i=0;i<FFT::N;++i)
                mul(a[i],b[i]);
            FFT::IDFT(&a[0]);
            return a.normalize();
        }
    }

    inline poly operator * (poly a,LL y)    {
        for (LL &x:a) mul(x,y);
        return a;
    }
    inline poly operator / (poly a,LL y)    {
        return a*pw(y);
    }
    inline poly operator % (poly a,int k)  {
        a.resize(k);
        return a;
    }

    inline poly operator ~ (poly a)    {//inv mod x^[a.size()+1]
        poly ans={pw(a[0])};
        for (int len=2;len<2*a.size();len<<=1) {
            poly ta=a%len;
            ans=(poly{2}-ans*ta%len)*ans;
            ans.resize(len);
        }
        return ans%a.size();
    }
    
    inline poly operator / (poly a,poly b)  {
        int len=a.size()-b.size()+1;
        if (len<=0) return poly{};
        reverse(a.begin(),a.end());
        reverse(b.begin(),b.end());
        b.resize(len);
        poly ans=a*(~b)%len;
        reverse(ans.begin(),ans.end());
        return ans;
    }
    
    inline poly operator % (poly a,poly b)  {
        return (a-a/b*b)%(b.size()-1);
    }

    inline poly sqrt(poly a) {//a0=1,2*len space
        poly ans={1};
        for (int len=2;len<2*a.size();len<<=1)  {
            poly ta=a%len;
            ans.resize(len);
            ans=(ans/2+ta/2*~ans)%len;
        }
        return ans%a.size();
    }

    inline poly d(poly a)   {
        poly ans(a.size()-1);
        for (int i=1;i<a.size();++i)
            ans[i-1]=a[i]*i%modu;
        return ans;
    }
    inline poly I(poly a)   {
        poly ans(a.size()+1);
        for (int i=0;i<a.size();++i)
            ans[i+1]=a[i]*pw(i+1)%modu;
        return ans;
    }

    inline poly ln(poly a)  {//a0=1
        return I(d(a)*~a)%a.size();
    }

    inline poly exp(poly a) {//a0=0
        poly ans={1};
        for (int len=2;len<2*a.size();len<<=1)  {
            poly ta=a%len;
            ans.resize(len);
            ans=ans*(poly{1}-ln(ans)+ta)%len;
        }
        return ans%a.size();
    }

    inline poly operator ^ (poly a,LL k)   {//a0!=0
        return exp(ln(a/a[0])*k)*pw(a[0],k);
    }

    inline poly operator << (poly a,int k)  {
        poly ans(a.size()+k);
        for (int i=0;i<a.size();++i)
            ans[i+k]=a[i];
        return ans;
    } 
    inline poly operator >> (poly a,int k)  {
        if (a.size()<=k) return poly{};
        poly ans(a.size()-k);
        for (int i=k;i<a.size();++i)
            ans[i-k]=a[i];
        return ans;
    } 
}

using namespace polynomial;

int main()  {
    poly a,b;
    int k;
    cin>>a>>b;
    cout<<a+b<<endl;
    cout<<a-b<<endl;
    cout<<a*b<<endl;
    cout<<a/b<<endl;
    cout<<a%b<<endl;
    cout<<(~a)<<endl;
    cout<<sqrt(a)<<endl;
    cout<<ln(a)<<endl;
    cout<<exp(a)<<endl;
    cout<<(a^k)<<endl;
}