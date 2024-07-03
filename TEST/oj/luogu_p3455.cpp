#include "IO/FastIO.h"
#include "MATH/Mobius.h"
#include "MATH/PairSqrtDecomposition.h"

/*
[P3455 [POI2007] ZAP-Queries](https://www.luogu.com.cn/problem/P3455)
*/
/**
 *  不妨设 n>=m
 *  求              \Sigma_{i=1}^n \Sigma_{j=1}^m [gcd(i,j)==k]
 *  等价于          \Sigma_{i=1}^(n/k) \Sigma_{j=1}^(m/k) [gcd(i,j)==1]
 *  不妨令          a=n/k,b=m/k
 *  则为            \Sigma_{i=1}^a \Sigma_{j=1}^b [gcd(i,j)==1]
 *  根据莫比乌斯函数的结论， [x==1]=\Sigma_{d|x)} {\mu(d)}
 *  原式等于        \Sigma_{i=1}^a \Sigma_{j=1}^b \Sigma_{d|gcd(i,j)} {\mu(d)}
 *  即              \Sigma_{i=1}^a \Sigma_{j=1}^b \Sigma_{d=1}^b {\mu(d)}[d|gcd(i,j)]
 *  此时，我们终于可以调换顺序
 *  即              \Sigma_{i=1}^a \Sigma_{j=1}^b \Sigma_{d=1}^b {\mu(d)}[d|gcd(i,j)]
 *  即              \Sigma_{d=1}^b {\mu(d)} \Sigma_{i=1}^a \Sigma_{j=1}^b [d|gcd(i,j)]
 *  前半段易得一个前缀和
 *  后半段要想非零，那么 i 和 j 都得是 d 的倍数
 *  整个问题转化为一个枚举 d 的问题
 *  即              \Sigma_{d=1}^b {\mu(d)} (a/d)*(b/d)
 *  这样复杂度不够优，所以还得整除分块，按 a/d 和 b/d 的商来以批量枚举 d
 *  复杂度 O(T * sqrt(m/k))
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
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> qs(t);
    uint32_t maxm{};
    for (auto &[n, m, k] : qs) {
        cin >> n >> m >> k;
        if (n < m) std::swap(n, m);
        maxm = std::max(maxm, m);
    }

    Solver sol;
    sol.prepare(maxm);
    for (auto &&[n, m, k] : qs) cout << sol.solve(n, m, k) << endl;
}

int main() {
    solve_mobius();
}