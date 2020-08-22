//similar eculid
struct Data	{
	bint f,g,h;
	Data (bint _f=0,bint _g=0,bint _h=0)	{   f=_f,g=_g,h=_h; }
};

bint S0(bint n)	{	return n+1;	}
bint S1(bint n)	{	return n*(n+1)/2;	}
bint S2(bint n)	{	return n*(n+1)*(2*n+1)/6;	}
bint sq(bint n)	{	return n*n;	}
//sigma (ax+b)/c x=0...n
Data eucilid(bint a,bint b,bint c,bint n)	{
	if (!a)	return Data(b/c*S0(n),b/c*S1(n),sq(b/c)*S0(n));
	if (a>=c||b>=c)	{
		Data t=eucilid(a%c,b%c,c,n);
		a/=c,b/=c;
		return Data(t.f + a*S1(n) + b*S0(n),
					t.g + a*S2(n) + b*S1(n),
					t.h + 2*a*t.g + 2*b*t.f + 2*a*b*S1(n) + sq(a)*S2(n) + sq(b)*S0(n));
	} else	{
		bint m=(a*n+b)/c;
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

//新数论分块
for (LL i=1,j;i<=n;i=j+1)	{
    LL a=n/i;   j=n/a;
    for (LL l=i,r;l<=j;l=r+1)	{
        assert(l);
        if (n%l==0)	{
            assert(l==j);
            break;
        } else	{// floor(n/i)=a,ceil(i/(n%i))=c
            LL c=(l-1)/(n%l)+1;
            r=((bint)c*n)/((bint)a*c+1);
        }
    }
}

//杜教筛
LL ans=sigma f·g(i);
for (LL i=2,j,t;i<=n;i=j+1)	{
	j=n/(t=n/i);
	ans-=(g(j)-g(i-1))*calc(t);
}

//min25
//f[n] func(i),i is prime && i<=n
for (LL i=1,j,t;i<=n;i=j+1)	{
	j=n/(t=n/i);w[++m]=t;
	if (w[m]<maxn)
		id1[w[m]]=m;
	else
		id2[j]=m;
	f[m]=S1(t)-1;
	g[m]=S2(t)-1;
}

for (int j=1;j<=pnum;++j)
	for (int i=1;i<=m&&p[j]*p[j]<=w[i];++i)	{
		int k=id(w[i]/p[j]);
		f[i]-=f[k]-j+1;
		g[i]-=p[j]*(g[k]-sp[j-1]);
	}

LL S(LL x,int y) {
	if(x<=1||p[y]>x) return 0;
	LL k=id(x),ret=(g[k]-sp[y-1]+f[k]-y+1+modu*2)%modu;
	for(int i=y;i<=pnum && (LL)p[i]*p[i]<=x;++i)	{
		LL t1=p[i],t2=(LL)p[i]*p[i];
		for(int e=1;t2<=x;++e,t1=t2,t2*=p[i])
			ret=(ret+(t1+1)%modu*S(x/t1,i+1)+(t2+1))%modu;
	}
	return ret;
}

//gauss
for (int i=0;i<n;++i)   {
	int k=i;
	for (int j=i;j<n;++j)
		if (abs(a[j][i])>abs(a[k][i]))
			k=j;
	for (int j=i;j<=n;++j)
		swap(a[k][j],a[i][j]);
	assert(abs(a[i][i])>0);
	for (int k=i+1;k<n;++k)   {
		double q=a[k][i]/a[i][i];
		for (int j=i;j<=n;++j)
			a[k][j]-=a[i][j]*q;
	}
}

for (int i=n-1;i>=0;--i)    {
	ans[i]=a[i][n];
	for (int j=i+1;j<n;++j)
		ans[i]-=a[i][j]*ans[j];
	ans[i]/=a[i][i];
}


//sigma floor(n/i)
namespace ds {
	namespace stac {
		const int N = 100005;
		uint qu[N][2]; int qr;
		void pop () { qr --; }
		void push (uint x, uint y) { qr ++; qu[qr][0] = x; qu[qr][1] = y; }
		void top (uint &x, uint &y) { x = qu[qr][0]; y = qu[qr][1]; }
	}
	using stac :: push;
	using stac :: pop;
	using stac :: top;
 
	uLL solve (ull n) {
		uLL ret = 0;
		ull w = pow (n, 0.35), v = sqrtl (n), x, y;
		uint dx, dy, ux, uy, mx, my;
		while (v * v <= n) v ++; while (v * v > n) v --;
		x = n / v, y = n / x + 1;
		push (1, 0); push (1, 1);
		auto outside = [&] (ull x, ull y) { return x * y > n; };
		auto cut_off = [&] (ull x, uint dx, uint dy) { return (uLL)x * x * dy >= (uLL)n * dx; };
		while (stac :: qr) {
			top (dx, dy);
			while (outside (x + dx, y - dy)) {
				ret += x * dy + ull(dy + 1) * (dx - 1) / 2;
				x += dx, y -= dy;
			}
			if (y <= w) break;
			while (true) {
				pop (), ux = dx, uy = dy, top (dx, dy);
				if (outside (x + dx, y - dy)) break;
			}
			while (true) {
				mx = ux + dx, my = uy + dy;
				if (!outside (x + mx, y - my)) {
					if (cut_off (x + mx, dx, dy)) break;
					ux = mx, uy = my;
				} else push (dx = mx, dy = my);
			}
		}
		for (y --; y; y --) ret += n / y;
		return stac :: qr = 0, ret * 2 - v * v;
	}
}
 