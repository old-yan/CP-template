#include "IO/FastIO.h"
#include "MATH/StaticMatrix.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P1939 矩阵加速（数列）](https://www.luogu.com.cn/problem/P1939)
*/
/**
 * 本题为矩阵快速幂模板题
 */

int main() {
    using mint = OY::mint1000000007;
    // using mint = OY::mgint1000000007;
    using mat13 = OY::StaticMatrix<mint, 1, 3>;
    using mat33 = OY::StaticMatrix<mint, 3, 3>;

    mat13 a{{{1, 1, 1}}};
    mat33 b{{{0, 0, 1}, {1, 0, 0}, {0, 1, 1}}};

    uint32_t t;
    cin >> t;
    for (uint32_t i = 0; i < t; i++) {
        uint32_t n;
        cin >> n;
        auto f_n = a * b.pow(n - 1);
        cout << f_n[0][0] << endl;
    }
}