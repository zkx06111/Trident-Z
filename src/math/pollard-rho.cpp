/*
 * Pollard-Rho�㷨��

���㷨��Ҫʹ�õĴ������ж���MillerMillerMiller-RabinRabinRabin�㷨��֮ǰ�Ѿ����۹��� 
Miller-Rabin�㷨������ 
����һ��������n������ȡ����һ����xxxʹ��xxx��nnn���������ļ��ʺ�С�������ȡ������x1x1x_1�Լ�x2x2x_2ʹ�����ǵĲ���n�������ļ��ʾ�����ˣ����ȡx1x1x_1�Լ�x2x2x_2ʹ��gcd(abs(x1?x2),n)>1gcd(abs(x1?x2),n)>1gcd(abs(x_1-x_2),n)>1�ĸ��ʾ͸����ˡ������Pollard-Rho�㷨����Ҫ˼�롣

��������gcd(abs(x1?x2),n)>1gcd(abs(x1?x2),n)>1gcd(abs(x_1-x_2),n)>1��x1x1x_1��x2x2x_2��gcd(abs(x1?x2),n)gcd(abs(x1?x2),n)gcd(abs(x_1-x_2),n)����n��һ��������ֻ��Ҫ�ж����Ƿ�Ϊ��������Ϊ����������n��������������ݹ�˹��̡�

�����ж�������ʹ��MillerMillerMiller-RabinRabinRabin�㷨��

��ô������������ȡ��x1x1x_1��x2x2x_2�أ� 
x1x1x_1������[1,n][1,n][1,n]�������������x2x2x_2����x[i]=(x[i-1]*x[i-1]%n+c)%n�������������cccΪ�������ֵ����ʵ֤�����������ǱȽ��ŵġ�

�˴�����phi��n��
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
    const int PN = 13; // �ɵ�Сһ��
    
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

