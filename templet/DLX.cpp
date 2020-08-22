#include<bits/stdc++.h>
#define maxm 3050
#define maxn 20050
#define maxp 30005
using namespace std;

const int m=9*9*4;

int sz,n;
int row[maxp],col[maxp];
int L[maxp],R[maxp],U[maxp],D[maxp];

int head[maxn],cnt[maxm];

bool vis[maxn];

int r[maxp],c[maxp],x[maxp];

void add(int i,int j)   {//from up to down,left to right add
    ++sz;
    ++cnt[j];
    row[sz]=i,col[sz]=j;
    D[sz]=j,U[sz]=U[j];
    U[D[sz]]=D[U[sz]]=sz;
    if (!~head[i])
        head[i]=L[sz]=R[sz]=sz;
    else    {
        R[sz]=head[i],L[sz]=L[head[i]];
        L[R[sz]]=R[L[sz]]=sz;
    }
}

void init() {//must init 
    sz=m;
    memset(head,-1,sizeof(head));
    for (int i=0;i<=m;++i)  {
        L[i]=(i+m)%(m+1),R[i]=(i+1)%(m+1);
        U[i]=D[i]=i;
    }
}

void remove(int c)  {
    R[L[c]]=R[c],L[R[c]]=L[c];
    for (int i=D[c];i!=c;i=D[i])
        for (int j=R[i];j!=i;j=R[j])    {
            U[D[j]]=U[j];
            D[U[j]]=D[j];
            --cnt[col[j]];
        }
}

void resume(int c)  {
    for (int i=U[c];i!=c;i=U[i])
        for (int j=L[i];j!=i;j=L[j])    {
            U[D[j]]=j;
            D[U[j]]=j;
            ++cnt[col[j]];
        }
    R[L[c]]=c,L[R[c]]=c;
}


bool dfs()  {
    if (!R[0])  return 1;
    int c=R[0];
    for (int i=R[0];i;i=R[i])
        if (cnt[c]>cnt[i])
            c=i;
    
    remove(c);
    for (int i=D[c];i!=c;i=D[i])    {
        vis[row[i]]=1;
        for (int j=R[i];j!=i;j=R[j]) remove(col[j]);
        if (!dfs()) return 0;
        for (int j=L[i];j!=i;j=L[j]) resume(col[j]);
        vis[row[i]]=0;
    }
    resume(c);
    return 0;
}
