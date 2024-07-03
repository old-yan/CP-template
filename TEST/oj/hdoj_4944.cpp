#include "IO/FastIO.h"
#include "MATH/SqrtDecomposition.h"

/*
[FSF’s game](http://acm.hdu.edu.cn/showproblem.php?pid=4944)
*/
/**
 * 本题要求                 \Sigma_{i=1}^n \Sigma_{j=1}^i \Sigma_{k=1}^j (i*j/gcd(i/k,j/k)) [k|i][k|j]
 * 改为枚举 k
 * 即                       \Sigma_{k=1}^n \Sigma_{i=1}^(n/k) \Sigma_{j=1}^i (i*k*j*k/gcd(i,j))
 * 按套路枚举 gcd(i,j)
 * 即                       \Sigma_{k=1}^n k*k \Sigma_{d=1}^(n/k) \Sigma_{i=1}^(n/k) \Sigma_{j=1}^i (i*j/d) [gcd(i,j)==d]
 * 即                       \Sigma_{k=1}^n k*k \Sigma_{d=1}^(n/k) d \Sigma_{i=1}^(n/k/d) \Sigma_{j=1}^i (i*j) [gcd(i,j)==1]
 * 根据 [x==1]==\Sigma_{d|x} \Mu(d)
 * 原式等于                 \Sigma_{k=1}^n k*k \Sigma_{d=1}^(n/k) d \Sigma_{i=1}^(n/k/d) \Sigma_{j=1}^i (i*j) \Sigma_{e|gcd(i,j)} \Mu(e)
 * 枚举 e                   \Sigma_{k=1}^n k*k \Sigma_{d=1}^(n/k) d \Sigma_{e=1}^(n/k/d) \Mu(e) \Sigma_{i=1}^(n/k/d) \Sigma_{j=1}^i (i*j) [e|gcd(i,j)]
 * 即                       \Sigma_{k=1}^n k*k \Sigma_{d=1}^(n/k) d \Sigma_{e=1}^(n/k/d) e*e*\Mu(e) \Sigma_{i=1}^(n/k/d/e) \Sigma_{j=1}^i (i*j)
 * 设 f(x)=\Sigma_{i=1}^x \Sigma_{j=1}^i (i*j)
 * 原式等于                 \Sigma_{k=1}^n k*k \Sigma_{d=1}^(n/k) d \Sigma_{e=1}^(n/k/d) e*e*\Mu(e) f(n/k/d/e)
 * 三个层次都数论分块可解
 *
 * 吗？不行。本题是多测，还得优化
 * 把 d*e 设为一个整体 S=d*e
 * 即                       \Sigma_{k=1}^n k*k \Sigma_{S=1}^(n/k) f(n/k/S) \Sigma_{d|S} S*S/d*\Mu(S/d)
 * 设 g(x)=\Sigma_{d|x} x*x/d*\Mu(x/d) ，显然 g 与外部无关，所以 g 可以预处理
 * 原式等于                 \Sigma_{k=1}^n k*k \Sigma_{S=1}^(n/k) f(n/k/S) g(S)
 *
 * 再把 k*S 设为一个整体 T=k*S
 * 即                       \Sigma_{T=1}^n \Sigma_{k|T} k*k f(n/T) g(T/k)
 * 即                       \Sigma_{T=1}^n f(n/T) \Sigma_{k|T} k*k g(T/k)
 * 设 h(x)=\Sigma_{k|x} k*k g(x/k)，显然 h 与外部无关，所以 h 可以预处理
 * 原式等于                 \Sigma_{T=1}^n f(n/T) h(T)
 * 数论分块可以解决
 */

static constexpr uint32_t N = 500000;
void solve_mobius() {
    uint32_t t;
    cin >> t;
    std::vector<uint32_t> qs(t);
    uint32_t mx{};
    for (auto &n : qs) {
        cin >> n;
        mx = std::max(mx, n);
    }

    // f 函数很简单
    auto f = [](uint32_t x) -> uint64_t {
        // 注意细节，5e5 的四次方除以 24 会丢失精度，必须中间先除以 6，最后除以 4
        return uint64_t(x) * (x + 2) * (x + 1) / 6 * (x * 3 + 1) / 4;
    };
    // g 函数为积性函数
    // h 函数为积性函数
    // 所以可以用积性函数计算器处理 g h
    // 但是我们发现 h 的式子非常简单，其实 h(x)==x
    // 所以 g h 都不用预处理了
    for (uint32_t i = 0; i != t; i++) {
        uint32_t ans{};
        for (auto &&T_range : OY::SqrtDecomposition<uint32_t>(qs[i])) {
            uint32_t l = T_range.left(), r = T_range.right();
            ans += f(T_range.quot()) * (uint64_t(r - l + 1) * (r + l) / 2);
        }
        cout << "Case #" << i + 1 << ": " << ans << '\n';
    }
}

/**
 * 第二种思路：
 * 我们要求的是F(n)=            \Sigma_{i=1}^n \Sigma_{j=1}^i \Sigma_{k=1}^j (i*j/gcd(i/k,j/k)) [k|i][k|j]
 * 考虑F(n)-F(n-1)=             \Sigma_{j=1}^n \Sigma_{k=1}^j (n*j/gcd(n/k,j/k)) [k|n][k|j]
 * 即                           \Sigma_{j=1}^n \Sigma_{k=1}^j (n*j*k/gcd(n,j)) [k|n][k|j]
 * 设 d=gcd(n,j)/k
 * 即                           \Sigma_{j=1}^n \Sigma_{d|gcd(n,j)} (n*j/d)
 * 即                           \Sigma_{d|n} \Sigma_{j=1}^(n/d) (n*j)
 * 即                           \Sigma_{d|n} n*(1+n/d)*(n/d)/2
 */
void solve_mobius2() {
    uint32_t t;
    cin >> t;
    std::vector<uint32_t> qs(t);
    uint32_t mx{};
    for (auto &n : qs) {
        cin >> n;
        mx = std::max(mx, n);
    }

    std::vector<uint32_t> dp(mx + 1);
    for (uint32_t i = 1; i <= mx; i++)
        for (uint32_t j = i, k = 1; j <= mx; j += i, k++) dp[j] += j * (uint64_t(1 + k) * k / 2);
    for (uint32_t i = 1; i <= mx; i++) dp[i] += dp[i - 1];
    for (uint32_t i = 0; i != t; i++)
        cout << "Case #" << i + 1 << ": " << dp[qs[i]] << '\n';
}

int main() {
    // solve_mobius();
    solve_mobius2();
}
