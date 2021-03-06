#pragma GCC optimize(3)
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i=a;i<=b;++i)
#define per(i,a,b) for(int i=a;i>=b;--i)
#define repd(i,a,b) for(int i=a;i>=b;--i)
#define rvc(i,S) for(int i=0;i<(int)S.size();++i)
#define fore(i,x) for(int i = head[x] ; i ; i = e[i].next)
#define mp make_pair
#define pb push_back
#define fi first
#define se second
#define debug(...) fprintf(stderr,__VA_ARGS__)
#define lowbit(x) (x&(-x))
using namespace std;
#define maxn 300020

typedef long long ll;
typedef vector <ll> Poly;
#define gi 13ll
#define ginv 696899742ll
const ll mod = 1811939329;
const ll G = 13;

int n,a[maxn];
ll ans[maxn];
ll w[2][maxn];

namespace Polygen {
	inline ll add(ll x,ll y){
		x += y;
		if ( x >= mod ) x -= mod;
		return x;
	}
	inline ll sub(ll x,ll y){
		x -= y;
		if ( x < 0 ) x += mod;
		return x;
	}
	inline ll power(ll x,ll y){
		ll res = 1;
		while ( y ){
			if ( y & 1 ) res = res * x % mod;
			x = x * x % mod;
			y >>= 1;
		}
		return res;
	}
	void init(){
		for (int i = 1; i < (1 << 18); i <<= 1) {
			ll wn = power(gi, (mod - 1) / (i << 1));
			for (int j = 0; j < i; j++) {
				w[1][i + j] = (j ? wn * w[1][i + j - 1] % mod : 1);
			}
			wn = power(gi, mod - 1 - (mod - 1) / (i << 1));
			for (int j = 0; j < i; j++) {
				w[0][i + j] = (j ? wn * w[0][i + j - 1] % mod : 1);
			}
		}
	}	
	void print(const Poly &p){
		cout<<p.size()<<endl;
		rvc(i,p) cout<<p[i]<<" ";
		cout<<endl;
	}
	inline void NTT(Poly &a,int len,int fl) { //预处理原根快了一倍！
		fl = max(0 , fl);
		int i , j , k;
		for (j = 1 , i = ( len >> 1 ) ; j < len; j++) {
			if (j > i) swap(a[i] , a[j]);
			for (k = (len>>1) ; k & i; k >>= 1 ) i ^= k;
			i ^= k;
		}

		for (i = 1; i < len; i <<= 1) {
			for (j = 0; j < len; j += i << 1) {
				for (k = 0; k < i; k++) {
					ll x = a[j + k], y = a[j + k + i] * w[fl][i + k] % mod;
					a[j + k] = add( x , y );
					a[j + k + i] = sub( x , y );
				}
			}
		}
		if (fl == 0) {
			ll inv = power(len , mod-2);
			for (int i = 0; i < len; i++)	a[i] = a[i] * inv % mod;
		}
	}
/*	void NTT(Poly &g,int len,int tag){
		int cnt = 0;
		while ( (1 << cnt) < len ) cnt++;
		rep(i,0,len - 1){
			int cur = 0;
			rep(j,0,cnt - 1){
				if ( i & (1 << j) ) cur += 1 << (cnt - j - 1);
			}
			if ( cur > i ) swap(g[i],g[cur]);
		}
		for (int i = 1 ; i < len ; i <<= 1){
			ll wn = power(G,(mod - 1) / (i << 1)); //除以位长，而不是半位长
			for (int j = 0 ; j < len ; j += (i << 1)){
				ll w = 1;
				for (int k = 0 ; k < i ; k++){
					ll x = g[j + k] , y = g[i + j + k] * w % mod;
					g[j + k] = (x + y) % mod;
					g[i + j + k] = (x - y + mod) % mod;
					w = w * wn % mod;
				}
			}
		}
		if ( tag == -1 ){
			reverse(g.begin() + 1,g.begin() + len); //从1开始翻转
			
			ll inv = power(len,mod - 2);
			for (int i = 0 ; i < len ; i++) g[i] = g[i] * inv % mod;
		}
	}	*/

	void clean(Poly &p){ //删除系数为0的高次项
		while ( p.size() && !p.back() ) p.pop_back();
	}
	inline Poly operator + (Poly p,const Poly &p2){
		p.resize( max(p.size(),p2.size()) );
		rvc(i,p2) p[i] = add(p[i],p2[i]);
		return p;
	}
	inline Poly operator - (Poly p,const Poly &p2){
		p.resize( max(p.size(),p2.size()) );
		rvc(i,p2) p[i] = sub(p[i],p2[i]);
		clean(p);
		return p;
	}
	inline Poly operator * (const Poly &p , const Poly &p2){
		int len = p.size() + p2.size() - 1;
		len = ceil(log2(len));
		len = 1 << len;
		static Poly p3 , p4;
		p3 = p , p4 = p2;
		
		p3.resize(len) , p4.resize(len);
	
		
		NTT(p3,len,1); 
	
		NTT(p4,len,1);
	
		rep(i,0,len - 1) p3[i] = p3[i] * p4[i] % mod;
		NTT(p3,len,-1);
		clean(p3);
	
		return p3;
	}

