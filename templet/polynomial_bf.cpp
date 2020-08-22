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

    poly operator * (const poly &a,const poly &b) {
        poly ans(a.size()+b.size());
        for (int i=0;i<a.size();++i)
            for (int j=0;j<b.size();++j)
                add(ans[i+j],a[i]*b[j]);
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