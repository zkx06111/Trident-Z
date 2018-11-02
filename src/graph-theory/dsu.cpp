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
#define maxn 1000020
#define eps 1e-8

typedef long long ll;
struct node{
	int next,to;
}e[maxn * 2];


int head[maxn],cnt;
int mxd[maxn],fa[maxn],son[maxn],dth[maxn],q;
int n,sz[maxn];

int *f[maxn],*g[maxn],tmp[maxn << 2],*id = tmp,ans[maxn];

inline void adde(int x,int y){
	e[++cnt].to = y;
	e[cnt].next = head[x];
	head[x] = cnt;
}
void dfs1(int x){
	mxd[x] = 1 , sz[x] = 1;
	fore(i,x){
		if ( fa[x] == e[i].to ) continue;
		dth[e[i].to] = dth[x] + 1 , fa[e[i].to] = x;
		dfs1(e[i].to);
		sz[x] += sz[e[i].to];
		mxd[x] = max(mxd[x],mxd[e[i].to] + 1);
		if ( mxd[son[x]] < mxd[e[i].to] + 1 ) son[x] = e[i].to;
	}
}
void dfs(int x){
	if ( son[x] ){
		f[son[x]] = f[x] + 1; //数组指针偏移技巧：直接沿用重儿子信息
		dfs(son[x]);
		ans[x] = ans[son[x]] + 1;
	}
	f[x][0] = 1;
	if ( f[x][0] >= f[x][ans[x]] ) ans[x] = 0;
	fore(i,x){
		if ( fa[x] == e[i].to || son[x] == e[i].to ) continue;
		int v = e[i].to;
		f[v] = id , id += mxd[v] + 2;
		dfs(v);
		rep(j,1,mxd[v]){
			f[x][j] += f[v][j - 1];
			if ( (j < ans[x] && f[x][j] >= f[x][ans[x]]) || f[x][j] > f[x][ans[x]] ){
				ans[x] = j;
			}
		}
	}
}
int main(){
	scanf("%d",&n);
	rep(i,1,n - 1){
		int x,y;
		scanf("%d %d",&x,&y);
		adde(x,y) , adde(y,x);
	}
	dth[1] = 1 , dfs1(1);
	f[1] = id , id += mxd[1] + 2;
	dfs(1);
	rep(i,1,n) printf("%d\n",ans[i]);
}

