#include<bits/stdc++.h>
#define maxn 500050
#define maxm 5000050
using namespace std;
typedef long long LL;

const int N=500005;

const LL inf=1e18;

//must guarantee no negative circle(dijk)
namespace MCMF  {
    int tot,vs,vt;
    LL cap[maxm],cost[maxm];
    int head[maxn],edge[maxm],nxt[maxm];

    void init() {
        tot=0;
        memset(head,-1,sizeof(head));
    }

    void addedge(int u,int v,LL c,LL w) {
        // cout<<u<<" -> "<<v<<" "<<w<<endl;
        edge[tot]=v; cap[tot]=c; cost[tot]=+w; nxt[tot]=head[u]; head[u]=tot++;
        edge[tot]=u; cap[tot]=0; cost[tot]=-w; nxt[tot]=head[v]; head[v]=tot++;
    }

    int prt[maxn];
    LL h[maxn],dis[maxn],flow[maxn];

    bool dijk() {
        using pr=pair<LL,int>;
        priority_queue<pr,vector<pr>,greater<pr> > q;

        for (int i=1;i<=vt;++i) dis[i]=inf,prt[i]=0;
        flow[vs]=inf,q.emplace(dis[vs]=0,vs);
        while (q.size())  {
            auto p=q.top(); q.pop();
            if (dis[p.second]<p.first) continue;
            int i=p.second;
            for (int k=head[i];~k;k=nxt[k]) {
                int j=edge[k];
                if (!cap[k]||dis[j]<=dis[i]+cost[k]) continue;
                prt[j]=k;
                dis[j]=dis[i]+cost[k];
                flow[j]=min(flow[i],cap[k]);
                q.emplace(dis[j],j);
            }
        }
        return dis[vt]<inf;
    }

    pair<LL,LL> MCMF()   {
        for (int i=1;i<=vt;++i) h[i]=0;

        LL F=0,C=0;
        auto normalize=[&]()  {
            for (int i=1;i<=vt;++i) h[i]+=dis[i];
            for (int i=1;i<=vt;++i) 
                for (int k=head[i];~k;k=nxt[k]) {
                    cost[k]+=dis[i]-dis[edge[k]];
                    assert(!cap[k]||cost[k]>=0);
                }
        };
        auto adjust=[&]()   {
            LL a=flow[vt];
            F+=a,C+=a*h[vt];
            for (int i=vt;i!=vs;i=edge[prt[i]^1]) {
                C+=a*cost[prt[i]];
                cap[prt[i]]-=a,cap[prt[i]^1]+=a;
            }
        };

        //first time to calc the shortest path from vs
        // dijk();
        for (int i=1;i<=vt;++i)
            for (int k=head[i];~k;k=nxt[k])
                if (cap[k])
                    dis[edge[k]]=min(dis[edge[k]],dis[i]+cost[k]);
        normalize();

        // cout<<"???"<<endl;

        while (dijk())  {
            adjust();
            normalize();
        }
        return make_pair(F,C);
    }
}

int main()  {
    MCMF::init();
    int q;
    scanf("%d",&q);
    while (q--) {
        int u,v;
        LL w;
        scanf("%d%d%lld",&u,&v,&w);
        MCMF::addedge(u,v+1,1,-w);
    }
    for (int i=1;i<N;++i) MCMF::addedge(i,i+1,2,0);
    MCMF::vs=1,MCMF::vt=N;
    printf("%lld\n",-MCMF::MCMF().second);
}