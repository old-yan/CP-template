#include "IO/FastIO.h"
#include "MATH/Mobius.h"
#include "MATH/PairSqrtDecomposition.h"
#include "MATH/StaticModInt32.h"

/*
[#P2693. jzptab](https://new.bzoj.org:88/p/P2693)
*/
/**
 *  本题与 P1829 的区别是，本题为多测，所以线性做法肯定过不去
 *
 *  假定 n>=m
 *  求                  \Sigma_{i=1}^n \Sigma_{j=1}^m lcm(i,j)
 *  P1829 推到
 *  原式等于            \Sigma_{d=1}^m d \Sigma_{e=1}^(m/d) \mu(e) (e*e)*(1+n/d/e)*(n/d/e)/2*(1+m/d/e)*(m/d/e)/2
 *  按套路，把 d*e 看成一个整体 S=d*e
 *  即                  \Sigma_{d=1}^m d \Sigma_{e=1}^(m/d) \mu(e) (e*e)*(1+n/S)*(n/S)*(1+m/S)*(m/S)/4
 *  枚举 S              \Sigma_{S=1}^m \Sigma_{d|S} d \mu(S/d) (S/d)*(S/d)*(1+n/S)*(n/S)*(1+m/S)*(m/S)/4
 *  即                  \Sigma_{S=1}^m (1+n/S)*(n/S)*(1+m/S)*(m/S)/4 \Sigma_{d|S} d \mu(S/d) (S/d)*(S/d)
 *  设 f(S)=\Sigma_{d|S} d \mu(S/d) (S/d)*(S/d)
 *  显然，f(S) 与具体的 n m 无关
 *  所以可以先预处理 f 函数
 *  原式等于            \Sigma_{S=1}^m (1+n/S)*(n/S)*(1+m/S)*(m/S)/4*f(S)
 *  最后，数论分块枚举 S
 *
 *  #### 注意，预处理 f 函数的时候，一般会考虑基于调和级数去处理
 *  由于 f 为积性函数，所以也可以线性推
 */

static constexpr uint32_t N = 10000000;
using mint = OY::StaticModInt32<100000009, false>;
mint fs_presum[N + 1];
OY::MOBIUS::Multiplicative<N> mul;
struct Solver {
    void prepare(uint32_t maxm) {
        // f(S) 需要用到积性函数
        // 直接用积性函数计算器计算即可
        auto calc_prime = [](uint32_t p) {
            mint p2 = mint::raw(p);
            return p2 - p2 * p2;
        };
        auto calc_prime_pow = [](uint32_t p, uint32_t cnt, uint32_t low) { return fs_presum[low] * mint::raw(p); };
        mul.solve(maxm, fs_presum, calc_prime, calc_prime_pow);
        for (uint32_t i = 1; i <= maxm; i++) fs_presum[i] += fs_presum[i - 1];
    }
    mint solve(uint32_t n, uint32_t m) const {
        mint res{};
        for (auto &&S_range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            mint f = fs_presum[S_range.right()] - fs_presum[S_range.left() - 1];
            res += mint(1 + S_range.quot1()) * S_range.quot1() * (1 + S_range.quot2()) * S_range.quot2() * f;
        }
        return res / 4;
    }
};

void solve_mobius() {
    uint32_t t;
    cin >> t;
    std::vector<std::pair<uint32_t, uint32_t>> qs(t);
    uint32_t maxm = 0;
    for (auto &[n, m] : qs) {
        cin >> n >> m;
        if (n < m) std::swap(n, m);
        maxm = std::max(maxm, m);
    }

    Solver sol;
    sol.prepare(maxm);
    for (auto &[n, m] : qs)
        cout << sol.solve(n, m) << endl;
}

int main() {
    solve_mobius();
}
