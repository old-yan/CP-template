#include "IO/FastIO.h"
#include "MATH/GaussJordanElimination.h"

/*
[P2455 [SDOI2006] 线性方程组](https://www.luogu.com.cn/problem/P2455)
*/
/**
 * 本题为高斯消元法模板题
 */

int main() {
    uint32_t n;
    cin >> n;

    OY::GaussJordanElimination<double, 50, 50> GE(n, n);
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) cin >> GE.coef(i, j);
        cin >> GE.result(i);
    }

    if (!GE.calc())
        cout << "-1";
    else if (GE.has_multi_solution())
        cout << "0";
    else {
        cout.precision(2);
        for (uint32_t i = 0; i < n; i++) {
            cout << 'x' << i + 1 << '=' << GE.get_solution(i) << '\n';
        }
    }
}
