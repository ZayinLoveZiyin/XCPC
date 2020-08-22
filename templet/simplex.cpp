#include<bits/stdc++.h>
#define maxn 105
using namespace std;

int n,m;

const double eps=1e-5;
const double inf=1e9;

const int width=5;

ostream& operator << (ostream& os,const vector<double>& A)	{
	for (int i=0;i<A.size();++i)	{
		os<<setw(width)<<A[i]<<" ";
		if (i+1<A.size())
			cout<<"& ";
	}
	return os;
}

ostream& operator << (ostream& os,const vector<vector<double> >& A)	{
	for (int i=0;i<A.size();++i)
		os<<A[i]<<endl;
	return os;
}

ostream& operator << (ostream& os,const vector<int>& A)	{
	for (int i=0;i<A.size();++i)
		os<<A[i]<<" ";
	os<<endl;
	return os;
}

struct matrix	{
	int n,m;
	vector<vector<double> > A;
	matrix(int _n=0,int _m=0):n(_n),m(_m)	{
		A.resize(n);
		for (int i=0;i<n;++i)	{
			A[i].resize(m);
			for (int j=0;j<m;++j)
				A[i][j]=0;
		}
	}
	void init(int _n,int _m)	{
		n=_n,m=_m;
		A.resize(n);
		for (int i=0;i<n;++i)	{
			A[i].resize(m);
			for (int j=0;j<m;++j)
				scanf("%lf",&A[i][j]);
		}
	}

	vector<double>& operator [] (const int& i)	{	return A[i];	}
	const vector<double>& operator [] (const int& i) const	{	return A[i];	}

	matrix operator * (const matrix& M) const	{
		assert(m==M.n);

		matrix ans(n,M.m);

		for (int i=0;i<ans.n;++i)
			for (int j=0;j<ans.m;++j)
				for (int k=0;k<m;++k)
					ans[i][j]+=A[i][k]*M[k][j];
		
		return ans;
	}
	vector<double> operator * (const vector<double>& b) const	{
		assert(m==b.size());

		vector<double> ans(n);
		for (int i=0;i<n;++i)	{
			ans[i]=0;
			for (int j=0;j<m;++j)
				ans[i]+=A[i][j]*b[j];
		}
		return ans;
	}

	friend ostream& operator << (ostream& os,const matrix& A)	{
		for (int i=0;i<A.n;++i)
			os<<A[i]<<endl;
		return os;
	}

	matrix operator ~ () const	{
		assert(n==m);
		matrix _A(n,n);
		for (int i=0;i<n;++i)
			_A[i][i]=1;

		vector<vector<double> > AA=A;

		for (int i=0;i<n;++i)	{
			assert(abs(AA[i][i])>eps);

			double t=AA[i][i];
			for (int j=0;j<n;++j)	{
				AA[i][j]/=t;
				_A[i][j]/=t;
			}

			for (int k=i+1;k<n;++k)	{
				double t=AA[k][i];
				for (int j=0;j<n;++j)	{
					AA[k][j]-=AA[i][j]*t;
					_A[k][j]-=_A[i][j]*t;
				}
			}
		}

		for (int i=n-1;i>=0;--i)	{
			for (int k=i+1;k<n;++k)	{
				double t=AA[i][k];
				for (int j=0;j<n;++j)	{
					_A[i][j]-=_A[k][j]*t;
					AA[i][j]-=AA[k][j]*t;
				}
			}
			// cout<<"AA:"<<endl;
			// cout<<AA<<endl;
			// cout<<"_A:"<<endl;
			// cout<<_A<<endl;
		}
		return _A;
	}
} A;


vector<double> operator * (const vector<double>& b,const matrix& A)	{
	assert(n==b.size());

	vector<double> ans(A.m);
	for (int j=0;j<ans.size();++j)	{
		ans[j]=0;
		for (int i=0;i<A.n;++i)
			ans[j]+=A[i][j]*b[i];
	}
	return ans;
}

vector<double> operator - (const vector<double>& a,const vector<double>& b)	{
	assert(a.size()==b.size());
	vector<double> ans;
	for (int i=0;i<a.size();++i)
		ans.push_back(a[i]-b[i]);
	return ans;
}

double operator * (const vector<double>& a,const vector<double>& b)	{
	assert(a.size()==b.size());
	double ans;
	for (int i=0;i<a.size();++i)
		ans+=a[i]*b[i];
	return ans;
}


vector<double> b,c;

string tostring(int x)	{
	string s;
	do {
		s=(char)(x%10+'0')+s;
	} while (x/=10);
	return s;
}

