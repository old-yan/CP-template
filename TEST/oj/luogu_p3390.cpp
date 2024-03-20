#include "IO/FastIO.h"
#include "MATH/DynamicMatrix.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P3390 【模板】矩阵快速幂](https://www.luogu.com.cn/problem/P3390)
*/

int main() {
    using mint = OY::mgint1000000007;
    // using mint = OY::mint1000000007;
    uint32_t n;
    uint64_t k;
    cin >> n >> k;
    auto mat = OY::DynamicMatrix<mint>::raw(n, n);
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) {
            cin >> mat[i][j];
        }
    }
    auto res = mat.pow(k);
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) {
            cout << res[i][j] << ' ';
        }
        cout << endl;
    }
}