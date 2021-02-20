#include<bits/stdc++.h>
#define maxn 50050
#define maxp 40000050
#define modu 998244353
using namespace std;
typedef long long LL;

int n,m;

int CNT=0;
namespace RPRMQ {
    const LL H=15,N=1<<H,inf=1e10,INF=1e18;

    struct seg  {
        int lx,rx;
        int l,r;
        LL d;
        bool operator < (const seg& S) const {
            return d>S.d;
        }
    };
    vector<seg> event[N];

    void addrange(int lx,int rx,int ly,int ry,int d)    {
        // cout<<"add ["<<lx<<","<<rx<<"] x ["<<ly<<","<<ry<<"] "<<d<<endl;
        event[lx].push_back(seg{lx,rx+1,ly,ry,d});
        event[rx+1].push_back(seg{lx,rx+1,ly,ry,-d});
    }

    int sz;
    struct node {
        int facnt;
        int ls,rs;
        LL mn,hmn;
        LL tag,htag;
    } T[maxp];
    #define ls(k) T[k].ls
    #define rs(k) T[k].rs
    #define mn(k) T[k].mn
    #define hmn(k) T[k].hmn
    #define tag(k) T[k].tag
    #define htag(k) T[k].htag
    #define facnt(k) T[k].facnt

    struct SegT {
        int rt,offset;
    } layerL[H][N],layerR[H][N];

    int newnode(int k)   {
        if (k&&facnt(k)==1) return k;
        T[++sz]=T[k];
        --facnt(k);
        ++facnt(ls(k)),++facnt(rs(k));
        facnt(sz)=1;
        assert(sz<maxp);
        return sz;
    }

    void renew(int &k,LL hd,LL d)    {
        k=newnode(k);
        hmn(k)=min(hmn(k),mn(k)+hd);
        mn(k)+=d;
        htag(k)=min(htag(k),tag(k)+hd);
        tag(k)+=d;
    }
    
    void godown(int k)  {
        if (!tag(k)&&htag(k)==INF) return ;
        renew(ls(k),htag(k),tag(k));
        renew(rs(k),htag(k),tag(k));
        tag(k)=0,htag(k)=INF;
    }


    void update(int k)  {
        mn(k)=min(mn(ls(k)),mn(rs(k)));
        hmn(k)=min(hmn(ls(k)),hmn(rs(k)));
    }

    void change(int &k,int l,int r,int a,int b,LL d)   {
        if (!d||a>r||l>b||a>b) return ;
        ++CNT;
        // cout<<"change:"<<k<<"("<<mn(k)<<","<<hmn(k)<<" | "<<tag(k)<<","<<htag(k)<<") "<<l<<" "<<r<<" "<<a<<" "<<b<<" "<<d<<endl;
        if (a<=l&&r<=b) {
            renew(k,d,d);
        } else  {
            godown(k);
            k=newnode(k);
            int mid=(l+r)>>1;
            if (a<=mid)
                change(ls(k),l,mid,a,b,d);
            if (b>mid)
                change(rs(k),mid+1,r,a,b,d);
            update(k);
        }
        assert(k);
        // cout<<"after change:"<<k<<"("<<mn(k)<<","<<hmn(k)<<" | "<<tag(k)<<","<<htag(k)<<") "<<l<<" "<<r<<" "<<a<<" "<<b<<" "<<d<<endl;
    }

    LL query(int k,int l,int r,int a,int b) {
        if (a>r||l>b||a>b) return INF;
        // cout<<"query:"<<k<<"("<<mn(k)<<","<<hmn(k)<<" | "<<tag(k)<<","<<htag(k)<<" | "<<flag(k)<<") "<<l<<" "<<r<<" "<<a<<" "<<b<<endl;
        if (a<=l&&r<=b) return min(hmn(k),mn(k));
        godown(k);
        int mid=(l+r)>>1;
        return min(query(ls(k),l,mid,a,b),query(rs(k),mid+1,r,a,b));
    }

    LL query(const SegT& T,int l,int r) {
        // cout<<"query rt["<<i<<"]="<<rt[i]<<endl;
        LL res=query(T.rt,1,n,l,r)+T.offset*inf;
        // cout<<"query:"<<T.rt<<" "<<l<<" "<<r<<" ~ "<<res<<endl;
        return res;
    }

    void travel(int k,int l,int r,LL offset)  {
        // cout<<"travel:"<<k<<"("<<mn(k)<<","<<hmn(k)<<" | "<<tag(k)<<","<<htag(k)<<") "<<l<<" "<<r<<endl;
        if (l==r)
            cout<<"travel:"<<k<<" "<<l<<" ~ "<<mn(k)+offset<<" "<<hmn(k)+offset<<endl;
        else    {
            godown(k);
            int mid=(l+r)>>1;
            travel(ls(k),l,mid,offset);
            travel(rs(k),mid+1,r,offset);
        }
    }

