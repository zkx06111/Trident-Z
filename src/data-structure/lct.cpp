#include<bits/stdc++.h>
using namespace std;
#define maxn 400020
#define inf 1e8
#define ls(x) ch[x][0]
#define rs(x) ch[x][1]

struct node{
	int x,y;
}e[maxn];
struct node2{
	int l,r,id;
	bool operator < (node2 a)const{
		if ( r == a.r ) return l < a.l;
		return r < a.r;
	}
};
int T,n,m,q,a[maxn];

struct LCT{
	int fa[maxn],mn[maxn],rev[maxn],ch[maxn][2],val[maxn];
	node id[maxn],rec[maxn];
	int stack_[maxn],tops,tot;

	void clear(){ 
		for (int i = 1 ; i <= tot ; i++) fa[i] = rev[i] = ch[i][0] = ch[i][1] = 0;
		for (int i = 1 ; i <= n ; i++) val[i] = mn[i] = inf; 
		tot = n;
	}
	inline bool isroot(int x){
		return (ls(fa[x]) != x) && (rs(fa[x]) != x);
	}
	inline void reverse(int x){
		if ( !x ) return;
		swap(ls(x),rs(x));
		rev[x] ^= 1;
	}
	inline void pushdown(int x){
		if ( rev[x] ){
			reverse(ls(x)) , reverse(rs(x));
			rev[x] = 0;
		}
	}
	inline void update(int x){
		mn[x] = val[x] , id[x] = rec[x];
		if ( ls(x) && mn[ls(x)] < mn[x] ) mn[x] = mn[ls(x)] , id[x] = id[ls(x)];
		if ( rs(x) && mn[rs(x)] < mn[x] ) mn[x] = mn[rs(x)] , id[x] = id[rs(x)];
	}
	inline void rotate(int x){
		int y = fa[x],t = rs(y) == x,z = ch[x][1 - t];
		if ( !isroot(y) ) ch[fa[y]][rs(fa[y]) == y] = x;
		fa[x] = fa[y] , fa[y] = x , ch[x][1 - t] = y;
		if ( z ) fa[z] = y;
		ch[y][t] = z;
		update(y);
	}
	inline void splay(int x){
		tops = 0 , stack_[++tops] = x;
		for (int i = x ; !isroot(i) ; i = fa[i]) stack_[++tops] = fa[i];
		while ( tops ) pushdown(stack_[tops--]); 
		while ( !isroot(x) ){
			int y = fa[x] , z = fa[y];
			if ( !isroot(y) && !((ls(y) == x) ^ (ls(z) == y)) ) rotate(y);
			else rotate(x);
			if ( isroot(x) )  break;
			rotate(x);
		}
		update(x);
	}
	inline void access(int x){
		int t = 0;
		for ( ; x ; x = fa[x] )
			splay(x) , ch[x][1] = t , update(x) , t = x;
	}
	inline void makeroot(int x){
		access(x) , splay(x) , reverse(x);
	}
	inline int findroot(int x){
		access(x) , splay(x);
		while ( ls(x) ) x = ls(x);
		return x;
	}
	inline void link(int x,int y){
		makeroot(x);
		makeroot(y) , splay(x) , ch[x][1] = y , fa[y] = x;
		update(x);
	}
	inline int insert(int x,int y,int t){
		if ( x == y ) return t;
		int p = findroot(x) , q = findroot(y);
		if ( p != q ){
			++tot , val[tot] = t , rec[tot] = (node){x,y};
			link(x,tot) , link(tot,y);
			return 0;
		}
		else{
			makeroot(x) , access(y) , splay(x);
			int xx = id[x].x , yy = id[x].y , res = mn[x];
			makeroot(xx) , access(yy) , splay(xx);
			ch[xx][1] = 0 , update(x) , fa[yy] = 0;
			++tot , val[tot] = t , rec[tot] = (node){x,y};
			link(x,tot) , link(tot,y);
			return res;
		}
	}
	inline void cut(int x,int y,int t){
		makeroot(x) , access(y) , splay(x);
		if ( mn[x] <= t ){
			int xx = id[x].x , yy = id[x].y;
			makeroot(xx) , access(yy) , splay(xx);
			ch[xx][1] = 0 , update(x) , fa[yy] = 0;
		}
	}
}lct;
struct segmentTree{
	int root[maxn],sum[maxn * 2],tot,ch[maxn * 2][2];
	void clear(){
		for (int i = 1 ; i <= m ; i++) root[i] = 0;
		for (int i = 1 ; i <= tot ; i++) ch[i][0] = ch[i][1] = sum[i] = 0;
		tot = 0;
	}
	inline void update(int x){
		sum[x] = sum[ls(x)] + sum[rs(x)];
	}
	int insert(int x,int l,int r,int d){
		int cur = ++tot;
		if ( l == r ){ sum[cur] = sum[x] + 1; return cur; }
		int mid = (l + r) >> 1;
		ls(cur) = ls(x) , rs(cur) = rs(x);
		if ( d <= mid ) ls(cur) = insert(ls(x),l,mid,d);
		else rs(cur) = insert(rs(x),mid + 1,r,d);
		update(cur);
		return cur;
	}
	int query(int x,int l,int r,int ls,int rs){
		if ( !x ) return 0;
		if ( ls <= l && rs >= r ) return sum[x];
		int mid = (l + r) >> 1 , res = 0;
		if ( ls <= mid ) res = query(ls(x),l,mid,ls,rs);
		if ( rs > mid ) res += query(rs(x),mid + 1,r,ls,rs);
		return res;
	}
}sgt;
void clear(){
	lct.clear() , sgt.clear();
}
int main(){		
	freopen("input.txt","r",stdin);
	scanf("%d",&T);
	while ( T-- ){
		scanf("%d %d %d",&n,&m,&q);
		clear();
		for (int i = 1 ; i <= m ; i++){ 
			scanf("%d %d",&e[i].x,&e[i].y);
			a[i] = lct.insert(e[i].x,e[i].y,i);
			if ( a[i] ) sgt.root[i] = sgt.insert(sgt.root[i - 1],1,m,a[i]);
			else sgt.root[i] = sgt.root[i - 1];
		}
		for (int i = 1 ; i <= m ; i++) cout<<a[i]<<" ";
		cout<<endl;
		for (int i = 1 ; i <= q ; i++){
			int l,r;
			scanf("%d %d",&l,&r);
			printf("%d\n",n - (r - l + 1) + sgt.query(sgt.root[r],1,m,l,r));
		}
	}
	return 0;
}


