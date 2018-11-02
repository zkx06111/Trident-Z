#include<bits/stdc++.h>
using namespace std;
#define maxn 200020
#define rep(i,l,r) for(register int i = l ; i <= r ; i++)
#define repd(i,r,l) for(register int i = r ; i >= l ; i--)
#define rvc(i,S) for (register int i = 0 ; i < (int)S.size() ; i++)
#define inf 1e8
#define N 6000020

typedef long long ll;
const ll mod = 1e9 + 7;
struct node{
    int next,to;
}e[maxn];
int head[maxn],num,fa[maxn],sz[maxn];
int n,root,mx[maxn],mn[maxn],mx2[maxn],id[maxn],ans[maxn];

int ls[N],rs[N],cnt[N],rt[maxn],tot;
vector <int> vec;
vector <int>::iterator it;

inline void adde(int x,int y){
    e[++num].to = y;
    e[num].next = head[x];
    head[x] = num;
}
inline void copy(int x,int y){
    cnt[x] = cnt[y] , ls[x] = ls[y] , rs[x] = rs[y];
}
inline void update(int x){
    cnt[x] = cnt[ls[x]] + cnt[rs[x]];
}
int merge(int x,int y,int l,int r){
    if ( !x && !y ) return 0;
    int cur = ++tot;
    if ( !x ){ copy(cur,y); return cur; }
    if ( !y ){ copy(cur,x); return cur; }
    if ( l == r ){ 
        cnt[cur] = cnt[x] + cnt[y];
        return cur;
    }
    int mid = (l + r) >> 1;
    ls[cur] = merge(ls[x],ls[y],l,mid);
    rs[cur] = merge(rs[x],rs[y],mid + 1,r);
    update(cur);
    return cur;
}
void print(int x,int l,int r){
    if ( l == r ){
        cout<<cnt[x]<<" ";
        return;
    }
    int mid = (l + r) >> 1;
    print(ls[x],l,mid);
    print(rs[x],mid  + 1,r);
}
int insert(int x,int l,int r,int id){
    int cur = ++tot;
    if ( l == r ){ cnt[cur] = cnt[x] + 1; return cur; }
    int mid = (l + r) >> 1;
    copy(cur,x);
    if ( id <= mid ) ls[cur] = insert(ls[x],l,mid,id);
    else rs[cur] = insert(rs[x],mid + 1,r,id);
    update(cur);
    return cur;
}
void modify(int &x,int l,int r,int id,int d){
    if ( !x ) x = ++tot;
    if ( l == r ){ cnt[x] += d ; return; }
    int mid = (l + r) >> 1;
    if ( id <= mid ) modify(ls[x],l,mid,id,d);
    else modify(rs[x],mid + 1,r,id,d);
    update(x);
}

void dfs(int x){
    sz[x] = 1 , mn[x] = inf;
    for (int i = head[x] ; i ; i = e[i].next){
        dfs(e[i].to);
        rt[x] = merge(rt[x],rt[e[i].to],1,n);
        sz[x] += sz[e[i].to];
        if ( sz[e[i].to] > mx[x] ) mx2[x] = mx[x] , id[x] = e[i].to , mx[x] = sz[e[i].to];
        else if ( sz[e[i].to] > mx2[x] ) mx2[x] = sz[e[i].to];
        mn[x] = min(mn[x],sz[e[i].to]);
    }
    rt[x] = insert(rt[x],1,n,sz[x]);
/*  cout<<x<<endl;
    print(rt[x],1,n);
    cout<<endl;*/
}
int queryl(int x,int y,int l,int r,int id){
    if ( !x || !id ) return 0;
    if ( l == r ){
        if ( cnt[x] > cnt[y] ) return l;
        return 0;
    }
    int mid = (l + r) >> 1;
    if ( id <= mid ) return queryl(ls[x],ls[y],l,mid,id);
    int d = queryl(rs[x],rs[y],mid + 1,r,id);
    if ( d ) return d;
    if ( cnt[ls[x]] > cnt[ls[y]] ) return queryl(ls[x],ls[y],l,mid,id);
    return 0;
}
int queryr(int x,int y,int l,int r,int id){
    if ( !x ) return 0;
    if ( l == r ){
        if ( cnt[x] > cnt[y] ) return l;
        return 0;
    }
    int mid = (l + r) >> 1;
    if ( id > mid ) return queryr(rs[x],rs[y],mid + 1,r,id);
    int d = queryr(ls[x],ls[y],l,mid,id);
    if ( d ) return d;
    if ( cnt[rs[x]] > cnt[rs[y]] ) return queryr(rs[x],rs[y],mid + 1,r,id);
    return 0;
}
inline bool cmp(int x,int y){ return x > y; }
inline void getans(int x,int fsz){
    //льепр╤вс
    if ( !fa[x] && !mx2[x] ){ ans[x] = n - 1; return; }
    if ( sz[x] == 1 ){ ans[x] = n - 1; return; }
    if ( fsz ) mn[x] = min(mn[x],fsz);
    int cur = 0;
    if ( mx[x] > fsz ){
        mx2[x] = max(mx2[x],fsz);
        int d = (mx[x] - mn[x]) / 2;
        int fir = queryl(rt[id[x]],0,1,n,d),last = queryr(rt[id[x]],0,1,n,d);
        cur = min(mx[x],min(max(mx2[x],max(mn[x] + fir,mx[x] - fir)),max(mx2[x],max(mn[x] + last,mx[x] - last))));
    }
    else{
        int d = (fsz - mn[x]) / 2;
        int fir = queryl(rt[root],rt[x],1,n,d),last = queryr(rt[root],rt[x],1,n,d);
        if ( !last ) last = inf;
        it = lower_bound(vec.begin(),vec.end(),d + sz[x],cmp);
    //  rvc(i,vec) cout<<vec[i]<<" ";
    //  cout<<*it<<endl;
        if ( it != vec.end() ) fir = max(fir,(*it) - sz[x]);
        if ( it != vec.begin() ){
            it--;
            last = min(last,(*it) - sz[x]);
        }
        cur = min(fsz,min(max(mx[x],max(mn[x] + fir,fsz - fir)),max(mx[x],max(mn[x] + last,fsz - last))));
    }
    ans[x] = cur;
}
void dfs2(int x){
    getans(x,n - sz[x]);
    vec.push_back(sz[x]);
    modify(rt[root],1,n,sz[x],-1);
    for (int i = head[x] ; i ; i = e[i].next){
        dfs2(e[i].to);
    }
    modify(rt[root],1,n,sz[x],1);
    vec.pop_back();
}
int main(){
    freopen("input.txt","r",stdin);
    scanf("%d",&n);
    rep(i,1,n){
        int x,y;
        scanf("%d %d",&x,&y);
        if ( !x ) root = y;
        else fa[y] = x,adde(x,y);
    }
    dfs(root);
/*  rep(i,1,n){
        cout<<i<<endl;
    print(rt[i],1,n);
    cout<<endl;
    }*/
    dfs2(root);
    rep(i,1,n) printf("%d\n",ans[i]);
    //printf("\n");
    return 0;
}
