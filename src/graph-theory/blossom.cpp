#include<bits/stdc++.h>
using namespace std;
#define maxn 1020
#define maxm 200020

struct node{
	int next_,to;
}e[maxm * 2];
int head[maxn],cnt;
int match[maxn],next_[maxn],q[maxn * 200],hh,tt;
int n,m,tot,col[maxn],fa[maxn],dfstime,vis[maxn];

inline void adde(int x,int y){
	e[++cnt].to = y;
	e[cnt].next_ = head[x];
	head[x] = cnt;
}
inline int getfa(int x){
	if ( x == fa[x] ) return x;
	return fa[x] = getfa(fa[x]);
}
inline int lca(int x,int y){
	++dfstime;
	while ( true ){
		if ( x ){
			x = getfa(x);
			if ( vis[x] == dfstime ) return x;
			vis[x] = dfstime;
			if ( match[x] ) x = next_[match[x]];
			else x = 0;
		}
		swap(x,y);
	}
}
inline void unit(int a,int p){
	while ( a != p ){
		int b = match[a] , c = next_[b];
		if ( getfa(c) != p && c != p ) next_[c] = b;
		if ( col[c] == 2 ) col[q[tt++] = c] = 1;
		if ( col[b] == 2 ) col[q[tt++] = b] = 1;
		fa[getfa(a)] = getfa(c) , fa[getfa(b)] = getfa(c);
		a = c;
	}
}
inline void bfs(int now){
	hh = tt = 0;
	for (int i = 1 ; i <= n ; i++) next_[i] = col[i] = 0 ,fa[i] = i;
	q[tt++] = now;
	col[now] = 1;
	while ( hh < tt ){
		now = q[hh++];
		for (int i = head[now] ; i ; i = e[i].next_){
			if ( match[now] == e[i].to ) continue;
			if ( getfa(now) == getfa(e[i].to) ) continue; // 不考虑在同一个奇环内增广
			if ( col[e[i].to] == 2 ) continue; //忽略已搜索过的Y部的点
			if ( col[e[i].to] == 1 ){
				int k = lca(now,e[i].to);
				if ( k != getfa(now) ) next_[now] = e[i].to;
				if ( k != getfa(e[i].to) ) next_[e[i].to] = now;
				unit(now,k) , unit(e[i].to,k);
			}
			else{
				next_[e[i].to] = now;
				if ( match[e[i].to] == 0 ){
					for (int a = e[i].to ; a ; ){
						int u = next_[a];
						int v = match[u];
						match[a] = u , match[u] = a;
						a = v;
					}
					return;
				}
				else{
					col[e[i].to] = 2;
					q[tt++] = match[e[i].to] , col[match[e[i].to]] = 1;
				}
			}
		}
	}
}	
int main(){
	scanf("%d %d",&n,&m);
	for (int i = 1 ; i <= m ; i++){
		int u,v;
		scanf("%d %d",&u,&v);
		adde(u,v);
		adde(v,u);
	}
	for (int i = 1 ; i <= n ; i++){
		if ( !match[i] ){
			bfs(i);
		}
	}
	for (int i = 1 ; i <= n ; i++) if ( match[i] ) tot++;
	printf("%d\n",tot / 2);
	for (int i = 1 ; i <= n ; i++){
		printf("%d ",match[i]);
	}
	return 0;
}