	Poly _p(maxn * 2),cur(maxn * 2);
	void poly_inverse(int n,const Poly& A,Poly& B){
		if ( n == 1 ){
			B[0] = power(A[0],mod - 2);
			return;
		}
	
		poly_inverse((n + 1) / 2,A,B);		

		int len = ceil(log2(n << 1));
		len = 1 << len;
	
		rep(i,0,n - 1) _p[i] = A[i];
		rep(i,n,len - 1) _p[i] = 0;
		NTT(B,len,1);
		NTT(_p,len,1);

		rep(i,0,len - 1)
			B[i] = B[i] * (2 - _p[i] * B[i] % mod + mod) % mod;
		NTT(B,len,-1);
		rep(i,n,len - 1) B[i] = 0;
	}
	Poly Inv(const Poly &p){
		int len = ceil(log2(p.size() << 1));
		len = 1 << len;
		Poly res(len);
		poly_inverse(p.size(),p,res);
		res.resize(p.size());
		return res;
	}	
		void check(const Poly &A,const Poly &B){ //检查逆元是否求对
		Poly x = A , y = B;
		int len = ceil(log2(x.size() << 1));
		len = 1 << len;
		x.resize(len) , y.resize(len);
		NTT(x,len,1) , NTT(y,len,1);
		rep(i,0,len - 1) x[i] = x[i] * y[i] % mod;
		NTT(x,len,-1);
		x.resize(A.size());
		print(x);
	}
	void check_sqrt(const Poly &A){
		Poly x = A;
		int len = ceil(log2(x.size() << 1));
		len = 1 << len;
		x.resize(len);
		NTT(x,len,1);
		rep(i,0,len - 1) x[i] = x[i] * x[i] % mod;
		NTT(x,len,-1);
		x.resize(A.size());
		print(x);
	}

	void sqrt(int n,const Poly &A,Poly &B){
		if ( n == 1 ){
			B[0] = 1;
			return; //保证常数项为1，否则求A[0]的平方根
		}
		sqrt((n + 1) / 2,A,B);
		
		int len = ceil(log2(n << 1));
		len = 1 << len;
		
		B.resize(n);
		cout<<B.size()<<endl;
		cur = Inv(B);
		cout<<cur.size()<<endl;	
	//	check(B,cur);
		cur.resize(len) , B.resize(len) , _p.resize(len);
		rep(i,0,n - 1) _p[i] = A[i];
		rep(i,n,len - 1) B[i] = cur[i] = _p[i] = 0;
		NTT(cur,len,1);
		NTT(_p,len,1);
		NTT(B,len,1);
		rep(i,0,len - 1) B[i] = (B[i] + cur[i] * _p[i]) % mod * power(2,mod - 2) % mod;
		NTT(B,len,-1);
		rep(i,n,len - 1) B[i] = 0;
	//	cout<<"this layer "<<n<<endl;
	//	print(A);
	//	check_sqrt(B);
	}
	inline Poly SQRT(const Poly &p){
		int len = ceil(log2(p.size() << 1));
		len = 1 << len;
		cout<<len<<endl;
		Poly res(len);
		sqrt(p.size(),p,res);
		rep(i,0,10) cout<<p[i]<<" ";
		cout<<endl;
		res.resize(p.size());
		return res;
	}
	Poly Divide(const Poly &a,const Poly &b){ // a(x) = b(x) * d(x) + r(x)
		int n = a.size() , m = b.size();
		if ( n < m ) return Poly {};
		Poly ra = a , rb = b , res;
		reverse(rb.begin(),rb.end());
		reverse(ra.begin(),ra.end());

		rb.resize(n - m + 1) , ra.resize(n - m + 1);
		rb = Inv(rb);
		res = rb * ra;
		res.resize(n - m + 1);
		reverse(res.begin(),res.end());
		clean(res);
		return res;
	}
	Poly Mod (const Poly &a,const Poly &b){ // a(x) = b(x) * d(x) + r(x)
		int n = a.size() , m = b.size();
		if ( n < m ) return a;
		Poly ra = a , rb = b , res;
		reverse(rb.begin(),rb.end());
		reverse(ra.begin(),ra.end());

		rb.resize(n - m + 1) , ra.resize(n - m + 1);
		rb = Inv(rb);
		res = rb * ra;
		res.resize(n - m + 1);
		reverse(res.begin(),res.end());
		clean(res);
		res = b * res;
		res = a - res;
		return res;
	}
	inline Poly operator / (const Poly &p,const Poly &p2){
		Poly res = Divide(p,p2);
		clean(res);
		return res;
	}
	inline Poly operator % (const Poly &p,const Poly &p2){
		Poly res = Mod(p,p2);
		clean(res);
		return res;
	}
	void Derivative(Poly &a){
		rep(i,0,(int)a.size() - 2) a[i] = a[i + 1] * (i + 1) % mod;
		a.pop_back();
	}
	void pre(int x,int l,int r,vector <Poly> &s){ //求出 (x - xi) 的多项式
		if ( l == r ){
			s[x] = Poly{ mod - a[l], 1};
			return;
		}
		int mid = (l + r) >> 1;
		pre(x << 1,l,mid,s);
		pre((x << 1) | 1,mid + 1,r,s);
		s[x] = s[x << 1] * s[(x << 1) | 1];
	}
	void getVal(int x,int l,int r,Poly &p,vector <Poly> &m,ll res[]){ //vector 一定要传地址！！
		Poly a = p % m[x];
		if ( l == r ){
			res[l] = a.empty() ? 0 : a.back();
			return;
		}
		int mid = (l + r) >> 1;
		getVal(x << 1,l,mid,a,m,res);
		getVal((x << 1) | 1,mid + 1,r,a,m,res);
	}

}
using namespace Polygen;

int main(){
	
	init();
	scanf("%d",&n);
	rep(i,0,n) scanf("%d",&a[i]);
	vector <Poly> s(n << 2);
	pre(1,0,n,s);
	static Poly f = s[1];
	Derivative(f);
	getVal(1,0,n,f,s,ans);
	ans[n] = mod - ans[n];
	rep(i,0,n - 1){
		printf("%lld ",ans[n] * power(ans[i],mod - 2) % mod);
	}
}
