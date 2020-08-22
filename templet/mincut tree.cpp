#include<bits/stdc++.h>
#define maxb 705
#define maxn 10050
#define maxm 70050
using namespace std;

int n,id[maxb][maxb];
char mp[maxb][maxb];

int dir[5][2];
//const int dir[6][2]={{-2,0},{2,0},{-1,-3},{-1,3},{1,-3},{1,3}};

int ID(int i,int j) {
    if (!id[i][j])
        id[i][j]=++n;
    return id[i][j];
}

int tot;
int cost[maxm<<1];
int head[maxn],edge[maxm<<1],nxt[maxm<<1];

void join(int u,int v)  {
    // cout<<u<<" -> "<<v<<endl;
    edge[tot]=v; nxt[tot]=head[u]; head[u]=tot++;
    edge[tot]=u; nxt[tot]=head[v]; head[v]=tot++;
}

void init() {
    tot=0;
    memset(head,-1,sizeof(head));
}

int vs,vt;

int d[maxn];
queue<int> q;

bool bfs()  {
    for (int i=1;i<=vt;++i)  d[i]=-1;
    q.push(vs),d[vs]=0;
    while (!q.empty())  {
        int i=q.front();    q.pop();
        for (int k=head[i],j;~k;k=nxt[k]) {
            if (cost[k]&&d[j=edge[k]]==-1)  {
                q.push(j),d[j]=d[i]+1;
            }
        }
    }
    return ~d[vt];
}

int dfs(int i,int f)    {
    if (i==vt)  return f;
    int sum=0;
    for (int k=head[i];~k;k=nxt[k]) {
        int j=edge[k];
        if (!cost[k]||d[j]!=d[i]+1) continue;
        int a=dfs(j,min(f,cost[k]));
        cost[k]-=a,cost[k^1]+=a;
        f-=a,sum+=a;
        if (!f) break;
    }
    if (!sum)    d[i]=-1;
    return sum;
}

int Dinic()    {
    for (int k=0;k<tot;++k) cost[k]=1;
    int ans=0;
    while (bfs())   ans+=dfs(vs,maxn);
    return ans;
}

int fa[maxn];
int cut[maxn][maxn];

bool vis[maxn];

void dfs(int i) {
    vis[i]=1;
    for (int k=head[i],j;~k;k=nxt[k])
        if (cost[k]&&!vis[j=edge[k]])
            dfs(j);
}

void solve()    {
    assert(n<=3000);

    int ans=0;
    for (int i=1;i<=n;++i)  fa[i]=1;
    for (int i=1;i<=n;++i)
        for (int j=1;j<=n;++j)
            cut[i][j]=maxn;
    for (int i=2;i<=n;++i)  {
        vs=i,vt=fa[i];
        int C=Dinic();
        for (int j=1;j<i;++j)
            ans+=(cut[i][j]=cut[j][i]=min(cut[fa[i]][j],C));
        
        for (int j=1;j<=n;++j)  vis[j]=0;
        dfs(i);
        for (int j=i+1;j<=n;++j)
            if (vis[j]&&fa[j]==fa[i])
                fa[j]=i;
    }
    printf("%d\n",ans);
}

int main()  {
    int T,icase=0;
    scanf("%d",&T);
    while (T--) {
        init();
        printf("Case #%d: ",++icase);
        solve();
    }
    return 0;
}