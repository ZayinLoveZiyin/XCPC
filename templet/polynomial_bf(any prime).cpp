#include<bits/stdc++.h>
#define maxn 5000050
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
        operator bool() const {
            return size();
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
        const double PI=acos(-1);

        int N,rev[maxn];
        struct comp {
            double x,y;
            comp(double _x=0,double _y=0) : x(_x),y(_y) {
            }
            comp operator + (const comp& C) {   return comp(x+C.x,y+C.y);   }
            comp operator - (const comp& C) {   return comp(x-C.x,y-C.y);   }
            comp operator * (const comp& C) {   return comp(x*C.x-y*C.y,x*C.y+y*C.x);   }
            comp operator / (const double& l) {   return comp(x/l,y/l);   }
        } w[maxn];
        void init(int n) {
            for (N=1;N<n;N<<=1);
            assert(N<maxn);

            rev[0]=0,rev[1]=N>>1;
            for (int i=2;i<N;++i)
                rev[i]=rev[i>>1]>>1|rev[i&1];
            
            for (int i=0;i<N;++i)
                w[i]=comp(cos(2*PI*i/N),sin(2*PI*i/N));
        }
        void DFT(comp *A)   {
            for (int i=0;i<N;++i)
                if (i<rev[i])
                    swap(A[i],A[rev[i]]);
            for (int len=2;len<=N;len<<=1)  {
                int m=len>>1;
                for (comp *p=A;p!=A+N;p+=len)
                    for (int i=0;i<m;++i)   {
                        comp u=p[i],v=p[i+m]*w[N/len*i];
                        p[i]=u+v;
                        p[i+m]=u-v;
                    }
            }
        }
        void IDFT(comp *A)    {
            DFT(A);
            reverse(A+1,A+N);
            for (int i=0;i<N;++i)
                A[i]=A[i]/N;
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

    const int lim=100;
    poly operator * (const poly &a,const poly &b) {
        poly ans(a.size()+b.size());
        if (a.size()<lim&&b.size()<lim) {
            for (int i=0;i<a.size();++i)
                for (int j=0;j<b.size();++j)
                    add(ans[i+j],a[i]*b[j]);
            return ans.normalize();
        } else  {
            static const int w=1000;
            static FFT::comp ta[3][maxn],tb[3][maxn],tc[maxn];

            FFT::init(a.size()+b.size());
            auto init=[&](const poly& a,FFT::comp ta[3][maxn])   {
                for (int i=0;i<FFT::N;++i)
                    for (int c=0;c<3;++c)
                        ta[c][i]=0;
                for (int i=0;i<a.size();++i)    {
                    LL x=a[i];
                    for (int c=0;c<3;++c)
                        ta[c][i]=x%w,x/=w;
                }
                for (int c=0;c<3;++c)
                    FFT::DFT(ta[c]);
            };
            init(a,ta),init(b,tb);

            for (int ca=0;ca<3;++ca)
                for (int cb=0;cb<3;++cb)    {
                    LL cw=pw(w,ca+cb);
                    for (int i=0;i<FFT::N;++i) tc[i]=ta[ca][i]*tb[cb][i];
                    FFT::IDFT(tc);
                    for (int i=0;i<ans.size();++i)
                        add(ans[i],(LL)(tc[i].x+0.5)*cw);
                }

            return ans.normalize();
        }
        return ans.normalize();
    }

    poly operator / (poly a,const poly &b) {
        if (a.size()<b.size()) return poly{};

        poly ans;
        while (a.size()>=b.size())  {
            LL q=a.back()*pw(b.back())%modu;
            ans.push_back(q);
            for (int i=0;i<b.size();++i)
                sub(a[a.size()-i-1],b[b.size()-i-1]*q);
            assert(a.back()==0);
            a.pop_back();
        }
        reverse(ans.begin(),ans.end());
        return ans.normalize();
    }

    poly operator % (const poly &a,const poly &b) {
        return (a-(a/b)*b).normalize();
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
