#include "IO/FastIO.h"
#include "MATH/Mobius.h"
#include "MATH/PairSqrtDecomposition.h"
#include "MATH/SqrtDecomposition.h"

/*
[P3327 [SDOI2015] 约数个数和](https://www.luogu.com.cn/problem/P3327)
*/
/**
 *  假定 n>=m
 *  求                  \Sigma_{i=1}^n \Sigma_{j=1}^m \Sigma_{d=1}^(i*j) [d|(i*j)]
 *  根据                \Sigma_{d=1}^(i*j) [d|(i*j)] == \Sigma_{d1|i} \Sigma_{d2|j} [gcd(d1,d2)==1]
 *  原式变形为          \Sigma_{i=1}^n \Sigma_{d1|i} \Sigma_{j=1}^m \Sigma_{d2|j} [gcd(d1,d2)==1]
 *  把枚举 d1 d2 放前面 \Sigma_{d1=1}^n \Sigma_{d2=1}^m \Sigma_{i=1}^n \Sigma_{j=1}^m [d1|i][d2|j][gcd(d1,d2)==1]
 *  即                  \Sigma_{d1=1}^n \Sigma_{d2=1}^m (n/d1)(m/d2)[gcd(d1,d2)==1]
 *
 *  到这里，这个问题看起来不太能做
 *  不妨换个问题，求    f(x)=\Sigma_{d1=1}^n \Sigma_{d2=1}^m (n/d1)(m/d2)[gcd(d1,d2)==x]
 *
 *   f(x) 也不好求，求 f(x) 的形式二的莫比乌斯变换
 *                      g(x)=\Sigma_{x|d} f(d)
 *  则                  g(x)=\Sigma_{x|d} \Sigma_{d1=1}^n \Sigma_{d2=1}^m (n/d1)(m/d2)[gcd(d1,d2)==d]
 *  即                  g(x)=\Sigma_{d1=1}^n \Sigma_{d2=1}^m (n/d1)(m/d2)[x|gcd(d1,d2)]
 *  即                  g(x)=\Sigma_{d1=1}^n \Sigma_{d2=1}^m (n/d1)(m/d2)[x|d1][x|d2]
 *  即                  g(x)=\Sigma_{d1=1}^(n/x) (n/x/d1) \Sigma_{d2=1}^(m/x) (m/x/d2)
 *
 *  我们发现 g(x) 前后两部分是一样的形式
 *  设 h(x)=\Sigma_{d=1}^x (x/d)
 *  则 g(x)=h(n/x)*h(m/x),
 *  f 是 g 的反演，可以求出 f(x)=\Sigma_{x|d} \mu(d/x) g(d)=\Sigma_{x|d} \mu(d/x) h(n/d) h(m/d)
 *  要计算 f(1)=\Sigma_{d=1}^m \mu(d) h(n/d) h(m/d) 需要数论分块枚举 d，复杂度 O(\sqrt m)
 */

static constexpr uint32_t N = 50000;
uint32_t h[N + 1];
struct Solver {
    OY::MOBIUS::Table<N, true> Mu = {0};
    void prepare(uint32_t maxn, uint32_t maxm) {
        Mu.resize(maxm);
        auto solve_h = [&](uint32_t x) {
            uint32_t res{};
            for (auto &&i_range : OY::SqrtDecomposition<uint32_t>(x))
                res += (i_range.m_right - i_range.m_left + 1) * i_range.m_quot;
            h[x] = res;
        };
        for (uint32_t hi = 1; hi <= maxn; hi++) solve_h(hi);
    }
    uint64_t solve(uint32_t n, uint32_t m) const {
        uint64_t res{};
        for (auto &&d_range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            uint64_t g = uint64_t(h[d_range.quot1()]) * h[d_range.quot2()];
            res += g * Mu.query_mobius(d_range.left(), d_range.right());
        }
        return res;
    }
};

void solve_mobius() {
    uint32_t t;
    cin >> t;
    std::vector<std::pair<uint32_t, uint32_t>> qs(t);
    uint32_t maxn = 0, maxm = 0;
    for (auto &[n, m] : qs) {
        cin >> n >> m;
        if (n < m) std::swap(n, m);
        maxn = std::max(maxn, n);
        maxm = std::max(maxm, m);
    }

    Solver sol;
    sol.prepare(maxn, maxm);
    for (auto &[n, m] : qs)
        cout << sol.solve(n, m) << endl;
}

int main() {
    solve_mobius();
}
