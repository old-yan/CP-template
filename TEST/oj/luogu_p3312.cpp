#include "DS/WTree.h"
#include "IO/FastIO.h"
#include "MATH/Mobius.h"
#include "MATH/PairSqrtDecomposition.h"

/*
[P3312 [SDOI2014] 数表](https://www.luogu.com.cn/problem/P3312)
*/
/**
 *  假定 n>=m
 *  【方便起见，我们了解一下符号 \sigma(x) 表示 x 的所有因数之和】
 *  设f(n,m)=           \Sigma_{i=1}^n \Sigma_{j=1}^m \sigma(gcd(i,j))
 *  即                  \Sigma_{i=1}^n \Sigma_{j=1}^m \Sigma_{x|gcd(i,j)} x
 *  按套路枚举 gcd
 *  即                  \Sigma_{d=1}^m \Sigma_{i=1}^n \Sigma_{j=1}^m \Sigma_{x|d} [gcd(i,j)==d] x
 *  即                  \Sigma_{d=1}^m \Sigma_{x|d} d \Sigma_{i=1}^(n/d) \Sigma_{j=1}^(m/d) [gcd(i,j)==1]
 *
 *  设g(n,m)=           \Sigma_{i=1}^n \Sigma_{j=1}^m [gcd(i,j)==1]
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
 *  原式等于            \Sigma_{d=1}^m \Sigma_{x|d} x \Sigma_{e=1}^(m/d) \mu(e) (n/d/e)(m/d/e)
 *  再按套路把 d*e 看成一个整体 S=d*e
 *  枚举 S              \Sigma_{S=1}^m (n/S)(m/S) \Sigma_{d|S} \Sigma_{x|d} x \mu(S/d)
 *  即                  \Sigma_{S=1}^m (n/S)(m/S) \Sigma_{d|S} \sigma(d) \mu(S/d)
 *
 *  设 h(S)=\Sigma_{d|S} \sigma(d) \mu(S/d)
 *  我们发现 h(S) 与 n m 无关，所以可以预处理
 *
 *  题目所求的也不是 f(n,m) ，而是 f(n,m) 在 \sigma(d)<=a 部分的贡献
 *  考虑离线所有询问，有序添加 \sigma(d) 对 h(S) 的贡献，有序回答查询
 */

static constexpr uint32_t N = 100000;
struct node {
    int mobius;
    uint32_t sigma_val;
    node() = default;
    node(int) : mobius(1), sigma_val(1) {}
    node(int m, uint32_t s) : mobius(m), sigma_val(s) {}
    node operator*(const node &rhs) const { return {mobius * rhs.mobius, sigma_val * rhs.sigma_val}; }
};
node mu_sigma[N + 1];
void solve_mobius() {
    uint32_t t;
    cin >> t;
    struct Query {
        uint32_t n, m, a, index;
    };
    std::vector<Query> qs(t);
    uint32_t mx{};
    for (uint32_t i = 0; i != t; i++) {
        auto &[n, m, a, index] = qs[i];
        cin >> n >> m >> a;
        index = i;
        mx = std::max({mx, n, m});
    }
    std::sort(qs.begin(), qs.end(), [](auto &&q1, auto &&q2) { return q1.a <= q2.a; });

    // 注意到莫比乌斯函数具有积性，sigma 函数也具有积性
    // 考虑在积性函数模板里同时计算出莫比乌斯函数和 sigma 函数
    auto calc_prime = [&](uint32_t p) { return node{-1, p + 1}; };
    auto calc_prime_pow = [](uint32_t p, uint32_t cnt, uint32_t low) { return node{0, mu_sigma[low].sigma_val + low * p}; };
    OY::MOBIUS::Multiplicative<N>{}.solve(mx, mu_sigma, calc_prime, calc_prime_pow);

    std::vector<std::pair<uint32_t, uint32_t>> ss(mx);
    for (uint32_t i = 1; i <= mx; i++) ss[i - 1] = {mu_sigma[i].sigma_val, i};
    std::sort(ss.begin(), ss.end(), [](auto &&q1, auto &&q2) { return q1.first < q2.first; });
    uint32_t cursor = 0;

    OY::WSumTree<uint32_t> hs(mx + 1);
    std::vector<uint32_t> res(t);
    for (auto &[n, m, a, index] : qs) {
        while (cursor < mx && ss[cursor].first <= a) {
            auto &&[val, d] = ss[cursor++];
            for (uint32_t S = d, i = 1; S <= mx; S += d, i++) hs.add(S, val * mu_sigma[i].mobius);
        }
        uint32_t ans{};
        for (auto &&S_range : OY::PairSqrtDecomposition<uint32_t>(n, m))
            ans += S_range.quot1() * S_range.quot2() * hs.query(S_range.left(), S_range.right());
        res[index] = ans & 0x7fffffff;
    }
    for (auto e : res) cout << e << endl;
}

int main() {
    solve_mobius();
}
