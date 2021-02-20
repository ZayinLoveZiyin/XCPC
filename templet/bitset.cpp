#include<bits/stdc++.h>
using namespace std;
typedef long long LL;

struct Bits	{
    static const int w=4;
    static const LL all=(1LL<<w)-1;
	const int n,N;
	vector<LL> s;
    Bits(int n):n(n),N((n-1)/w+1)    {
		s.resize(N);
    }
	friend ostream& operator << (ostream& os,const Bits& B)	{
		for (int i=0;i<B.n;++i) os<<B[i];
		return os;
	}

	int operator [](const int& p) const {	return s[p/w]>>(p%w)&1;	}
	
    void set(int p) {   s[p/w]|=1LL<<(p%w); }
    void reset(int p) {   s[p/w]&=~(1LL<<(p%w)); }
    int count() {
        int cnt=0;
        for (int i=0;i<N;++i)
            cnt+=__builtin_popcountll(s[i]);
        return cnt;
    }

    Bits operator - (const Bits& A) const   {
        Bits B(n);
        for (int i=0;i<N;++i)   B.s[i]=s[i]-A.s[i];
        for (int i=0;i<N-1;++i)
            if (B[i]<0)
                B.s[i]+=1LL<<w,--B.s[i+1];
        return B;
    }
    Bits operator | (const Bits& A) const   {
        Bits B(n);
        for (int i=0;i<N;++i)
            B.s[i]=s[i]|A.s[i];
        return B;
    }
    Bits operator & (const Bits& A) const   {
        Bits B(n);
        for (int i=0;i<N;++i)
            B.s[i]=s[i]&A.s[i];
        return B;
    }
    Bits operator ^ (const Bits& A) const   {
        Bits B(n);
        for (int i=0;i<N;++i)
            B.s[i]=s[i]^A.s[i];
        return B;
    }
    Bits operator >> (int x) const   {
        Bits B(n);
		for (int d=x/w,i=d;i<N;++i)
			B.s[i-d]=s[i];
		x%=w;
		LL o=0,mask=(1LL<<x)-1;
        for (int i=N-1;i>=0;--i)	{
			LL t=B.s[i]&mask;
			B.s[i]=o<<(w-x)|(B.s[i]>>x);
			o=t;
		}
        return B;
    }
    Bits operator << (int x) const   {
        Bits B(n);
		for (int d=x/w,i=d;i<N;++i)
			B.s[i]=s[i-d];
		x%=w;
		LL o=0,mask=(1LL<<x)-1;
        for (int i=0;i<N;++i)	{
			LL t=B.s[i]>>(w-x);
			B.s[i]=(B.s[i]<<x|o)&all;
			o=t;
		}
        return B;
    }
	Bits flip() const	{
        Bits B(n);
		for (int i=0;i<N;++i)
			B.s[i]=s[i]^all;
		return B;
	}
	Bits rev() const	{
		auto revb=[&](LL x) {
			x = (x >> 32) | (x << 32);
			x = ((x & 0xffff0000ffff0000) >> 16) | ((x & 0x0000ffff0000ffff) << 16);
			x = ((x & 0xff00ff00ff00ff00) >> 8) | ((x & 0x00ff00ff00ff00ff) << 8);
			x = ((x & 0xf0f0f0f0f0f0f0f0) >> 4) | ((x & 0x0f0f0f0f0f0f0f0f) << 4);
			x = ((x & 0xcccccccccccccccc) >> 2) | ((x & 0x3333333333333333) << 2);
			x = ((x & 0xaaaaaaaaaaaaaaaa) >> 1) | ((x & 0x5555555555555555) << 1);
			return x;
		};
        Bits B(n);
		for (int i=0;i<N;++i)
			B.s[i]=revb(s[N-1-i])>>(64-w);
        return B>>((w-n%w)%w);
	}

    int findFirst() {
		for (int i=0;i<N;++i)
			if (s[i])
                return __builtin_ctzll(s[i])+i*w;
        return -1;
    }

	Bits(Bits&& B):n(B.n),N(B.N),s(B.s)	{
	}
	Bits& operator = (const Bits& B)	{
		s=B.s;
	}
};