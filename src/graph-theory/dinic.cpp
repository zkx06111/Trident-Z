//检验混合途欧拉回路
#include<bits/stdc++.h>
using namespace std;
#define maxn 100020
#define rep(i,l,r) for(register int i = l ; i <= r ; i++)
#define repd(i,r,l) for(register int i = r ; i >= l ; i--)
#define rvc(i,S) for(register int i = 0 ; i < (int)S.size() ; i++)
#define rvcd(i,S) for(register int i = ((int)S.size()) - 1 ; i >= 0 ; i--)
#define fore(i,x) for(register int i = head[x] ; i ; i = e[i].next)
#define pb push_back
#define prev prev_
#define stack stack_
#define inf 0x3f3f3f3f

typedef long long ll;

struct node{
	int next,to,f;
}e[maxn * 2];
int head[maxn],cnt = 1,cur[maxn];
int q[maxn],hh,tt,S,T,maxflow,dis[maxn],sum;

int deg[maxn],n,m,t,bl;

void clear(){
	bl = 1 , cnt = 1;
	rep(i,1,T) deg[i] = head[i] = 0;
	sum = maxflow = 0;
}
inline void adde(int x,int y,int c){
	e[++cnt].to = y;
	e[cnt].next = head[x];
	e[cnt].f = c;
	head[x] = cnt;
	e[++cnt].to = x;
	e[cnt].next = head[y];
	e[cnt].f = 0; //多组数据一定要把每个变量都清空
	head[y] = cnt;
}
bool bfs(){
	tt = hh = 0;
	rep(i,1,T) dis[i] = 0;
	dis[S] = 1 , q[tt++] = S; //初值设为1，否则会被迷之更新而死循环，从来没有犯过的错误
	while ( hh < tt ){
		int x = q[hh++];
		for (int i = head[x] ; i ; i = e[i].next){
			if ( e[i].f && !dis[e[i].to] ){
				dis[e[i].to] = dis[x] + 1;
				q[tt++] = e[i].to;
			}
		}
	}
	return dis[T];
}
int dfs(int x,int delta){
	if ( !delta || x == T ) return delta;
	int res = 0;
	for (int &i = cur[x] ; i ; i = e[i].next){
		if ( e[i].f && dis[e[i].to] == dis[x] + 1 ){
			int d = dfs(e[i].to,min(delta,e[i].f));
			e[i].f -= d , e[i ^ 1].f += d;
			delta -= d, res += d;
			if ( !delta ) return res;
		}
	}
	if ( delta ) dis[x] = -1;
	return res;
}
int main(){
//	freopen("input.txt","r",stdin);
	scanf("%d",&t);
	while ( t-- ){
		clear();
		scanf("%d %d",&n,&m);
		rep(i,1,m){
			int x,y,d;
			scanf("%d %d %d",&x,&y,&d);
			deg[x]++ , deg[y]--;
			if ( !d ) adde(x,y,1);
		}
		S = n + 1, T = n + 2;
		rep(i,1,n){
			int d = abs(deg[i]);
			if ( d & 1 ){ bl = 0; break; }
			if ( deg[i] > 0 ) adde(S,i,deg[i] / 2) , sum += deg[i] / 2;
			else if ( deg[i] < 0 ) adde(i,T,-deg[i] / 2);
		}
		if ( !bl ){ 
			cout<<"impossible\n";
			continue;
		}
		while ( bfs() ){
			rep(i,1,T) cur[i] = head[i];
			maxflow += dfs(S,inf);
		}
	//	cout<<maxflow<<endl;
		if ( maxflow == sum ) cout<<"possible\n";
		else cout<<"impossible\n";
	}

}

