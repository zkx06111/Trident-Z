
#include<bits/stdc++.h>
using namespace std;
#define maxn 600020
 
typedef long long ll;
 
struct SAM{
	int next[maxn][26],tot,pnt[maxn],val[maxn],last,sz[maxn];
	int a[maxn],b[maxn],jump[20][maxn];
	void Add(int x){ //插入一个字符
		int np = ++tot ,p = last;
		val[np] = val[p] + 1 , sz[np] = 1; //只有np节点有sz
		while ( !next[p][x] && p ) next[p][x] = np , p = pnt[p];
		int q = next[p][x];
		if ( !q ) pnt[np] = p , next[p][x] = np;
		else if ( val[p] + 1 == val[q] ) pnt[np] = q;
		else{
			int nq = ++tot;
			val[nq] = val[p] + 1;
			pnt[nq] = pnt[q];
			pnt[np] = pnt[q] = nq;
			memcpy(next[nq],next[q],sizeof(next[q]));
			while ( next[p][x] == q && p ) next[p][x] = nq , p = pnt[p];
			if ( next[p][x] == q ) next[p][x] = nq;
		}	
		last = np;
	}
	void getsize(){ //桶排序（相同于拓扑序），求right集合大小
		for (int i = 1 ; i <= tot ; i++) a[val[i]]++;
		for (int i = 1 ; i <= n ; i++) a[i] += a[i - 1];
		for (int i = tot ; i >= 1 ; i--) b[a[val[i]]--] = i;
		for (int i = tot ; i >= 1 ; i--) sz[pnt[b[i]]] += sz[b[i]];
	}
	void pre(){ //预处理倍增
		for (int i = 1 ; i <= tot ; i++) jump[0][i] = pnt[i];
		for (int i = 1 ; i <= 17 ; i++)
			for (int j = 1 ; j <= tot ; j++)
				jump[i][j] = jump[i - 1][jump[i - 1][j]];
	}
	void print(){ //打印
		for (int i = 1 ; i <= tot ; i++) cout<<i<<" "<<pnt[i]<<" "<<sz[i]<<endl;
	}
}sam;
	
void init(){
	scanf("%s",ch + 1);
	for (int i = 1 ; i <= n ; i++) sam.Add(ch[i] - 'a');
	sam.getsize();
	sam.pre();
//	sam.print();
}
int main(){

	init();

	return 0;
}