    void calc() {
        sz=0;
        hmn(0)=htag(0)=INF;


        int OP=0;
        int lastrt,cnt,hcnt;

        // #define OPTIM1
        #ifdef OPTIM1
            vector<pair<int,LL> > op;
            auto insop=[&](const vector<seg>& vs,int sgn=1)   {
                for (seg s:vs)
                    ++OP,
                    // cout<<"insop: ["<<s.l<<","<<s.r<<"] "<<s.d<<endl,
                    op.emplace_back(s.l,sgn*s.d),op.emplace_back(s.r+1,-sgn*s.d);
            };
            auto exeop=[&]() {
                // cout<<"to exe:"<<op.size()<<endl;
                ++RPRMQ::T[lastrt].facnt;
                op.emplace_back(N,0);
                sort(op.begin(),op.end());
                LL lastx=0,sum=0;
                for (int i=0,j=0;i<op.size();i=j)   {
                    ++cnt,
                    change(lastrt,1,n,lastx,op[i].first-1,sum);
                    lastx=op[i].first;
                    for (;j<op.size()&&lastx==op[j].first;++j)
                        sum+=op[j].second;
                }
                // cout<<"???:"<<lastrt<<endl;
                op.clear();
                return SegT{lastrt,hcnt};
            };
        #else
            vector<seg> op;
            auto insop=[&](const vector<seg>& vs,int sgn=1)   {
                for (seg s:vs)
                    ++OP,
                    op.push_back(s),op.back().d*=sgn;
                    // cout<<"insop: ["<<s.l<<","<<s.r<<"] "<<s.d<<endl,
                    // op.emplace_back(s.l,sgn*s.d),op.emplace_back(s.r+1,-sgn*s.d);
            };
            auto exeop=[&]() {
                ++RPRMQ::T[lastrt].facnt;
                sort(op.begin(),op.end(),[&](const seg& a,const seg& b){
                    return a.l<b.l||(a.l==b.l&&a.r<b.r);
                });
                vector<seg> newop;
                for (seg s:op)
                    if (newop.size()&&s.l==newop.back().l&&s.r==newop.back().r)
                        newop.back().d+=s.d;
                    else
                        newop.push_back(s);
                sort(newop.begin(),newop.end());
                for (seg s:newop)  {
                    ++cnt;
                    // cout<<"add ["<<s.l<<","<<s.r<<"] "<<s.d<<endl;
                    change(lastrt,1,n,s.l,s.r,s.d);
                }
                op.clear();
                return SegT{lastrt,hcnt};
            };
        #endif

        auto reset=[&]() {
            // T.change(lastrt,1,n,1,n,-inf*++cnt);
            ++cnt;
            insop({seg{-1,-1,1,n,-inf*cnt}});
            hcnt+=cnt,cnt=0;
            // cout<<"reset:"<<hcnt<<endl;
        };

        lastrt=cnt=hcnt=0;
        function<void(int,int,int)> build=[&](int dep,int l,int r){
            if (l==r) return ;
            // cout<<"build:"<<dep<<" "<<l<<" "<<r<<endl;
            assert(dep<H);
            int mid=(l+r)>>1;
            for (int i=l;i<=mid;++i) insop(event[i],1);
            // for (int i=l;i<=mid;++i)
            //     for (seg s:event[i])    {
            //         if (l<=s.lx&&s.rx<=mid) continue;
            //         insop({s},1);
            //     }
            reset();
            // cout<<"hcnt:"<<hcnt<<endl;
            for (int i=mid+1;i<=r;++i)  {
                insop(event[i],1);
                // cout<<"layerL:"<<dep<<" "<<i<<endl;
                layerL[dep][i]=exeop();
                // travel(layerL[dep][i].rt,1,n,layerL[dep][i].offset*inf);
            }
            for (int i=r;i>mid;--i) insop(event[i],-1);
            // for (int i=mid+1;i<=r;++i)
            //     for (seg s:event[i])    {
            //         if (mid<s.lx&&s.rx<=r) continue;
            //         insop({s},-1);
            //     }
            build(dep+1,mid+1,r);
            reset();
            for (int i=mid;i>=l;--i)    {
                // cout<<"layerR:"<<dep<<" "<<i<<endl;
                layerR[dep][i]=exeop();
                // travel(layerR[dep][i].rt,1,n,layerR[dep][i].offset*inf);
                insop(event[i],-1);
            }
            build(dep+1,l,mid);
        };
        build(0,1,n);
        // cerr<<"info:"<<sz<<" "<<CNT<<" "<<OP<<endl;
    }

    int Log2(int x)   {
        return !x?-1:Log2(x>>1)+1;
    }

    LL query(int lx,int rx,int ly,int ry,int dep=0,int l=1,int r=n)   {
        assert(dep<H);
        // cout<<"query:"<<lx<<" "<<rx<<" "<<ly<<" "<<ry<<" ~ "<<dep<<" "<<l<<" "<<r<<endl;
        int mid=(l+r)>>1;
        if (lx==mid&&rx==mid) return query(layerR[dep][lx],ly,ry);
        if (lx==mid+1&&rx==mid+1) return query(layerL[dep][rx],ly,ry);
        
        if (lx<=mid&&rx>mid)
            return min(query(layerL[dep][rx],ly,ry),
                       query(layerR[dep][lx],ly,ry));
        else
        if (rx<=mid)
            return query(lx,rx,ly,ry,dep+1,l,mid);
        else
        if (lx>mid)
            return query(lx,rx,ly,ry,dep+1,mid+1,r);
        else
            assert(0);

        // int t=max(Log2(lx^rx),0);
        // cout<<"t:"<<t<<endl;
        // return min(layerL[t].query(rx,ly,ry),
        //            layerR[t].query(lx,ly,ry));
    }
}

int main()  {
    int n,m,q;
    scanf("%d%d%d",&n,&m,&q);
    while (m--) {
        int lx,rx,ly,ry,d;
        scanf("%d%d%d%d%d",&lx,&ly,&rx,&ry,&d);
        RPRMQ::addrange(lx,rx,ly,ry,-d);
    }

    RPRMQ::calc();
    while (q--) {
        int lx,rx,ly,ry;
        scanf("%d%d%d%d",&lx,&ly,&rx,&ry);
        printf("%lld\n",-RPRMQ::query(lx,rx,ly,ry));
    }
    return 0;
}