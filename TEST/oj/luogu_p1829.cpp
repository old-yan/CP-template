#include "IO/FastIO.h"
#include "MATH/Mobius.h"
#include "MATH/PairSqrtDecomposition.h"
#include "MATH/StaticModInt32.h"

/*
[P1829 [国家集训队] Crash的数字表格 / JZPTAB](https://www.luogu.com.cn/problem/P1829)
*/
/**
 *  假定 n>=m
 *  求                  \Sigma_{i=1}^n \Sigma_{j=1}^m lcm(i,j)
 *  我们更习惯枚举 gcd
 *  即                  \Sigma_{i=1}^n \Sigma_{j=1}^m (i*j/gcd(i,j))
 *  把 gcd 作为枚举项
 *  即                  \Sigma_{d=1}^m \Sigma_{i=1}^n \Sigma_{j=1}^m [d==gcd(i,j)] (i*j/d)
 *  显然，d 可以整除 i 或 j
 *  即                  \Sigma_{d=1}^m \Sigma_{i=1}^(n/d) \Sigma_{j=1}^(m/d) [gcd(i,j)==1] (i*j*d)
 *  即                  \Sigma_{d=1}^m d \Sigma_{i=1}^(n/d) \Sigma_{j=1}^(m/d) [gcd(i,j)==1] (i*j)
 *
 *  设 f(n,m)=\Sigma_{i=1}^(n) \Sigma_{j=1}^(m) [gcd(i,j)==1] (i*j)，即原式后半部分
 *  根据 [x==1]=\Sigma_{d|x} \mu(d)
 *  有f(n,m)=           \Sigma_{i=1}^(n) \Sigma_{j=1}^(m) \Sigma_{d|gcd(i,j)} \mu(d) (i*j)
 *  把 d 拿前面
 *  f(n,m)=             \Sigma_{d=1}^m \Sigma_{i=1}^(n/d) \Sigma_{j=1}^(m/d) \mu(d) (i*d*j*d)
 *  f(n,m)=             \Sigma_{d=1}^m \Sigma_{i=1}^(n/d) \mu(d) (i*d*d)*(1+m/d)*(m/d)/2
 *  f(n,m)=             \Sigma_{d=1}^m \mu(d) (d*d)*(1+n/d)*(n/d)/2*(1+m/d)*(m/d)/2
 *
 *  原式=               \Sigma_{d=1}^m d f(n/d,m/d)
 *  内外层都可以数论分块
 */

static constexpr uint32_t N = 10000000;
using mint = OY::StaticModInt32<20101009, true>;
mint d2_mu_d_presum[N + 1];
struct Solver {
    mint solve(uint32_t n, uint32_t m) const {
        OY::MOBIUS::Table<N, false> Mu(m);
        for (uint32_t i = 1; i <= n; i++) d2_mu_d_presum[i] = d2_mu_d_presum[i - 1] + mint(i) * i * Mu.query_mobius(i);
        auto query_d2_mu_d = [&](uint32_t l, uint32_t r) {
            return d2_mu_d_presum[r] - d2_mu_d_presum[l - 1];
        };
        mint ans{};
        for (auto &&d_range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            mint inner_ans{};
            for (auto &&d2_range : OY::PairSqrtDecomposition<uint32_t>(d_range.quot1(), d_range.quot2())) {
                uint64_t d2_min = d2_range.left(), d2_max = d2_range.right();
                mint cur = query_d2_mu_d(d2_min, d2_max) * (1 + d2_range.quot1()) * d2_range.quot1() * (1 + d2_range.quot2()) * d2_range.quot2();
                inner_ans += cur;
            }
            uint64_t d_min = d_range.left(), d_max = d_range.right();
            ans += inner_ans * (d_min + d_max) * (d_max - d_min + 1) / 8;
        }
        return ans;
    }
};
void solve_mobius() {
    uint32_t n, m;
    cin >> n >> m;
    if (n < m) std::swap(n, m);
    cout << Solver().solve(n, m);
}

int main() {
    solve_mobius();
}
