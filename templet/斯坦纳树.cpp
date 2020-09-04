#include<bits/stdc++.h>
using namespace std;
#define fi first
#define se second

const int N=10005;

int n,m,d,inf;
int f[N][1<<8],g[1<<8];
bool vis[N];
deque<int> q;
vector<pair<int,int> > e[N];

bool chk(int msk) {
	for (int i=0;i<d;i++) if (((msk>>i)&1)^((msk>>d+i)&1)) return 0;
	return 1;
}

int main() {
	scanf("%d%d%d",&n,&m,&d);
	for (int i=1;i<=m;i++) {
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		e[x].push_back({y,z});
		e[y].push_back({x,z});
	}
	memset(f,0x3f,sizeof(f));
	memset(g,0x3f,sizeof(g));
	inf=f[0][0];
	for (int i=1;i<=d;++i) f[i][1<<i-1]=0,f[n-i+1][1<<d+i-1]=0,vis[i]=1;
	for (int msk=0;msk<1<<d+d;msk++) {
		for (int i=1;i<=n;i++) for (int s=msk&(msk-1);s;s=(s-1)&msk)
			f[i][msk]=min(f[i][msk],f[i][s]+f[i][msk^s]);
		for (int i=1;i<=n;++i) if (f[i][msk]<inf) q.push_back(i);
		while (!q.empty()) {
	        int h=q.front();q.pop_front();
	        vis[h]=0;
	        for (auto v:e[h]) if (f[v.fi][msk]>f[h][msk]+v.se) {
				f[v.fi][msk]=f[h][msk]+v.se;
				if (!vis[v.fi]) {
					vis[v.fi]=1;
					if (!q.empty() && f[v.fi][msk]<=f[q.front()][msk]) q.push_front(v.fi);
					else q.push_back(v.fi);
				}
			}
		}
		for (int i=1;i<=n;i++) g[msk]=min(g[msk],f[i][msk]);
	}
	for (int msk=1;msk<1<<d+d;msk++) for (int s=msk&(msk-1);s;s=(s-1)&msk) {
		if (chk(s) && chk(msk^s)) g[msk]=min(g[msk],g[s]+g[msk^s]);
	}
	if (g[(1<<d+d)-1]<inf) printf("%d\n",g[(1<<d+d)-1]);
	else puts("-1");
}
