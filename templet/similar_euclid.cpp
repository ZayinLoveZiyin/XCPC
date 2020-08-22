#include<bits/stdc++.h>
using namespace std;
typedef long long LL;

struct Data	{
	LL f,g,h;
	Data (LL _f=0,LL _g=0,LL _h=0)	{
		f=_f,g=_g,h=_h;
	}
	bool operator == (const Data& D)	const	{
		return f==D.f&&g==D.g&&h==D.h;
	}
};

LL S0(LL n)	{	return n+1;	}
LL S1(LL n)	{	return n*(n+1)/2;	}
LL S2(LL n)	{	return n*(n+1)*(2*n+1)/6;	}
LL sq(LL n)	{	return n*n;	}
//sigma (ax+b)/c
Data eucilid(LL a,LL b,LL c,LL n)	{
	if (!a)	return Data(b/c*S0(n),b/c*S1(n),sq(b/c)*S0(n));
	if (a>=c||b>=c)	{
		Data t=eucilid(a%c,b%c,c,n);
		a/=c,b/=c;
		return Data(t.f + a*S1(n) + b*S0(n),
					t.g + a*S2(n) + b*S1(n),
					t.h + 2*a*t.g + 2*b*t.f + 2*a*b*S1(n) + sq(a)*S2(n) + sq(b)*S0(n));
	} else	{
		LL m=(a*n+b)/c;
		Data t=eucilid(c,c-b-1,a,m-1);
		return Data(n*m-t.f,
					(m*n*(n+1)-t.f-t.h)/2,
					n*m*m-t.f-2*t.g);
	}
}

//sigma k*x%m s.t. k*x%m in [l,r)
LL calc(int k,int n,int m,int l,int r)	{
	Data L=eucilid(k,m-l,m,n),R=eucilid(k,m-r,m,n);
	return k*(L.g-R.g)-m*((L.h-R.h)-(L.f-R.f))/2;
}

int main()	{
	return 0;
}