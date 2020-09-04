#include<stdio.h>
#include<algorithm>
#include<cstring>
#define co(a,b) f[gf(a)]=gf(b)
#define cint const int &
#define clr(a) memset(a,0,sizeof(a));
#define N 505

using namespace std;

int Q[N],s[N],f[N],ne[N],V[N],ma[N],mat[N],n,m,ans,tot,t,l,r;

struct edge{int v,n;}e[N*N];
inline void push(cint u,cint v){e[++tot]=(edge){v,s[u]};s[u]=tot;}

int gf(cint x){return f[x]==x?x:f[x]=gf(f[x]);}

inline int lca(int x,int y)
{
	for (t++;;swap(x,y)) if (x)
	{
		x=gf(x);
		if (V[x]==t) return x;
		V[x]=t;
		x=ne[mat[x]];
	}
}

inline void g(int a,cint p)
{
	while (a!=p)
	{
		int b=mat[a],c=ne[b];
		if (gf(c)!=p) ne[c]=b;
		if (ma[b]==2) ma[Q[++r]=b]=1;
		co(a,b);co(b,c);a=c;
	}
}

inline void find(cint st,cint n)
{
	for (int i=1;i<=n;i++) f[i]=i;clr(ma);
	ma[Q[1]=st]=1;
	for (l=r=1;l<=r;l++)
	{
		int u=Q[l];
		for (int i=s[u];i;i=e[i].n)
		{
			int v=e[i].v;
			if (mat[u]==v||gf(u)==gf(v)||ma[v]==2) continue;
			if (ma[v]==1)
			{
				int tmp=lca(u,v);
				if (gf(u)!=tmp) ne[u]=v;
				if (gf(v)!=tmp) ne[v]=u;
				g(u,tmp);g(v,tmp);
			}
			else if (!mat[v])
			{
				ne[v]=u;
				for (int k=v,x,y;k;k=y) y=mat[x=ne[k]],mat[k]=x,mat[x]=k;
				return;
			}
			else ne[v]=u,ma[v]=2,ma[Q[++r]=mat[v]]=1;
		}
	}
}

inline void prepare()
{
	ans=tot=t=0;
	memset(s,0,sizeof(s));
	memset(mat,0,sizeof(mat));
	clr(V);
}

int main()
{
	scanf("%d%d",&n,&m);
	for (int u,v;m--;) scanf("%d%d",&u,&v),push(u,v),push(v,u);
	for (int i=1;i<=n;i++) if (!mat[i]) find(i,n);
	for (int i=1;i<=n;i++) if (mat[i]) ans++;
	printf("%d\n",ans>>1);
	for (int i=1;i<=n;i++) printf("%d ",mat[i]);
}