double simplex()	{

	vector<int> base={0,2,4};
	// // for (int i=0;i<m;++i) base.push_back(i);
	// // std::random_shuffle(base.begin(),base.end());
	// // base.resize(n);
	base.resize(1);
	for (int i=0;i<n;++i) base.push_back(m-n+i);

	matrix T(n+1,m+1);

	for (int j=0;j<m;++j)
		T[0][j]=c[j];
	for (int i=0;i<n;++i)
		for (int j=0;j<m;++j)
			T[i+1][j]=A[i][j];
	for (int i=0;i<n;++i)
		T[i+1][m]=b[i];

	cout<<"initial:"<<endl;
	cout<<T<<endl;

	matrix B(n+1,n+1);
	B[0][0]=1;
	for (int i=1;i<=n;++i)
		for (int j=1;j<=n;++j)
			B[i][j]=T[i][base[j]];
	T=~B*T;


	auto print=[&]()	{
		cout<<"T:"<<endl;
		cout<<setw(5)<<" "<<" & ";
		for (int i=1;i<=m;++i)
			cout<<setw(width)<<"x"+tostring(i)<<" & ";
		cout<<" \\\\ \\hline"<<endl;
		for (int i=0;i<=n;++i)	{
			if (i)
				cout<<setw(5)<<"x"+tostring(base[i]+1)<<" & ";
			else
				cout<<setw(5)<<"f"<<" & ";
			cout<<T[i]<<" \\\\ \\hline"<<endl;
		}
	};

	while (1)	{
		print();

		int in=-1;

		for (int j=0;j<m;++j)	{
			bool flag=1;
			for (int i=1;i<=n;++i)
				if (base[i]==j)
					flag=0;
			if (!flag||T[0][j]>eps) continue;
			if (!~in||T[0][j]<T[0][in])
				in=j;
		}

		if (!~in) return -T[0][m];

		vector<double> r;
		for (int i=0;i<=n;++i)
			r.push_back(T[i][m]/T[i][in]);

		int out=-1;
		for (int i=1;i<=n;++i)	{
			if (T[i][in]<eps) continue;
			if (!~out||r[i]<r[out])
				out=i;
		}
		if (!~out) return inf;

		double t=T[out][in];
		for (int j=0;j<=m;++j)
			T[out][j]/=t;
			
		for (int i=0;i<=n;++i)	{
			if (i==out) continue;
			double t=T[i][in]/T[out][in];
			for (int j=0;j<=m;++j)
				T[i][j]-=T[out][j]*t;
		}

		cout<<"in:  x"<<in+1<<endl;
		cout<<"out: x"<<base[out]+1<<endl;
		cout<<endl;

		base[out]=in;


	}

	return -1;
}

double dual_simplex()	{

	vector<int> base={0,2,4};
	// // for (int i=0;i<m;++i) base.push_back(i);
	// // std::random_shuffle(base.begin(),base.end());
	// // base.resize(n);
	base.resize(1);
	for (int i=0;i<n;++i) base.push_back(m-n+i);

	matrix T(n+1,m+1);

	for (int j=0;j<m;++j)
		T[0][j]=c[j];
	for (int i=0;i<n;++i)
		for (int j=0;j<m;++j)
			T[i+1][j]=A[i][j];
	for (int i=0;i<n;++i)
		T[i+1][m]=b[i];
	

	auto print=[&]()	{
		cout<<"T:"<<endl;
		cout<<setw(5)<<" "<<" & ";
		for (int i=1;i<=m;++i)
			cout<<setw(width)<<"x"+tostring(i)<<" & ";
		cout<<" \\\\ \\hline"<<endl;
		for (int i=0;i<=n;++i)	{
			if (i)
				cout<<setw(5)<<"x"+tostring(base[i]+1)<<" & ";
			else
				cout<<setw(5)<<"f"<<" & ";
			cout<<T[i]<<" \\\\ \\hline"<<endl;
		}
	};

	cout<<"initial:"<<endl;
	print();

	matrix B(n+1,n+1);
	B[0][0]=1;
	for (int i=1;i<=n;++i)
		for (int j=1;j<=n;++j)
			B[i][j]=T[i][base[j]];
	T=~B*T;

	while (1)	{
		print();

		int out=-1;
		for (int i=1;i<=n;++i)	{
			if (T[i][m]>eps) continue;
			// cout<<i<<":"<<T[i][m]<<endl;
			if (!~out||T[i][m]<T[out][m])
				out=i;
		}
		if (!~out) return -T[0][m];

		vector<double> r;
		for (int j=0;j<m;++j)
			r.push_back(-T[0][j]/T[out][j]);

		int in=-1;
		for (int j=0;j<m;++j)	{
			bool flag=1;
			for (int i=1;i<=n;++i)
				if (base[i]==j)
					flag=0;
			if (!flag||T[out][j]>-eps) continue;
			if (!~in||r[j]<r[in])
				in=j;
		}
		if (!~in) return inf;

		double t=T[out][in];
		for (int j=0;j<=m;++j)
			T[out][j]/=t;
			
		for (int i=0;i<=n;++i)	{
			if (i==out) continue;
			double t=T[i][in]/T[out][in];
			for (int j=0;j<=m;++j)
				T[i][j]-=T[out][j]*t;
		}

		cout<<"in:  x"<<in+1<<endl;
		cout<<"out: x"<<base[out]+1<<endl;
		cout<<endl;

		base[out]=in;


	}

	return -1;
}

int main()	{
	srand(time(0));

	scanf("%d%d",&n,&m);
	A.init(n,m);

	// cout<<(~A)<<endl;

	b.resize(n);
	for (int i=0;i<n;++i)
		scanf("%lf",&b[i]);
	c.resize(m);
	for (int i=0;i<m;++i)
		scanf("%lf",&c[i]);
	
	// printf("%lf\n",dual_simplex());
	printf("%lf\n",simplex());
	return 0;
}