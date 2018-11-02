#include<bits/stdc++.h>
using namespace std;
#define maxn 100020
#define rep(i,l,r) for (register int i = l ; i <= r ; i++)
#define repd(i,r,l) for(register int i = r ; i >= l ; i--)
#define lowbit(x) (x&(-x))

typedef long long ll;
int s[maxn],c[maxn],t1[maxn * 2],t2[maxn * 2],sa[maxn],rk[maxn],h[maxn];
int n,a[maxn];
int mn[20][maxn * 2],cnt[maxn];
int mx[2][maxn];
ll ans[maxn];

//sa[i]表示排名为i的后缀的位置
    //rk[i]表示第i个后缀的排名
    //x[i]表示i的排名（第一关键字）
    //y[i]表示第二关键字排名为i的后缀的起始位置
    //h[i]表示后缀i在排序后与前一位的lcp
    //求i，j的lcp是排序后rk[i],rk[j],height的min

void suffix_array(){
    int m = n , *x = t1 , *y = t2;
    rep(i,0,m) c[i] = 0;
    rep(i,0,n - 1) c[x[i] = s[i]]++;
    rep(i,1,m) c[i] += c[i - 1];
    rep(i,0,n - 1) sa[--c[x[i]]] = i;
//  rep(i,1,n) cout<<sa[i - 1]<<" ";
//  cout<<endl;;    
    for (register int k = 1 ; k < n ; k <<= 1){
        register int p = 0;
        memset(y,0,sizeof(t1));
        repd(i,n - 1,n - k) y[p++] = i; //必须倒着for，在后面的位置更小
        rep(i,0,n - 1) if ( sa[i] >= k ) y[p++] = sa[i] - k;

        rep(i,0,m) c[i] = 0;
        rep(i,0,n - 1) c[x[y[i]]]++;
        rep(i,1,m) c[i] += c[i - 1];
        repd(i,n - 1,0) sa[--c[x[y[i]]]] = y[i];

        p = 0 , swap(x,y) , x[sa[0]] = ++p;
        rep(i,1,n - 1) x[sa[i]] = (y[sa[i]] == y[sa[i - 1]]) && (y[sa[i] + k] == y[sa[i - 1] + k]) ? p : ++p;
        if ( p >= n ) break; //如果当前已经完成排名，则break
        m = p;
    }
    rep(i,0,n - 1) rk[sa[i]] = i;
    int k = 0;
    rep(i,0,n - 1){
        if ( !rk[i] ) continue;
        int j = sa[rk[i] - 1];
        if ( k ) k--;
        while ( s[j + k] == s[i + k] ) k++;
        h[rk[i]] = k;
    }   
/*  rep(i,0,n - 1) cout<<sa[i]<<" "<<h[i]<<endl;;
    cout<<endl;;
    rep(i,0,n - 1) cout<<rk[i]<<" ";
    cout<<endl;*/
}
void pre(){
    sort(a,a + n);
    rep(i,0,n - 1) s[i] = lower_bound(a,a + n,s[i]) - a + 1;
    reverse(s,s + n);
}
//求两个后缀的最长公共前缀
//在后缀排序后的h数组中用区间RMQ查最小值
void init(){
    int k = 0;
    rep(i,0,n){
        if ( i > (1 << (k + 1)) ) k++;
        cnt[i] = k;
    }
    suffix_array();
    rep(i,0,n - 1) mn[0][i] = h[i];
    rep(i,1,17)
        rep(j,0,n - 1)
            mn[i][j] = min(mn[i - 1][j],mn[i - 1][j + (1 << (i - 1))]);
}
inline int lcp(int x,int y){
    if ( y == -1 || y >= n ) return 0;
    if ( x > y ) swap(x,y);
    x++;
    int c = cnt[y - x + 1];
    return min(mn[c][x],mn[c][y - (1 << c) + 1]);
}
//==============================================================================
inline int query(int t,int id){
    int res = -1;
    for (int i = id ; i ; i -= lowbit(i)) res = max(res,mx[t][i]);
    return res;
}
inline void insert(int t,int id){
    for (int i = id ; i <= n ; i += lowbit(i)) mx[t][i] = max(mx[t][i],id);
}
int main(){
    freopen("input.txt","r",stdin);
    scanf("%d",&n);
    rep(i,0,n - 1) scanf("%d",&s[i]) , a[i] = s[i];
    pre();
    init();
    repd(i,n - 1,0){
        int t1 = query(0,rk[i]) - 1 , t2 = n - query(1,n - rk[i] - 1);
        ans[i] = ans[i + 1] + (n - i) - max(lcp(rk[i],t1),lcp(rk[i],t2));
        insert(0,rk[i] + 1) , insert(1,n - rk[i]);
    }
    repd(i,n - 1,0) printf("%lld\n",ans[i]);
    return 0;
}
