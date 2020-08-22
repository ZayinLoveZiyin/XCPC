
int rnd[maxn];
int rt,sz[maxn];
int ls[maxn],rs[maxn];
int mnw[maxn];
int val[maxn],wei[maxn];

int newnode(int v,int w=inf)  {
	static uniform_int_distribution<int> u(1,1e9);
	static default_random_engine e(rand());

    ++n;
    rnd[n]=u(e);
    sz[n]=1;
    ls[n]=rs[n]=0;
    val[n]=v,mnw[n]=wei[n]=w;
    return n;
}

void update(int k)  {
    sz[k]=sz[ls[k]]+sz[rs[k]]+1;
}

int merge(int x,int y)  {
    if (!x||!y) return x|y;
    int k;
    if (rnd[x]<rnd[y])
        rs[k=x]=merge(rs[x],y);
    else
        ls[k=y]=merge(x,ls[y]);
    return update(k),k;
}

void spilt_val(int k,int v,int &x,int &y) {
    if (!k) return x=y=0,void(0);
    if (val[k]<=v)
        spilt_val(rs[x=k],v,rs[k],y);
    else
        spilt_val(ls[y=k],v,x,ls[k]);
    update(k);
}

void spilt_sz(int k,int s,int &x,int &y) {
    if (!s) return x=0,y=k,void(0);
    if (sz[ls[k]]<s) 
        spilt_sz(rs[x=k],s-sz[ls[k]]-1,rs[k],y);
    else 
        spilt_sz(ls[y=k],s,x,ls[k]);
    update(k);
}

void travel(int rt) {
    if (!rt) return ;
    travel(ls[rt]);
    cout<<val[rt]<<"("<<sz[rt]<<") ";
    travel(rs[rt]);
}

#define print(k) cout<<#k<<":"; travel(k); cout<<endl;