#include "IO/FastIO.h"
#include "MATH/GaussJordanElimination.h"

/*
[P3389 【模板】高斯消元法](https://www.luogu.com.cn/problem/P3389)
*/
/**
 * 本题为高斯消元法模板题
 */

int main() {
    uint32_t n;
    cin >> n;

    OY::GaussJordanElimination<double, 100, 100> GE(n, n);
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) cin >> GE.coef(i, j);
        cin >> GE.result(i);
    }

    if (!GE.calc() || GE.has_multi_solution())
        cout << "No Solution";
    else {
        cout.precision(2);
        for (uint32_t i = 0; i < n; i++) cout << GE.get_solution(i) << '\n';
    }
}
