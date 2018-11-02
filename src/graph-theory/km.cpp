//KM算法
//解决二分图最大权匹配（如果两边点数不一样加虚点，不存在的边边权为0）
//最小权将边权取负
//O(n^3)
//比费用流快
#pragma GCC optimize(3)
#include<bits/stdc++.h>
#define rep(i,a,b) for(int i=a;i<=b;++i)
#define repd(i,a,b) for(int i=a;i>=b;--i)
#define per(i,a,b) for(int i=a;i>=b;--i)
#define rvc(i,S) for(int i=0;i<S.size();++i)
//#define mp make_pair
#define pb push_back
#define fi first
#define se second
#define debug(...) fprintf(stderr,__VA_ARGS__)
using namespace std;
#define maxn 2020
#define maxm 2000020
#define inf 0x3f3f3f3f

struct node{
    int next,to,from,f,ct;
}e[maxm * 2];
struct point{
    int x,y;
}wine[maxn],peo[maxn],res;
int head[maxn],cnt = 1;
int w[maxn][maxn],x[maxn],y[maxn],prev_x[maxn],prev_y[maxn],son_y[maxn],slack[maxn],par[maxn];
int lx,ly,pop;
int n,m,ans,mn = inf,maxcost;

int dist(point p1,point p2){
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}
inline void adjust(int v){ //调整匹配，prev记录交替轨，son_y记录匹配
    son_y[v] = prev_y[v];
    if ( prev_x[son_y[v]] != -2 ) adjust(prev_x[son_y[v]]);
}
inline bool find(int v){ //查找左边点的匹配
    rep(i,1,pop){
        if ( prev_y[i] == -1 ){
            if ( slack[i] > x[v] + y[i] - w[v][i] ){
                slack[i] = x[v] + y[i] - w[v][i];
                par[i] = v;
            }
            if ( x[v] + y[i] == w[v][i] ){
                prev_y[i] = v;
                if ( son_y[i] == -1 ){
                    adjust(i);
                    return 1;
                }
                if ( prev_x[son_y[i]] != -1 ) continue;
                prev_x[son_y[i]] = i;
                if ( find(son_y[i]) ) return 1;
            }
        }
    }
    return 0;
}
void km(){
    int m;
    rep(i,1,pop){
        son_y[i] = -1;
        y[i] = 0;
    }
    rep(i,1,pop){
        x[i] = 0;
        rep(j,1,pop){
            x[i] = max(x[i],w[i][j]);
        }
    }
    bool flag;
    rep(i,1,pop){
        rep(j,1,pop){
            prev_x[j] = prev_y[j] = -1;
            slack[j] = inf;
        }
        prev_x[i] = -2;
        if ( find(i) ) continue;
        flag = 0;
        while ( !flag ){ //不断增广边直到有新点被匹配
            m = inf;
            rep(j,1,pop) if ( prev_y[j] == -1) m = min(m,slack[j]);
            rep(j,1,pop){
                if ( prev_x[j] != -1 ) x[j] -= m;
                if ( prev_y[j] != -1 ) y[j] += m;
                else slack[j] -= m;
            }
            rep(j,1,pop){
                if ( prev_y[j] == -1 && !slack[j] ){
                    prev_y[j] = par[j];
                    if ( son_y[j] == -1 ){
                        adjust(j);
                        flag = 1;
                        break;
                    }
                    prev_x[son_y[j]] = j;
                    if ( find(son_y[j]) ){
                        flag = 1;
                        break;
                    }
                }
            }
        }
    }
    rep(i,1,pop) maxcost += w[son_y[i]][i];
}
int main(){
    scanf("%d %d",&n,&m);
    rep(i,1,n) scanf("%d %d",&wine[i].x,&wine[i].y);
    rep(i,1,m) scanf("%d %d",&peo[i].x,&peo[i].y);
    scanf("%d %d",&res.x,&res.y);
    rep(i,1,n) ans += dist(res,wine[i]) * 2;
    rep(i,1,m){
        rep(j,1,n){
            int d1 = dist(res,wine[j]),d2 = dist(peo[i],wine[j]);
            if ( d1 > d2 ) w[i][j] = d1 - d2;   
            else mn = min(mn,d2 - d1);
        }
    }   
    pop = max(n,m);
    km();
//  if ( maxcost > 0 ) ans -= maxcost;
//  else ans += mn;
    if ( maxcost ) ans -= maxcost;
    else ans += mn;
    printf("%d\n",ans);
    return 0;
}
