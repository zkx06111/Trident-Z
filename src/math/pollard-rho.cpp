/*
 * Pollard-Rho算法：

该算法需要使用的大素数判定的MillerMillerMiller-RabinRabinRabin算法，之前已经讨论过。 
Miller-Rabin算法戳这里 
对于一个大整数n，我们取任意一个数xxx使得xxx是nnn的质因数的几率很小，但如果取两个数x1x1x_1以及x2x2x_2使得它们的差是n的因数的几率就提高了，如果取x1x1x_1以及x2x2x_2使得gcd(abs(x1?x2),n)>1gcd(abs(x1?x2),n)>1gcd(abs(x_1-x_2),n)>1的概率就更高了。这就是Pollard-Rho算法的主要思想。

对于满足gcd(abs(x1?x2),n)>1gcd(abs(x1?x2),n)>1gcd(abs(x_1-x_2),n)>1的x1x1x_1和x2x2x_2，gcd(abs(x1?x2),n)gcd(abs(x1?x2),n)gcd(abs(x_1-x_2),n)就是n的一个因数，只需要判断它是否为素数，若为素数，则是n的质因数，否则递归此过程。

其中判断素数就使用MillerMillerMiller-RabinRabinRabin算法。

那么我们怎样不断取得x1x1x_1和x2x2x_2呢？ 
x1x1x_1在区间[1,n][1,n][1,n]中随机出来，而x2x2x_2则由x[i]=(x[i-1]*x[i-1]%n+c)%n推算出来，其中ccc为任意给定值，事实证明，这样就是比较优的。

此代码求phi（n）
*/

# include <stdio.h>
# include <string.h>
# include <iostream>
# include <algorithm>
// # include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
const int M = 5e5 + 10;
const int mod = 1e9+7;

# define RG register
# define ST static

inline ll mul(ll a, ll b, ll mod) {
    ll ret = 0;
    a %= mod, b %= mod;
    while(b) {
        if(b&1) {
            ret = ret + a;
            if(ret >= mod) ret -= mod;
        }
        a <<= 1;
        if(a >= mod) a -= mod;
        b >>= 1;
    }
    return ret;
}

inline ll pwr2(ll a, ll b, ll mod) {
    ll ret = 1;
    a %= mod;
    while(b) {
        if(b&1) ret = mul(ret, a, mod);
        a = mul(a, a, mod);
        b >>= 1;
    }
    return ret;
}

inline ll gcd(ll a, ll b) {
    return b==0 ? a : gcd(b, a%b);
}

namespace Millar_Rabin {
    const int Prime[14] = {0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};    
    const int PN = 13; // 可调小一点
    
    inline bool witness(int pr, ll res, int times, ll n) {
        ll p = pwr2((ll)pr, res, n);
        if(p == 1) return 0;   
        for (int i=0; i<times; ++i) {
            if(p == n-1) return false;
            if(p == 1) return false;
            p = mul(p, p, n);
        }
        return true;
    }
    
    inline bool main(ll n) {
        for (int i=1; i<=PN; ++i) {
            if(n == Prime[i]) return 1;
            if(n % Prime[i] == 0) return 0;
        }
        ll p = n-1;
        int times = 0;
        while(!(p&1)) {
            ++times;
            p >>= 1;
        }
        for (int i=1; i<=PN; ++i) 
            if(witness(Prime[i], p, times, n)) return false;
        return true;
    }
}

namespace PollardRho {
    const int N = 110;
    ll q[N]; int qn;    
    
    inline void PR(ll n) {
        if(Millar_Rabin::main(n)) {
            q[++qn] = n;
            return ;
        }
        ll a, b, c, del;
        while(1) {
            c = rand() % n;
            a = b = rand() % n;
            b = (mul(b, b, n) + c) % n;
            while(a != b) {
                del = a-b;
                del = gcd(abs(del), n);
                if(del > 1 && del < n) {
                    PR(del); PR(n/del);
                    return ;
                }
                a = (mul(a, a, n) + c) % n;
                b = (mul(b, b, n) + c) % n;
                b = (mul(b, b, n) + c) % n;
            }
        }
    }
    
    inline ll getphi(ll n) {
        if(n == 1) return 1ll;
        sort(q+1, q+qn+1);
        ll res = q[1] - 1;
        for (int i=2; i<=qn; ++i) {
            if(q[i] != q[i-1]) res = res * (q[i] - 1);
            else res = res * q[i];
        }
        return res;
    }
    
    inline void main(ll n) {
        qn = 0;
        PR(n);
        cout << getphi(n) << endl;
    }
}

int main() {
    srand(19260817);
    ll n; cin >> n;
    if(n == 1) {
        puts("1");
        return 0;
    }
    PollardRho::main(n);
    return 0;
}

