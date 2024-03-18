#include "IO/FastIO.h"
#include "MATH/PollardRho.h"

/*
[P1075 [NOIP2012 普及组] 质因数分解](https://www.luogu.com.cn/problem/P1075)
*/
/**
 * 本题为 Pollard Rho 模板题
 */

int main() {
    uint64_t n, mx{};
    cin >> n;
    OY::PollardRho::enumerate_prime_factors(n, [&](auto p) {
        mx = std::max(mx, p);
    });
    cout << mx;
}