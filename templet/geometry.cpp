#include<bits/stdc++.h>
#define maxn 300050
using namespace std;

const double eps=1e-9;
inline int sgn(double x)   {
    if (x>eps)  return 1;
    if (x<-eps) return -1;
    return 0;
}

int n,m;
struct node {
    double x,y;
    node(double _x=0,double _y=0)   {   x=_x,y=_y;    }
    node operator + (const node& N) const   {   return node(x+N.x,y+N.y);    }
    node operator - (const node& N) const   {   return node(x-N.x,y-N.y);    }
    node operator * (const double& l)   const   {   return node(x*l,y*l);    }
    node operator / (const double& l)   const   {   return node(x/l,y/l);    }
    double operator * (const node& N)   const   {   return x*N.y-y*N.x;      }
    double angle()  {   return atan2(y,x);    }
    bool operator < (const node& N) const   {   return x<N.x||(x==N.x&&y<N.y);    }
} P[maxn];

ostream& operator << (ostream& os,const node& N)    {
    os<<"("<<N.x<<","<<N.y<<")";
    return os;
}

struct line {
    node p,q;
    double a;
    line(node _p=0,node _q=0) { p=_p,q=_q,a=(q-p).angle();    }
    bool onleft(const node& v)  const  {  return sgn((q-p)*(v-p))>0;    }
    bool operator < (const line& L) const   {   return a<L.a||(!sgn(a-L.a)&&L.onleft(p));    }
} L[maxn];

ostream& operator << (ostream& os,const line& L)    {
    os<<L.p<<" -> "<<L.q;
    return os;
}

int l,r;
line q[maxn];

node line_inter(const line& l1,const line& l2) {
    double a=(l1.q-l1.p)*(l2.p-l1.p);
    double b=(l1.q-l1.p)*(l1.p-l2.q);
    return (l2.p*b+l2.q*a)/(a+b);
}

double half_plane_insection(line *L,int n)    {
    sort(L,L+n);
    q[l=r=1]=*L;
    for (int i=1;i<n;++i)   {
        if (!sgn(L[i-1].a-L[i].a))  continue;
        while (l<r&&!L[i].onleft(line_inter(q[r],q[r-1]))) --r;
        while (l<r&&!L[i].onleft(line_inter(q[l],q[l+1]))) ++l;
        q[++r]=L[i];
    }
    while (l<r&&!q[l].onleft(line_inter(q[r],q[r-1]))) --r;
    while (l<r&&!q[r].onleft(line_inter(q[l],q[l+1]))) ++l;
    if (r-l<=1) return 0;
    double ans=0;
    q[r+1]=q[l];
    for (int i=l;i<=r;++i)
        P[i]=line_inter(q[i],q[i+1]);
    P[r+1]=P[l];
    for (int i=l;i<=r;++i)
        ans+=P[i]*P[i+1];
    return abs(ans)/2;
}