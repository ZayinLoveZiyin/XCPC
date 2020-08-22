#include<bits/stdc++.h>
using namespace std;

namespace algebra {
	const int inf = 1e9;
	const int mod = 2e7 - 1;
	const int magic = 3000; // threshold for sizes to run the naive algo
	
	namespace fft {
		const int maxn = 1 << 18;
 
		typedef double ftype;
		typedef complex<ftype> point;
 
		point w[maxn];
		const ftype pi = acos(-1);
		bool initiated = 0;
		void init() {
			if(!initiated) {
				for(int i = 1; i < maxn; i *= 2) {
					for(int j = 0; j < i; j++) {
						w[i + j] = polar(ftype(1), pi * j / i);
					}
				}
				initiated = 1;
			}
		}
		template<typename T>
		void fft(T *in, point *out, int n, int k = 1) {
			if(n == 1) {
				*out = *in;
			} else {
				n /= 2;
				fft(in, out, n, 2 * k);
				fft(in + k, out + n, n, 2 * k);
				for(int i = 0; i < n; i++) {
					auto t = out[i + n] * w[i + n];
					out[i + n] = out[i] - t;
					out[i] += t;
				}
			}
		}
		
		template<typename T>
		void mul_slow(vector<T> &a, const vector<T> &b) {
			vector<T> res(a.size() + b.size() - 1);
			for(size_t i = 0; i < a.size(); i++) {
				for(size_t j = 0; j < b.size(); j++) {
					res[i + j] += a[i] * b[j];
				}
			}
			a = res;
		}
		
		
		template<typename T>
		void mul(vector<T> &a, const vector<T> &b) {
			if(min(a.size(), b.size()) < magic) {
				mul_slow(a, b);
				return;
			}
			init();
			static const int shift = 15, mask = (1 << shift) - 1;
			size_t n = a.size() + b.size() - 1;
			while(__builtin_popcount(n) != 1) {
				n++;
			}
			a.resize(n);
			static point A[maxn], B[maxn];
			static point C[maxn], D[maxn];
			for(size_t i = 0; i < n; i++) {
				A[i] = point(a[i] & mask, a[i] >> shift);
				if(i < b.size()) {
					B[i] = point(b[i] & mask, b[i] >> shift);
				} else {
					B[i] = 0;
				}
			}
			fft(A, C, n); fft(B, D, n);
			for(size_t i = 0; i < n; i++) {
				point c0 = C[i] + conj(C[(n - i) % n]);
				point c1 = C[i] - conj(C[(n - i) % n]);
				point d0 = D[i] + conj(D[(n - i) % n]);
				point d1 = D[i] - conj(D[(n - i) % n]);
				A[i] = c0 * d0 - point(0, 1) * c1 * d1;
				B[i] = c0 * d1 + d0 * c1;
			}
			fft(A, C, n); fft(B, D, n);
			reverse(C + 1, C + n);
			reverse(D + 1, D + n);
			int t = 4 * n;
			for(size_t i = 0; i < n; i++) {
				int64_t A0 = llround(real(C[i]) / t);
				T A1 = llround(imag(D[i]) / t);
				T A2 = llround(imag(C[i]) / t);
				a[i] = A0 + (A1 << shift) + (A2 << 2 * shift);
			}
			return;
		}
	}
	template<typename T>
	T bpow(T x, size_t n) {
		return n ? n % 2 ? x * bpow(x, n - 1) : bpow(x * x, n / 2) : T(1);
	}
 
	template<int m>
	struct modular {
		int64_t r;
		modular() : r(0) {}
		modular(int64_t rr) : r(rr) {if(abs(r) >= mod) r %= mod; if(r < 0) r += mod;}
		modular inv() const {return bpow(*this, m - 2);}
		modular operator * (const modular &t) const {return (r * t.r) % m;}
		modular operator / (const modular &t) const {return *this * t.inv();}
		modular operator += (const modular &t) {r += t.r; if(r >= m) r -= m; return *this;}
		modular operator -= (const modular &t) {r -= t.r; if(r < 0) r += m; return *this;}
		modular operator + (const modular &t) const {return modular(*this) += t;}
		modular operator - (const modular &t) const {return modular(*this) -= t;}
		modular operator *= (const modular &t) {return *this = *this * t;}
		modular operator /= (const modular &t) {return *this = *this / t;}
		
		bool operator == (const modular &t) const {return r == t.r;}
		bool operator != (const modular &t) const {return r != t.r;}
		
		operator int64_t() const {return r;}
	};
	typedef modular<mod> base;
	template<int T>
	istream& operator >> (istream &in, modular<T> &x) {
		return in >> x.r;
	}
	
	
	template<typename T>
	struct poly {
		vector<T> a;
		
		void normalize() { // get rid of leading zeroes
			while(a.size() && a.back() == T(0)) {
				a.pop_back();
			}
		}
		
		poly(){}
		poly(T a0) : a{a0}{normalize();}
		poly(vector<T> t) : a(t){normalize();}
		
