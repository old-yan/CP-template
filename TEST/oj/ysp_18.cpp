#include "IO/FastIO.h"
#include "MATH/GaussJordanElimination.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[System of Linear Equations](https://judge.yosupo.jp/problem/system_of_linear_equations)(https://github.com/yosupo06/library-checker-problems/issues/18)
*/
/**
 * 本题为方程组消元模板题
 */

using mint = OY::mgint998244353;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::GJE::GaussJordanElimination<mint, 500, 500> GS(m, n);
    for (uint32_t i = 0; i != n; i++)
        for (uint32_t j = 0; j != m; j++) cin >> GS.coef(i, j);
    for (uint32_t i = 0; i != n; i++) cin >> GS.result(i);
    if (!GS.calc())
        cout << "-1\n";
    else {
        cout << GS.rank() << endl;
        for (uint32_t i = 0; i != m; i++) cout << GS.get_solution(i) << " \n"[i == m - 1];
        auto call = [&](auto &&vec) {
            for (uint32_t i = 0; i != m; i++) cout << vec[i] << " \n"[i == m - 1];
        };
        GS.enumerate_base(call);
    }
}