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

const int MAXN = 65;
ll x[MAXN],n;
vector<ll> f;

ll multi(ll a, ll b, ll p) {
    ll ans = 0;
    while(b) {
        if(b&1LL) ans = (ans+a)%p;
        a = (a+a)%p;
        b >>= 1;
    }
    return ans;
}

ll qpow(ll a, ll b, ll p) {
    ll ans = 1;
    while(b) {
        if(b&1LL) ans = multi(ans, a, p);
        a = multi(a, a, p);
        b >>= 1;
    }
    return ans;
}

bool Miller_Rabin(ll n) {
    if(n == 2) return true;
    int s = 20, i, t = 0;
    ll u = n-1;
    while(!(u & 1)) {
        t++;
        u >>= 1;
    }
    while(s--) {
        ll a = rand()%(n-2)+2;
        x[0] = qpow(a, u, n);
        for(i = 1; i <= t; i++) {
            x[i] = multi(x[i-1], x[i-1], n);
            if(x[i] == 1 && x[i-1] != 1 && x[i-1] != n-1) return false;
        }
        if(x[t] != 1) return false;
    }
    return true;
}

ll gcd(ll a, ll b) {
    return b ? gcd(b, a%b) : a;
}

ll Pollard_Rho(ll n, int c) {
    ll i = 1, k = 2, x = rand()%(n-1)+1, y = x;
    while(true) {
        i++;
        x = (multi(x, x, n) + c)%n;
        ll p = gcd((y-x+n)%n, n);
        if(p != 1 && p != n) return p;
        if(y == x) return n;
        if(i == k) {
            y = x;
            k <<= 1;
        }
    }
}

void find(ll n, int c) {
    if(n == 1) return;
    if(Miller_Rabin(n)) {
        f.push_back(n);
        return;
    }
    ll p = n, k = c;
    while(p >= n) p = Pollard_Rho(p, c--);
    find(p, k);
    find(n/p, k);
}
int main(){
	while ( cin>>n ){
		find(n,20000907);
		rvc(i,f) cout<<f[i]<<" ";
		cout<<endl;
	}
}