		poly operator += (const poly &t) {
			a.resize(max(a.size(), t.a.size()));
			for(size_t i = 0; i < t.a.size(); i++) {
				a[i] += t.a[i];
			}
			normalize();
			return *this;
		}
		poly operator -= (const poly &t) {
			a.resize(max(a.size(), t.a.size()));
			for(size_t i = 0; i < t.a.size(); i++) {
				a[i] -= t.a[i];
			}
			normalize();
			return *this;
		}
		poly operator + (const poly &t) const {return poly(*this) += t;}
		poly operator - (const poly &t) const {return poly(*this) -= t;}
		
		poly mod_xk(size_t k) const { // get same polynomial mod x^k
			k = min(k, a.size());
			return vector<T>(begin(a), begin(a) + k);
		}
		poly mul_xk(size_t k) const { // multiply by x^k
			poly res(*this);
			res.a.insert(begin(res.a), k, 0);
			return res;
		}
		poly div_xk(size_t k) const { // divide by x^k, dropping coefficients
			k = min(k, a.size());
			return vector<T>(begin(a) + k, end(a));
		}
		poly substr(size_t l, size_t r) const { // return mod_xk(r).div_xk(l)
			l = min(l, a.size());
			r = min(r, a.size());
			return vector<T>(begin(a) + l, begin(a) + r);
		}
		poly inv(int n) const { // get inverse series mod x^n
			assert(!is_zero());
			poly ans = a[0].inv();
			int a = 1;
			while(a < n - 1) {
				poly C = (ans * mod_xk(a)).div_xk(a) + 
				         (ans * substr(a, 2 * a)).mod_xk(a);
				ans -= (ans * C).mod_xk(a).mul_xk(a);
				a *= 2;
			}
			return ans.mod_xk(n);
		}
		
		poly operator *= (const poly &t) {fft::mul(a, t.a); normalize(); return *this;}
		poly operator * (const poly &t) const {return poly(*this) *= t;}
		
		poly reverse(size_t n, bool rev = 0) const { // reverses and leaves only n terms
			poly res(*this);
			if(rev) { // If rev = 1 then tail goes to head
				res.a.resize(max(n, res.a.size()));
			}
			std::reverse(res.a.begin(), res.a.end());
			return res.mod_xk(n);
		}
		
		pair<poly, poly> divmod_slow(const poly &b) const { // when divisor or quotient is small
			vector<T> A(a);
			vector<T> res;
			while(A.size() >= b.a.size()) {
				res.push_back(A.back() / b.a.back());
				if(res.back() != T(0)) {
					for(size_t i = 0; i < b.a.size(); i++) {
						A[A.size() - i - 1] -= res.back() * b.a[b.a.size() - i - 1];
					}
				}
				A.pop_back();
			}
			std::reverse(begin(res), end(res));
			return {res, A};
		}
		
		pair<poly, poly> divmod(const poly &b) const { // returns quotiend and remainder of a mod b
			if(deg() < b.deg()) {
				return {poly{0}, *this};
			}
			int d = deg() - b.deg();
			if(min(d, b.deg()) < magic) {
				return divmod_slow(b);
			}
			poly D = (reverse(d + 1) * b.reverse(d + 1).inv(d + 1)).mod_xk(d + 1).reverse(d + 1, 1);
			return {D, *this - D * b};
		}
		
		poly operator / (const poly &t) const {return divmod(t).first;}
		poly operator % (const poly &t) const {return divmod(t).second;}
		poly operator /= (const poly &t) {return *this = divmod(t).first;}
		poly operator %= (const poly &t) {return *this = divmod(t).second;}
		poly operator *= (const T &x) {
			for(auto &it: a) {
				it *= x;
			}
			normalize();
			return *this;
		}
		poly operator /= (const T &x) {
			for(auto &it: a) {
				it /= x;
			}
			normalize();
			return *this;
		}
		poly operator * (const T &x) const {return poly(*this) *= x;}
		poly operator / (const T &x) const {return poly(*this) /= x;}
		
		void print() const {
			for(auto it: a) {
				cout << it << ' ';
			}
			cout << endl;
		}
		T eval(T x) const { // evaluates in single point x
			T res(0);
			for(int i = int(a.size()) - 1; i >= 0; i--) {
				res *= x;
				res += a[i];
			}
			return res;
		}
		
		T& lead() { // leading coefficient
			return a.back();
		}
		int deg() const { // degree
			return a.empty() ? -inf : a.size() - 1;
		}
		bool is_zero() const { // is polynomial zero
			return a.empty();
		}
		T& operator [](size_t idx) {
			return a[idx];
		}
		void append(base x) { // apends new leading coefficient
			a.push_back(x);
		}
		bool operator == (const poly &t) const {return a == t.a;}
		bool operator != (const poly &t) const {return a != t.a;}
		
	};
 
	typedef poly<base> polyn;
 
	template<typename T>
	T resultant(poly<T> a, poly<T> b) {
		if(b.is_zero()) {
			return 0;
		} else if(b.deg() == 0) {
			return bpow(b.lead(), a.deg());
		} else {
			int pw = a.deg();
			a %= b;
			pw -= a.deg();
			base mul = bpow(b.lead(), pw) * base((b.deg() & a.deg() & 1) ? -1 : 1);
			base ans = resultant(b, a);
			return ans * mul;
		}
	}
};
 
using namespace algebra;