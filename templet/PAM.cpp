#include<bits/stdc++.h>
#define maxc 26
#define maxn 1000060
#define modu 1000000007
using namespace std;
typedef long long LL;

int all,text[maxn];
int sz,last;

int len[maxn],diff[maxn];
int fail[maxn],anc[maxn];
int ch[maxn][maxc];

int info[maxn];

int newnode(int l)  {
    len[sz]=l;
    fail[sz]=anc[sz]=diff[sz]=0;
    memset(ch[sz],0,sizeof(ch[sz]));
    return sz++;
}

void init() {
    sz=last=all=0;
    newnode(0),newnode(-1);
    fail[0]=1;
    text[0]=-1;
}

int getfail(int x)  {
    for (;text[all-1-len[x]]!=text[all];) 
        x=fail[x];
    return x;
}

void extend(int w)  {
    text[++all]=w;
    int x=getfail(last);
    if (!ch[x][w])  {
        int y=newnode(len[x]+2);
        fail[y]=ch[getfail(fail[x])][w];
        ch[x][w]=y;
        diff[y]=len[y]-len[fail[y]];
        anc[y]=(diff[y]==diff[fail[y]]?anc[fail[y]]:fail[y]);
    }
    last=ch[x][w];
}

int n;
char s[maxn],t[maxn];

int dp[maxn];

int main()  {
    scanf("%s",s+1),n=strlen(s+1);

    init();
    dp[0]=0;
    for (int i=1;i<=n;++i)  {
        extend(s[i]-'a');
		dp[i]=dp[i-1]+1;
        for (int j=last;len[j]>0;j=anc[j])  {
            info[j]=dp[i-len[anc[j]]-diff[j]];
            if (diff[j]==diff[fail[j]])
                info[j]=min(info[j],info[fail[j]]);
			dp[i]=min(dp[i],info[j]+1);
        }
    }
    printf("%d\n",dp[n]);
    return 0;
}