#include "IO/FastIO.h"
#include "MATH/Eratosthenes.h"
#include "MATH/Mobius.h"
#include "MATH/SqrtDecomposition.h"

/*
[P2158 [SDOI2008] 仪仗队](https://www.luogu.com.cn/problem/P2158)
*/
/**
 *  求                  \Sigma_{i=1}^n \Sigma_{i=1}^n [gcd(i,j)==1]
 *  根据 [x==1]=\Sigma_{d|x} \mu(d)
 *  即                  \Sigma_{i=1}^n \Sigma_{j=1}^n \Sigma_{d|gcd(i,j)} \mu(d)
 *  即                  \Sigma_{d=1}^n \Sigma_{i=1}^(n/d) \Sigma_{j=1}^(n/d) \mu(d)
 *  即                  \Sigma_{d=1}^n \mu(d) (n/d)^2
 *  数论分块，可以在根号 n 复杂度内求解（除了莫比乌斯筛的时间）
 *
 *  另外，互质的概念也可以从欧拉函数求解。打出前缀和表之后可以 O(1) 求解
 */

static constexpr uint32_t N = 40000;
void solve_phi() {
    uint32_t n;
    cin >> n;
    if (n == 1) {
        cout << 0 << endl;
        return;
    }
    OY::Eratosthenes::Sieve<N, true, false, false> es(n - 1);
    uint64_t ans{};
    for (uint32_t i = 2; i <= n - 1; i++) ans += es.get_Euler_Phi(i);
    cout << ans * 2 + 3;
}

struct Solver {
    uint64_t solve(uint32_t n) const {
        OY::MOBIUS::Table<N, true> Mu(n);
        uint64_t ans{};
        for (auto &&d_range : OY::SqrtDecomposition<uint32_t>(n)) {
            // 算出 f 函数的底数乘积
            uint32_t d_min = d_range.left(), d_max = d_range.right();
            ans += uint64_t(Mu.query_mobius(d_min, d_max)) * d_range.quot() * d_range.quot();
        }
        return ans;
    }
};
void solve_mobius() {
    uint32_t n;
    cin >> n;
    if (n == 1) {
        cout << 0 << endl;
        return;
    }
    cout << Solver().solve(n - 1) + 2;
}

int main() {
    solve_phi();
    // solve_mobius();
}
