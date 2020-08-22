#include<bits/stdc++.h>
#define maxn 1000050
using namespace std;
typedef long long LL;

namespace IO {
	char In[1<<20],*p1=In,*p2=In,c,Out[1<<23],*Op=Out,St[10],*Tp=St;
	#define Getc (p1==p2&&(p2=(p1=In)+fread(In,1,1<<20,stdin),p1==p2)?EOF:*p1++)
	inline void Flush(){fwrite(Out,1,Op-Out,stdout);}
	inline int read(int x=0)   {
        int s=1;
        while(!isdigit(c=Getc)&&c!='-');
        if (c=='-') s=-1,c=Getc;
        for(;isdigit(c);c=Getc)x=(x<<3)+(x<<1)+(c^48);
        return s*x;
    }
	inline void write(int x)    {
        do *Tp++=x%10^48; while(x/=10);
        do *Op++=*--Tp; while(Tp!=St);
        *Op++='\n';
    }
}