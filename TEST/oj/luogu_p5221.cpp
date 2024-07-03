#include "IO/FastIO.h"
#include "MATH/Eratosthenes.h"
#include "MATH/Mobius.h"
#include "MATH/SqrtDecomposition.h"
#include "MATH/StaticModInt32.h"

/*
[P5221 Product](https://www.luogu.com.cn/problem/P5221)
*/
/**
 *  求                  \Prod_{i=1}^n \Prod_{j=1}^n lcm(i,j)/gcd(i,j)
 *  我们更习惯枚举 gcd
 *  即                  \Prod_{i=1}^n \Prod_{j=1}^n (i*j/gcd(i,j)/gcd(i,j))
 *  因为是求 Prod 所以都可以拆出来
 *  即                  \Prod_{i=1}^n \Prod_{j=1}^n (i*j) (\Prod_{i=1}^n \Prod_{j=1}^n gcd(i,j))^(-2)
 *
 *  显然，前面两项很好算
 *  即                  \Prod_{i=1}^n (i^n) \Prod_{j-1} j
 *  即                  (n!)^n * (n!)^n
 *  即                  (n!)^(2*n)
 *
 *  考虑后半部分，且先忽略 -2 次
 *  即求 f(n)=          \Prod_{i=1}^n \Prod_{j=1}^n gcd(i,j)
 *  按套路，枚举 d      \Prod_{d=1}^n d^(\Sigma_{i=1}^n \Sigma_{j=1}^n [gcd(i,j)==d])
 *  即                  \Prod_{d=1}^n d^(\Sigma_{i=1}^(n/d) \Sigma_{j=1}^(n/d) [gcd(i,j)==1])
 *
 *  再把后面一堆弄出来
 *  设 g(n)=            \Sigma_{i=1}^n \Sigma_{i=1}^n [gcd(i,j)==1]
 *  这个式子在 P2158 里进行过分析，可以莫比乌斯求解，也可以欧拉函数求解
 */

static constexpr uint32_t N = 1000000;
using mint = OY::StaticModInt32<104857601, true>;
using mint2 = OY::StaticModInt32<104857600, false>;
uint64_t phi_presum[N + 1];
void solve_phi() {
    uint32_t n;
    cin >> n;
    OY::Eratosthenes::Sieve<N, true, false, false> es(n);
    for (uint32_t i = 1; i <= n; i++) phi_presum[i] = phi_presum[i - 1] + es.get_Euler_Phi(i);
    mint fac{1};
    mint f{1};
    for (auto &&fd_range : OY::SqrtDecomposition<uint32_t>(n)) {
        // 算出 f 函数的底数乘积
        uint32_t fd_min = fd_range.left(), fd_max = fd_range.right();
        mint fd{1};
        for (uint32_t d = fd_min; d <= fd_max; d++) fd *= d;
        fac *= fd;
        // 计算指数 g ，注意指数位置的 mint 模数不同
        mint2 g = phi_presum[fd_range.quot()] * 2 - 1;
        f *= fd.pow(g.val());
    }

    cout << (fac.pow(2 * n) / f / f);
}

void solve_mobius() {
    uint32_t n;
    cin >> n;
    OY::MOBIUS::Table<N, true> Mu(n);
    mint fac = 1;
    for (uint32_t i = 1; i <= n; i++) fac *= i;
    mint f{1};
    for (auto &&fd_range : OY::SqrtDecomposition<uint32_t>(n)) {
        // 算出 f 函数的底数乘积
        uint32_t fd_min = fd_range.left(), fd_max = fd_range.right();
        mint fd{1};
        for (uint32_t d = fd_min; d <= fd_max; d++) fd *= d;
        // 计算指数 g ，注意指数位置的 mint 模数不同
        mint2 g{};
        for (auto &&gd_range : OY::SqrtDecomposition<uint32_t>(fd_range.quot()))
            g += mint2(Mu.query_mobius(gd_range.left(), gd_range.right())) * gd_range.quot() * gd_range.quot();
        f *= fd.pow(g.val());
    }

    cout << (fac.pow(2 * n) / f / f);
}

/**
 *  上文推到，原式等于          \Prod_{i=1}^n \Prod_{j=1}^n (i*j) (\Prod_{i=1}^n \Prod_{j=1}^n gcd(i,j))^(-2)
 *  即                          (n!)^(2*n) (\Prod_{i=1}^n \Prod_{j=1}^n gcd(i,j))^(-2)
 *  设 f(n)=                    \Prod_{i=1}^n \Prod_{j=1}^n gcd(i,j)
 *  即                          \Prod_{d=1}^n d^(\Sigma_{i=1}^(n/d) \Sigma_{j=1}^(n/d) [gcd(i,j)==1])
 *  即                          \Prod_{d=1}^n d^(\Sigma_{i=1}^(n/d) \Sigma_{j=1}^(n/d) \Sigma_{e|gcd(i,j)} \mu(e))
 *  即                          \Prod_{d=1}^n d^(\Sigma_{e=1}^(n/d) \mu(e)\Sigma_{i=1}^(n/d/e) \Sigma_{j=1}^(n/d/e))
 *  把 d*e 设为一个整体 S=d*e
 *  即                          \Prod_{S=1}^n \Prod_{d|S} d^(\mu(S/d)\Sigma_{i=1}^(n/S) \Sigma_{j=1}^(n/S))
 *
 *  设 g(x)=\Prod_{d|x} d^(\mu(x/d))，g 与 n 无关可以预处理
 *  f(n)等于                    \Prod_{S=1}^n g(S)^((n/S)*(n/S))
 * 
 *  本解法可用于多测
 */
mint fact[N + 1], g_pre_prod[N + 1];
struct Solver2 {
    void prepare(uint32_t mx) {
        OY::MOBIUS::Table<N + 1, false> Mu(mx);
        fact[0] = g_pre_prod[0] = 1;
        for (uint32_t i = 1; i <= mx; i++) fact[i] = fact[i - 1] * i, g_pre_prod[i] = 1;
        for (uint32_t i = 1; i <= mx; i++) {
            mint base = mint::raw(i), mul[3] = {base.inv(), 1, base};
            for (uint32_t j = i, k = 1; j <= mx; j += i, k++) g_pre_prod[j] *= mul[Mu.query_mobius(k) + 1];
            g_pre_prod[i] *= g_pre_prod[i - 1];
        }
    }
    mint solve(uint32_t n) {
        mint ans = fact[n].pow(n);
        for (auto &&S_range : OY::SqrtDecomposition<uint32_t>(n)) {
            uint32_t l = S_range.left(), r = S_range.right();
            ans /= (g_pre_prod[r] / g_pre_prod[l - 1]).pow(uint64_t(S_range.quot()) * S_range.quot());
        }
        return ans * ans;
    }
};
void solve_mobius2() {
    uint32_t n;
    cin >> n;
    Solver2 sol;
    sol.prepare(n);
    cout << sol.solve(n) << endl;
}

int main() {
    solve_phi();
    // solve_mobius();
    // solve_mobius2();
}
