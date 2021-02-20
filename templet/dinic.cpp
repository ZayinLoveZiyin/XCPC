#include<bits/stdc++.h>
#define maxn 5050
#define maxm 100050
using namespace std;
typedef long long LL;

const int lim=2500;

int id[maxn];

namespace Dinic {
    int n,tot,vs,vt;
    int head[maxn],edge[maxm],nxt[maxm];
    int cap[maxm];

    int cur[maxn];

    int d[maxn];

    void init(int _n) {
        n=_n+2;
        vs=_n+1,vt=vs+1;
        tot=0;
        for (int i=1;i<=n;++i) head[i]=-1;
    }

    void addedge(int u,int v,int w=1)   {
        edge[tot]=v; cap[tot]=w; nxt[tot]=head[u]; head[u]=tot++;
        edge[tot]=u; cap[tot]=0; nxt[tot]=head[v]; head[v]=tot++;
        assert(tot<maxm);
    }

    bool bfs()  {
        for (int i=1;i<=n;++i) cur[i]=head[i];
        for (int i=1;i<=n;++i) d[i]=-1;
        queue<int> q;
        q.push(vs),d[vs]=0;
        while (!q.empty())  {
            int i=q.front(); q.pop();
            for (int k=head[i],j;~k;k=nxt[k])
                if (cap[k]&&!~d[j=edge[k]])
                    q.push(j),d[j]=d[i]+1;
        }
        return ~d[vt];
    }

    int dfs(int i,int f)    {
        if (i==vt) return f;
        int sum=0;
        for (int &k=cur[i];~k;k=nxt[k]) {
            int j=edge[k];
            if (!cap[k]||d[j]!=d[i]+1) continue;
            int a=dfs(j,min(f,cap[k]));
            cap[k]-=a,cap[k^1]+=a;
            f-=a,sum+=a;
            if (!f) break;
        }
        if (!sum) d[i]=-1;
        return sum;
    }

    int maxflow()   {
        static int ans=0;
        while (bfs()) ans+=dfs(vs,1e9);
        return ans;
    }
}

LL n;

int main()  {
    scanf("%lld",&n);
    string s="1";
    Dinic::init(lim*2);
    for (int i=1;i<=lim&&i<=n;++i)  {
        Dinic::addedge(Dinic::vs,i);
        Dinic::addedge(lim+i,Dinic::vt);
        for (int j=1;j<=i;++j)
            if ((n-j+1)%i==0)
                Dinic::addedge(i,lim+j);
        for (int j=1;j<i;++j)
            if ((n-i+1)%j==0)
                Dinic::addedge(j,lim+i);
        if (Dinic::maxflow()==i)
            s=s+"1";
        else
            s=s+"0";
    }
}