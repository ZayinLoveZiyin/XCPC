#include<bits/stdc++.h>
using namespace std;

namespace SA   {
    
    #define fo(i,a,b) for(int i=a;i<=b;i++)
    #define fd(i,a,b) for(int i=a;i>=b;i--)

    const int maxh=21;
    const int maxl=800050;
    int rm,SA[maxl],rank[2*maxl],r2[2*maxl],tong[maxl],height[maxl];

    int Log2[maxl],ST[maxl][maxh];

    void Jsort(int n)
    {
        fo(i,0,rm) tong[i]=0;
        fo(i,1,n) tong[rank[i]]++;
        fo(i,1,rm) tong[i]+=tong[i-1];
        fd(i,n,1) SA[tong[rank[r2[i]]]--]=r2[i];
    }
    bool cmp(int i,int w) {return r2[SA[i]]!=r2[SA[i-1]] || r2[SA[i]+w]!=r2[SA[i-1]+w];}

    void build(int *s,int n) // no zero
    {
        
        for (int i=1;i<=n;++i) SA[i]=0;
        for (int i=1;i<=2*n;++i) rank[i]=r2[i]=0;

        fo(i,1,n) rank[i]=s[i], r2[i]=i;
        rm=500;
        Jsort(n);

        for(int w=1; rm<n || w==1; w<<=1)
        {
            int j=0;
            fo(i,n-w+1,n) r2[++j]=i;
            fo(i,1,n) if (SA[i]-w>0) r2[++j]=SA[i]-w;
            Jsort(n);
            rm=0;//, memcpy(r2,rank,sizeof(r2));
            for (int i=1;i<=2*n;++i) r2[i]=rank[i];
            fo(i,1,n) rank[SA[i]]=(rm+=cmp(i,w));
            
        }
        
        for(int i=1, j=0, k=0; i<=n; height[rank[i++]]=k)
            for(j=SA[rank[i]-1], k-=(k>0); s[i+k]==s[j+k]; k++);
        
        for (int i=2;i<=n;++i) Log2[i]=Log2[i>>1]+1;
        for (int i=1;i<=n;++i) ST[i][0]=height[i];
        for (int j=1;j<=Log2[n];++j)
            for (int i=1;i+(1<<j)-1<=n;++i)
                ST[i][j]=min(ST[i][j-1],ST[i+(1<<j-1)][j-1]);
    }

    int LCP(int i,int j)   {
        if (i==j) return maxl;
        i=rank[i],j=rank[j];
        if (i>j) swap(i,j);
        ++i;
        int t=Log2[j-i+1];
        return min(ST[i][t],ST[j-(1<<t)+1][t]);
    }

    bool cmpsuffix(int i,int j) {
        return rank[i]<rank[j];
    }
}
