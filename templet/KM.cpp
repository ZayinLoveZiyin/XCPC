#include<bits/stdc++.h>
#define maxn 405
#define inf 0x3f3f3f3f
using namespace std;
typedef long long LL;

int nl,nr,n,m;
int g[maxn][maxn];

int q[maxn],l,r;

bool Vx[maxn],Vy[maxn];

int slack[maxn];
int lx[maxn],ly[maxn];

int pre[maxn];
int S[maxn],T[maxn];

void init()
{
	memset(lx,0x80,sizeof(lx));
	memset(ly,0,sizeof(ly));
	scanf("%d%d%d",&nl,&n,&m);
	nr=max(nl,n);
	while (m--)
	{
		int u,v;
		scanf("%d%d",&u,&v);
		scanf("%d",&g[u][v]);
		lx[u]=max(lx[u],g[u][v]);
	}
}

void adjust()
{
	int d=inf;
	for (int j=1;j<=nr;++j)
		if (!Vy[j])
			d=min(d,slack[j]);
	for (int i=1;i<=nl;++i)
		if (Vx[i])
			lx[i]-=d;
	for (int j=1;j<=nr;++j)
		if (Vy[j])
			ly[j]+=d;
		else
			slack[j]-=d;
}

int bfs(int i)
{
	memset(slack,0x3f,sizeof(slack));
	memset(Vx,0,sizeof(Vx));
	memset(Vy,0,sizeof(Vy));
	l=r=0;
	q[r++]=i;
	while (1)
	{
		while (l<r)
		{
			int i=q[l++];
			Vx[i]=1;
			for (int j=1;j<=nr;++j)
			{
				int d=lx[i]+ly[j]-g[i][j];
				if (Vy[j]||d>slack[j])	continue;
				pre[j]=i;
				if (!d)
				{
					if (!T[j])	return j;
					Vy[j]=1;
					q[r++]=T[j];
				} else slack[j]=d;
			}
		}
		adjust();
		for (int j=1;j<=nr;++j)
			if (!Vy[j]&&!slack[j])
			{
				if (!T[j])	return j;
				Vy[j]=1;
				q[r++]=T[j];
			}
	}
}

void solve()
{
	for (int i=1;i<=nl;++i)
	{
		int j=bfs(i);
		while (j)	
			swap(j,S[T[j]=pre[j]]);
	}
	
	LL ans=0;
	for (int i=1;i<=nl;++i)
		ans+=g[i][S[i]];
	printf("%lld\n",ans);
	for (int i=1;i<=nl;++i)
		printf("%d ",g[i][S[i]]?S[i]:0);
}

int main()
{
	init();
	solve();
	return 0;
}