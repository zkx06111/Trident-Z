#include<bits/stdc++.h>
using namespace std;
#define maxn 1000020
#define rep(i,l,r) for(register int i = l ; i <= r ; i++)
#define repd(i,r,l) for(register int i = r ; i >= l ; i--)
#define rvc(i,S) for(register int i = 0 ; i < (int)S.size() ; i++)
#define rvcd(i,S) for(register int i = ((int)S.size()) - 1 ; i >= 0 ; i--)
#define pb push_back
#define prev prev_
#define stack stack_
#define inf 0x3f3f3f3f


typedef long long ll;
const ll mod = 23333333333333333ll;
int n,m;
char ch[maxn];

struct node{
    int next,to;
};
inline void up(ll &x,ll y){ x = (x + y) % mod; }
int dfn[maxn],id[maxn];
inline bool cmp(int x,int y){
    return dfn[x] < dfn[y];
}
struct Tree{
    node e[maxn];
    int head[maxn],cnt;
    int dth[maxn],tot,dfstime,vis[maxn];
    int a[maxn * 2],num[maxn * 2],mn[21][maxn * 2];
    int stack[maxn],tops,times,rt,sz[maxn],len[maxn];
    vector <int> vec;
    ll ans;

    Tree () {

    }
    void clear(){
        tops = 0 , vec.clear() , ++times;
        ans = 0;
        cnt = 0;
    }

    inline void adde(int x,int y){
        e[++cnt].to = y;
        e[cnt].next = head[x];
        head[x] = cnt;
    }
    void dfs(int x){
        dfn[x] = ++tot;
        a[tot] = x;
        for (int &i = head[x] ; i ; i = e[i].next){
    //      cout<<x<<" "<<e[i].to<<endl;
            dth[e[i].to] = dth[x] + 1;
            dfs(e[i].to);
            a[++tot] = x;
        }
    }
    void init(){
        dfs(1); //默认1为根
        rep(i,1,tot){
            num[i] = num[i - 1];
            if ( (1 << (num[i] + 1)) < i ) num[i]++;
        }
        rep(i,1,tot) mn[0][i] = a[i];
        rep(i,1,20){
            rep(j,1,tot){
                if ( (j + (1 << (i - 1)) > tot) || dth[mn[i - 1][j]] < dth[mn[i - 1][j + (1 << (i - 1))]] ) 
                    mn[i][j] = mn[i - 1][j];
                else 
                    mn[i][j] = mn[i - 1][j + (1 << (i - 1))];
            }
        }
    }
    inline int lca(int x,int y){
        int l = dfn[x], r = dfn[y];
        if ( l > r ) swap(l,r);
        int len = r - l + 1,c = num[len],id1 = mn[c][l],id2 = mn[c][r - (1 << c) + 1];
        if ( dth[id1] < dth[id2] ) return id1;
        return id2;
    }
    void insert(int x){ //建立虚树
        if ( !tops ){ stack[++tops] = x; return; }
        int v = stack[tops],lca_ = lca(v,x),w = stack[tops - 1];
        while ( tops > 1 && dth[w] > dth[lca_] ){
            adde(w,v);
            tops--;
            v = w;
            w = stack[tops - 1];
        }
        if ( v != lca_ ){
            adde(lca_,v) , tops--;
            if ( stack[tops] != lca_ ) stack[++tops] = lca_;
        }
        stack[++tops] = x;
    }
    void dfs2(int x){
    //  cout<<x<<"len: "<<len[x]<<endl;
        sz[x] = vis[x] == times ? 1 : 0;
        ll sum = 0;
        for (int &i = head[x] ; i ; i = e[i].next){ //清空技巧，树形dp用完边即清空head
    //      cout<<x<<" "<<e[i].to<<endl;
            dfs2(e[i].to);
            sz[x] += sz[e[i].to];
            up(sum,(ll)sz[e[i].to] * (sz[e[i].to] - 1) / 2);
        }
        up(ans,(ll)len[x] * (((ll)sz[x] * (sz[x] - 1) / 2 - sum) % mod));
    //  cout<<x<<" sz: "<<sz[x]<<endl;
    }
    void solve(){
        clear();
        int m,x;
        scanf("%d",&m);
        rep(i,1,m){
            scanf("%d",&x);
            x = id[n - x + 1];
    //      cout<<x<<" ";
            if ( vis[x] != times ) vec.pb(x) , vis[x] = times;
        }
    //  cout<<endl;
        sort(vec.begin(),vec.end(),cmp);
        rvc(i,vec) insert(vec[i]);
        while ( tops > 1 ) adde(stack[tops - 1],stack[tops]) , tops--;
        rt = stack[1];
        dfs2(rt);
        printf("%lld\n",ans);
    }
}tree;
struct SAM{
    int next[maxn][26],tot,pnt[maxn],val[maxn],last,sz[maxn];
    int a[maxn],b[maxn];

    void Add(int x){ //插入一个字符
        int np = ++tot ,p = last;
        val[np] = val[p] + 1 , id[val[np]] = tot + 1 , sz[np] = 1; //只有np节点有sz
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
    void pre(){
        rep(i,1,tot) tree.adde(pnt[i] + 1,i + 1) , tree.len[i + 1] = val[i];
    }
    void print(){ //打印
        for (int i = 1 ; i <= tot ; i++) cout<<i<<" "<<pnt[i]<<" "<<sz[i]<<endl;
    }
}sam;

int main(){
    freopen("input.txt","r",stdin);
    scanf("%d %d",&n,&m);
    scanf("%s",ch + 1);
    reverse(ch + 1,ch + n + 1);
    rep(i,1,n) sam.Add(ch[i] - 'a');
    sam.pre();
    tree.init();
    rep(i,1,m) tree.solve();
}
