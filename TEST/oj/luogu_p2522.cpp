#include "IO/FastIO.h"
#include "MATH/Mobius.h"
#include "MATH/PairSqrtDecomposition.h"

/*
[P2522 [HAOI2011] Problem b](https://www.luogu.com.cn/problem/P2522)
*/
/**
 *  首先认识到本题就是 P3455 的二维差分
 *  所以把 P3455 做了就会本题了
 */

static constexpr uint32_t N = 50000;
struct Solver {
    OY::MOBIUS::Table<N, true> Mu;
    void prepare(uint32_t maxm) {
        Mu.resize(maxm);
    }
    uint64_t solve(uint32_t n, uint32_t m, uint32_t k) const {
        uint64_t ans{};
        // 按 quot 递增遍历整除分块
        // 则 left right 递减
        for (auto &&x : OY::PairSqrtDecomposition<uint32_t>(n / k, m / k)) ans += uint64_t(Mu.query_mobius(x.left(), x.right())) * x.quot1() * x.quot2();
        return ans;
    }
};
void solve_mobius() {
    uint32_t t;
    cin >> t;
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t, uint32_t, uint32_t>> qs(t);
    uint32_t maxm{};
    for (auto &[a, b, c, d, k] : qs) {
        cin >> a >> b >> c >> d >> k;
        maxm = std::max(maxm, std::min(b, d));
    }

    Solver sol;
    sol.prepare(maxm);
    for (auto &&[a, b, c, d, k] : qs) {
        cout << sol.solve(a - 1, c - 1, k) + sol.solve(b, d, k) - sol.solve(a - 1, d, k) - sol.solve(b, c - 1, k) << endl;
    }
}

int main() {
    solve_mobius();
}
