#include<bits/stdc++.h>
#define maxn 100050
using namespace std;
typedef long long LL;

const int bufsize=1<<20;
int buflen;
char buf[bufsize],*bufptr=buf;
inline char getc() {
    if (bufptr==buf+buflen) buflen=fread(bufptr=buf,1,maxn,stdin);
    if (bufptr==buf+buflen) assert(0);
    return *bufptr++;
}
LL read()   {
    LL x=0;
    char c=getchar();
    while (!('0'<=c&&c<='9')) c=getchar();
    while ('0'<=c&&c<='9') x=(x<<3)+(x<<1)+c-'0',c=getchar();
    return x;
}

const LL inf=1e15;

int n,m,rt;

namespace LCT   {//take care of node 0
    #define ls(x) ch[x][0]
    #define rs(x) ch[x][1]
    #define si(x) (si[x].top())

    int tag[maxn];
    LL val[maxn],mn[maxn]={inf},mt[maxn]={inf},mi[maxn]={inf};

    struct Heap {
        priority_queue<LL> p,q;
        void insert(LL x)  {
            q.push(-x);
        }
        void erase(LL x)   {
            p.push(-x);
        }
        LL top()  {
            while (p.size()&&q.size()&&p.top()==q.top()) p.pop(),q.pop();
            return q.size()?-q.top():inf;
        }
    } si[maxn];
    int ch[maxn][2],fa[maxn],rev[maxn];

    void init(int x) {
        scanf("%lld",val+x),mn[x]=mt[x]=val[x];
        mi[x]=inf;
        fa[x]=ls(x)=rs(x)=rev[x]=tag[x]=0;
    }

    int get(int x)  {   return ch[fa[x]][1]==x; }
    int isroot(int x)   {   return !(ch[fa[x]][0]==x||ch[fa[x]][1]==x); }

    void makerev(int x) {
        if (!x) return ;
        swap(ls(x),rs(x));
        rev[x]^=1;
    }

    void update(int x)  {
        mt[x]=min(min(mt[ls(x)],mt[rs(x)]),val[x]);
        mi[x]=min(min(mi[ls(x)],mi[rs(x)]),si(x));
        mn[x]=min(mt[x],mi[x]);
    }

    void renew(int x,LL d) {
        if (!x) return ;
        val[x]=tag[x]=mt[x]=d;
        mn[x]=min(d,mi[x]);
    }

    void godown(int x)  {
        if (rev[x])
            makerev(ls(x)),makerev(rs(x)),rev[x]=0;
        if (tag[x])
            renew(ls(x),tag[x]),renew(rs(x),tag[x]),tag[x]=0;
    }


    void rotate(int x)  {
        int y=fa[x],z=fa[y],c=get(x);
        ch[y][c]=ch[x][c^1];
        if (ch[x][c^1]) fa[ch[x][c^1]]=y;
        if (!isroot(y)) ch[z][get(y)]=x;
        ch[x][c^1]=y,fa[x]=z,fa[y]=x;
        update(y),update(x);
    }

    void refresh(int x)    {
        if (!isroot(x)) refresh(fa[x]);
        godown(x);
    }
    void splay(int x)   {
        refresh(x);
        for (;!isroot(x);rotate(x))
            if (!isroot(fa[x]))
                rotate(get(fa[x])==get(x)?fa[x]:x);
    }
    
    void travel(int x)  {
        if (!x) return ;
        godown(x);
        travel(ls(x));
        cout<<x<<"("<<ls(x)<<","<<rs(x)<<") ";
        travel(rs(x));
    }

    void print(int x)   {
        while (!isroot(x)) x=fa[x];
        cout<<"travel:"; travel(x); cout<<endl;
    }
    
    void info() {
        // for (int i=1;i<=n;++i) refresh(i);
        // for (int i=1;i<=n;++i) {
        //     cout<<i<<":"<<fa[i]<<" ("<<ls(i)<<","<<rs(i)<<") ~ "<<val[i]<<" ("<<mt[i]<<","<<mi[i]<<","<<mn[i]<<") ";
        //     cout<<"set["<<si[i].size()<<"](";
        //     for (auto it=si[i].begin();it!=si[i].end();++it) cout<<*it<<" ";
        //     cout<<")"<<endl;
        // }
    }

    void access(int x)  {
        for (int t=0;x;t=x,x=fa[x]) {
            splay(x);
            if (t) si[x].erase(mn[t]);
            if (rs(x)) si[x].insert(mn[rs(x)]);
            rs(x)=t;
            update(x);
        }
    }

    int findroot(int x)    {
        access(x),splay(x);
        while (ls(x)) x=ls(x);
        splay(x);//can not be deleted
        return x;
    }

    void makeroot(int x)    {
        access(x),splay(x),makerev(x);
    }
    
    void split(int x,int y) {
        makeroot(x),access(y),splay(y);
    }

    void link(int x,int y)  {
        makeroot(x);
        if (findroot(y)==x) return ;
        access(y),splay(y);
        fa[x]=y;
        si[y].insert(mn[x]);
        update(y);
    }

    void cutf(int x)    {
        access(x),splay(x);
        ls(x)=fa[ls(x)]=0;
        update(x);
    }
    void cut(int x,int y)   {
        makeroot(x);
        if (findroot(y)==x&&fa[y]==x&&!ls(y))
            cutf(y);
    }
}

using namespace LCT;

vector<pair<int,int> > E;

int main()  {
    n=read(),m=read();

    E.resize(n);
    for (int k=1;k<n;++k)
        E[k].first=read(),E[k].second=read();
    for (int i=1;i<=n;++i)
        LCT::init(i);
    for (int k=1;k<n;++k)       
        link(E[k].first,E[k].second);
    makeroot(rt=read());

    while (m--) {
        LL op,x,y,d;
        op=read(),x=read();
        switch (op) {
            case 1:{//change root
                makeroot(rt=x);
                break;
            }
            case 2:{//change path x-y to d
                y=read(),d=read();
                split(x,y);
                renew(y,d);
                break;
            }
            case 3:{//minest value in subtree
                makeroot(rt);
                access(x);
                splay(x);
                printf("%lld\n",min(si(x),val[x]));
            }
        }
    }
    return 0;
}