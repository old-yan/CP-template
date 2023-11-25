#include "IO/FastIO.h"
#include "MATH/StaticMatrix.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P1306 斐波那契公约数](https://www.luogu.com.cn/problem/P1306)
*/
/**
 * 本题为矩阵快速幂模板题
 */

int main() {
    using mint = OY::StaticModInt32<100000000, false>;
    // using mint = OY::StaticMontgomeryModInt32<100000000, false>;
    using mat12 = OY::StaticMatrix<mint, 1, 2>;
    using mat22 = OY::StaticMatrix<mint, 2, 2>;

    uint32_t n, m;
    cin >> n >> m;
    mat12 a{{{0, 1}}};
    mat22 b{{{0, 1}, {1, 1}}};
    uint32_t gcd = std::gcd(n, m);
    auto f_gcd = a * b.pow(gcd);
    cout << f_gcd[0][0];
}