#include<bits/stdc++.h>
#define maxb 1050
#define maxn 100050
using namespace std;
typedef long long LL;

int g[maxn]={1},f[maxb]={0,1,2,5,7};

LL ans=0;
void dfs(int c,int x,int s)
{
    ans+=c;
    if (s==40)  return ;
    for (int i=x;i+s<=40;++i)
        dfs(c+1,i,s+i);
}

int main()
{
    for (int i=5;i<maxb;++i)    f[i]=3+2*f[i-2]-f[i-4];
    for (int i=1;i<maxn;++i)
        for (int j=1;f[j]<=i;++j)
            if ((j+1)>>1&1)
                g[i]+=g[i-f[j]];
            else
                g[i]-=g[i-f[j]];
    
    dfs(0,1,0);
    cout<<ans<<endl;
    return 0;
}