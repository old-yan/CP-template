#include "IO/FastIO.h"
#include "MATH/StaticMatrix.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P1962 斐波那契数列](https://www.luogu.com.cn/problem/P1962)
*/
/**
 * 本题为矩阵快速幂模板题
 */

int main() {
    using mint = OY::mint1000000007;
    // using mint = OY::mgint1000000007;
    using mat12 = OY::StaticMatrix<mint, 1, 2>;
    using mat22 = OY::StaticMatrix<mint, 2, 2>;

    uint64_t n;
    cin >> n;
    mat12 a{{{1, 1}}};
    mat22 b{{{0, 1}, {1, 1}}};
    auto f_n = a * b.pow(n - 1);
    cout << f_n[0][0];
}