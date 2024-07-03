#include "IO/FastIO.h"
#include "MATH/Mobius.h"
#include "MATH/PairSqrtDecomposition.h"
#include "MATH/StaticModInt32.h"

/*
[P3704 [SDOI2017] 数字表格](https://www.luogu.com.cn/problem/P3704)
*/
/**
 *  假定 n>=m
 *  求                  \Prod_{i=1}^n \Prod_{j=1}^m f(gcd(i,j))
 *  按套路枚举 gcd(i,j)
 *  即                  \Prod_{d=1}^m f(d)^(\Sigma_{i=1}^n \Sigma_{j=1}^m [gcd(i,j)==d])
 *  即                  \Prod_{d=1}^m f(d)^(\Sigma_{i=1}^(n/d) \Sigma_{j=1}^(m/d) [gcd(i,j)==1])
 *
 *  把指数拎出来
 *  设 g(n,m)=          \Sigma_{i=1}^n \Sigma_{j=1}^m [gcd(i,j)==1]
 *  这个式子在 P2257 里进行过分析
 *  根据莫比乌斯函数的结论， [x==1]=\Sigma_{d|x} \mu(d)
 *  g(n,m)=             \Sigma_{i=1}^n \Sigma_{j=1}^m \Sigma_{d|gcd(i,j)} \mu(d)
 *  即                  \Sigma_{i=1}^n \Sigma_{j=1}^m \Sigma_{d=1}^m \mu(d) [d|gcd(i,j)]
 *  此时，我们终于可以调换顺序
 *  即                  \Sigma_{d=1}^m \mu(d) \Sigma_{i=1}^n \Sigma_{j=1}^m [d|gcd(i,j)]
 *  即                  \Sigma_{d=1}^m \mu(d) \Sigma_{i=1}^(n/d) \Sigma_{j=1}^(m/d)
 *  即                  \Sigma_{d=1}^m \mu(d) (n/d) (m/d)
 *  通过数论分块，g 可以根号求解
 *
 *  内层数论分块，外层也数论分块，我们惊喜地发现竟然就这么过了
 *  显然这是不正确的时间复杂度，我们继续优化
 *  原式为              \Prod_{d=1}^m f(d)^g(n/d,m/d)
 *  即                  \Prod_{d=1}^m f(d)^(\Sigma_{e=1}^m} \mu(e) (n/d/e) (m/d/e))
 *  把 d*e 乘积看成一个整体 S=d*e
 *  即                  \Prod_{d=1}^m f(d)^(\Sigma_{e=1}^m} \mu(e) (n/S) (m/S))
 *  即                  \Prod_{S=1}^m \Prod_{d|S} f(d)^(\mu(S/d) (n/S) (m/S))
 *  即                  \Prod_{S=1}^m (\Prod_{d|S} f(d)^\mu(S/d))^((n/S) (m/S))
 *  最外面的指数 (n/S)(m/S) 可以数论分块
 *  里面的底数与 n m 无关可以预处理
 *  设 h(S)=\Prod_{d|S} f(d)^\mu(S/d)
 *  原式等于            \Prod_{S=1}^m h(S)^((n/S)(m/S))
 */

using mint = OY::mint1000000007;
static constexpr uint32_t N = 1000000;
mint hs_pre_prod[N + 1][2];
void solve_mobius() {
    uint32_t t;
    cin >> t;
    std::vector<std::pair<uint32_t, uint32_t>> qs(t);
    uint32_t mx{};
    for (auto &[n, m] : qs) cin >> n >> m, mx = std::max({mx, n, m});
    // 边预处理斐波那契数列
    mint pre = 0, fibbo = 1;
    // 边枚举 d 计算 h(S) 并计算 h(S) 的前缀积
    for (uint32_t d = 1; d <= mx; d++) hs_pre_prod[d][0] = 1;
    OY::MOBIUS::Table<N, true> Mu(mx);
    hs_pre_prod[0][0] = hs_pre_prod[0][1] = 1;
    for (uint32_t d = 1; d <= mx; d++) {
        mint val[3] = {fibbo.inv(), 1, fibbo};
        fibbo += pre, pre = val[2];
        for (uint32_t j = 1, k = d; k <= mx; j++, k += d) hs_pre_prod[k][0] *= val[Mu.query_mobius(j) + 1];
        hs_pre_prod[d][0] *= hs_pre_prod[d - 1][0];
        hs_pre_prod[d][1] = hs_pre_prod[d][0].inv();
    }
    for (auto &&[n, m] : qs) {
        if (n < m) std::swap(n, m);
        mint ans{1};
        // 数论分块枚举 S
        for (auto &&S_range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            mint base = hs_pre_prod[S_range.right()][0] * hs_pre_prod[S_range.left() - 1][1];
            ans *= base.pow(uint64_t(S_range.quot1()) * S_range.quot2());
        }
        cout << ans << endl;
    }
}

int main() {
    solve_mobius();
}
