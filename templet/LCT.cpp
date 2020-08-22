#include<bits/stdc++.h>
#define maxn 500050
using namespace std;

int n,m;
namespace LCT   {
    #define ls(x) ch[x][0]
    #define rs(x) ch[x][1]

    int sum[maxn],val[maxn];
    int ch[maxn][2],fa[maxn],rev[maxn];

    void info() {
        for (int i=1;i<=n;++i) cout<<i<<":"<<fa[i]<<" ("<<ls(i)<<","<<rs(i)<<")"<<endl;
    }

    void init(int x) {
        scanf("%d",val+x),sum[x]=val[x];
        fa[x]=ls(x)=rs(x)=rev[x]=0;
    }

    int get(int x)  {   return ch[fa[x]][1]==x; }
    int isroot(int x)   {   return !(ch[fa[x]][0]==x||ch[fa[x]][1]==x); }

    void makerev(int x) {
        if (!x) return ;
        swap(ls(x),rs(x));
        rev[x]^=1;
    }

    void godown(int x)  {
        if (rev[x])
            makerev(ls(x)),makerev(rs(x)),rev[x]=0;
    }

    void update(int x)  {
        sum[x]=sum[ls(x)]^sum[rs(x)]^val[x];
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

    void access(int x)  {
        for (int t=0;x;t=x,x=fa[x]) 
            splay(x),rs(x)=t,update(x);
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
    void link(int x,int y)  {
        makeroot(x);
        if (findroot(y)!=x) 
            fa[x]=y;
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

    void split(int x,int y) {
        makeroot(x),access(y),splay(y);
    }
}

int main()  {
    scanf("%d%d",&n,&m);
    for (int i=1;i<=n;++i)
        LCT::init(i);

    while (m--) {
        int op,x,y;
        scanf("%d%d%d",&op,&x,&y);
        // cout<<"op:"<<op<<" "<<x<<" "<<y<<endl;
        switch (op) {
            case 0:{
                LCT::split(x,y);
                // LCT::print(y);
                printf("%d\n",LCT::sum[y]);
                break;
            }
            case 1:{
                LCT::link(x,y);
                // LCT::print(y);
                break;
            }
            case 2:{
                LCT::cut(x,y);
                break;
            }
            case 3:{
                LCT::splay(x);
                LCT::val[x]=y;
                LCT::update(x);
                break;
            }
        }
        // LCT::info();
        // cout<<endl;
    }
    return 0;
}