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
using namespace std;

typedef long long ll;
#define maxn 100020

struct point{
	int x,y;
	bool operator < (point a)const{
		if ( x == a.x ) return y < a.y;
		return x < a.x;
	/*	if ( y == a.y ) return x < a.x;
		return y < a.y;*/
	}	
	bool operator == (point a)const{
		return x == a.x && y == a.y;
	}
	point(){};
	point(int a,int b):x(a),y(b){};
	point operator - (point a){
		return point(x - a.x,y - a.y);
	}
	point operator + (point a){
		return point(x + a.x,y + a.y);
	}
	int norm(){
		return x * x + y * y;
	}
};
struct line{
	point a,b;
	line(){};
	line(point x,point y):a(x),b(y){};
};

inline int det(point a,point b){ return a.x * b.y - a.y * b.x; }
inline int dot(point a,point b){ return a.x * b.x + a.y * b.y; }
inline int dist(point a,point b){ return (a - b).norm(); }

vector <point> a,b,c;
vector <point> dt[120];
int n,ans;

void convex_hull(vector <point> &a){
	vector <point> res;
	if ( !a.size() ) return;
	sort(a.begin(),a.end());
	a.erase(unique(a.begin(),a.end()),a.end());
	res.resize(2 * a.size() + 5);
	int m = 0;
	rvc(i,a){
		while ( m > 1 && det(res[m - 1] - res[m - 2],a[i] - res[m - 2]) <= 0 ) --m;
		res[m++] = a[i];
	}	
	int k = m;
	repd(i,(int)(a.size() - 2),0){
		while ( m > k && det(res[m - 1] - res[m - 2],a[i] - res[m - 2]) <= 0 ) --m;
		res[m++] = a[i];
	}
	res.resize(m);
//	if ( a.size() > 1 ) res.resize(m - 1);
	a = res;
}
int cal(point p){
	int x = p.x , y = p.y;
	if ( x >= 0 ){
		if ( y >= 0 ) return 1;
		return 0;
	}
	if ( y >= 0 ) return 2;
	return 3;
}
void getans(){
	c.clear();
	c.resize(a.size() + b.size() + 5);
	/*rvc(i,a){
		cout<<a[i].x<<" "<<a[i].y<<endl;
	}
	cout<<endl;
	rvc(i,b){
		cout<<b[i].x<<" "<<b[i].y<<endl;
	}
	cout<<endl;*/
	c[0] = a[0] + b[0];
	ans = max(ans,c[0].norm());
	int n = a.size() , m = b.size() , pn = 0 , pm = 0;
	rep(i,1,n + m - 2){
		if ( pn == n - 1 ){
			point tmp = b[pm + 1] - b[pm]; pm++;
			c[i] = c[i - 1] + tmp;
		} 
		else if ( pm == m - 1 ){
			point tmp = a[pn + 1] - a[pn]; pn++;
			c[i] = c[i - 1] + tmp;
		}
		else{
			point tmp = a[pn + 1] - a[pn] , tnp = b[pm + 1] - b[pm];
			if (cal(tmp) < cal(tnp) || (cal(tmp) == cal(tnp) &&  (det(tmp,tnp) >= 0)) ) c[i] = c[i - 1] + tmp , pn++;
			else c[i] = c[i - 1] + tnp , pm++;//
		}
		ans = max(ans,c[i].norm());
	}
	/*rep(i,0,n + m - 2){
		cout<<c[i].x<<" "<<c[i].y<<" "<<c[i].norm()<<endl;
	}	
	cout<<endl;*/
}
void solve(int l,int r){	
	if ( l == r ) return;
	int mid = (l + r) >> 1;
	solve(l,mid) , solve(mid + 1,r);
	a.clear() , b.clear();
	rep(i,l,mid){
		rvc(j,dt[i]){
			a.pb(dt[i][j]);
		}
	}
	convex_hull(a);
	rep(i,mid + 1,r){
		rvc(j,dt[i]){
			b.pb(point(-dt[i][j].x,-dt[i][j].y));
		}
	}
	convex_hull(b);
	if ( a.size() && b.size() ) getans(); //minkovski
}
int main(){
	freopen("k.in","r",stdin);
	//freopen("input.txt","r",stdin);
	freopen("1.out","w",stdout);
	int t = 0;
	while ( 1 ){
		++t;
		scanf("%d",&n);	
		if ( !n ) break;
		int mx = 0;
		rep(i,1,n){
			int x,y,k;
			scanf("%d %d %d",&x,&y,&k);
			dt[k].pb(point(x,y));
			mx = max(mx,k);
		}
		solve(0,mx);
		printf("%d\n",ans);
		ans = 0;
		rep(i,0,mx) dt[i].clear();
	}
}

