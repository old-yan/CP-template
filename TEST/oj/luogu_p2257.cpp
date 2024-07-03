#include "IO/FastIO.h"
#include "MATH/Mobius.h"
#include "MATH/PairSqrtDecomposition.h"

/*
[P2257 YY的GCD](https://www.luogu.com.cn/problem/P2257)
*/
/**
 *  不妨假设 n>=m
 *  求              \Sigma_{i=1}^n \Sigma_{j=1}^m [isp(gcd(i,j))]
 *  即              \Sigma_{i=1}^n \Sigma_{j=1}^m \Sigma_{k=1}^m [gcd(i,j)==k][isp(k)]
 *  即              \Sigma_{k=1}^\m [isp(k)] \Sigma_{i=1}^n \Sigma_{j=1}^m [gcd(i,j)==k]
 *  等价于          \Sigma_{k=1}^\m [isp(k)] \Sigma_{i=1}^(n/k) \Sigma_{j=1}^(m/k) [gcd(i,j)==1]
 *
 *  不妨设 f(n,m)=\Sigma_{i=1}^n \Sigma_{j=1}^m [gcd(i,j)==1]，即原式后半部分
 *  根据莫比乌斯函数的结论， [x==1]=\Sigma_{d|x} \mu(d)
 *  f(n,m)=         \Sigma_{i=1}^n \Sigma_{j=1}^m \Sigma_{d|gcd(i,j)} \mu(d)
 *  即              \Sigma_{i=1}^n \Sigma_{j=1}^m \Sigma_{d=1}^m \mu(d) [d|gcd(i,j)]
 *  此时，我们终于可以调换顺序
 *  即              \Sigma_{d=1}^m \mu(d) \Sigma_{i=1}^n \Sigma_{j=1}^m [d|gcd(i,j)]
 *  一看最后面，又能化简
 *  即              \Sigma_{d=1}^m \mu(d) \Sigma_{i=1}^(n/d) \Sigma_{j=1}^(m/d)
 *
 *  外层数论分块，内层也来个数论分块，两层数论分块跑很快
 *
 *  吗？错了，这样交上去之后超时了。因为是多测。
 *  原式=           \Sigma_{k=1}^\m [isp(k)] f(n/k,m/k)
 *  即              \Sigma_{k=1}^\m [isp(k)] \Sigma_{d=1}^m \mu(d) \Sigma_{i=1}^(n/k/d) \Sigma_{j=1}^(m/k/d)
 *  显然，k*d 可以看成一个整体 S=k*d
 *  即              \Sigma_{S=1}^m \Sigma_{d=1}^m \mu(d) \Sigma_{k=1}^m [isp(k)] (n/S) (m/S)
 *  如果试图枚举 S
 *  即              \Sigma_{S=1}^m (n/S) (m/S) \Sigma_{k|S} \mu(S/k) [isp(k)]
 *  后半部分 g(S)=\Sigma_{k|S} \mu(S/k) [isp(k)] 为与 n m 无关的式子，可以多测共享
 *  预处理出 g(S) 即可通过多测。另外注意前面的 S 需要数论分块枚举
 */

static constexpr uint32_t N = 10000000;
int gS_presum[N + 1];
struct Solver {
    void prepare(uint32_t maxm) {
        OY::MOBIUS::Table<N, false> Mu(maxm);
        for (uint32_t i = 1; i <= maxm; i++) {
            if (Mu.is_prime(i))
                for (uint32_t j = 1, k = i; k <= maxm; j++, k += i) gS_presum[k] += Mu.query_mobius(j);
            gS_presum[i] += gS_presum[i - 1];
        }
    }
    uint64_t solve(uint32_t n, uint32_t m) const {
        uint64_t ans{};
        for (auto &&S_range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            uint32_t l = S_range.left(), r = S_range.right();
            // n_S 表示 n/S, m_S 表示 m/S
            uint32_t n_S = S_range.quot1(), m_S = S_range.quot2();
            ans += uint64_t(n_S) * m_S * (gS_presum[r] - gS_presum[l - 1]);
        }
        return ans;
    }
};
void solve_mobius() {
    uint32_t t;
    cin >> t;
    std::vector<std::pair<uint32_t, uint32_t>> nms(t);
    uint32_t maxm = 0;
    for (auto &[n, m] : nms) {
        cin >> n >> m;
        if (n < m) std::swap(n, m);
        maxm = std::max(maxm, m);
    }

    Solver sol;
    sol.prepare(maxm);

    for (auto &&[n, m] : nms)
        cout << sol.solve(n, m) << endl;
}

int main() {
    solve_mobius();
}
