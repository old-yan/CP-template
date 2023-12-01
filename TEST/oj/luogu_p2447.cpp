#include "IO/FastIO.h"
#include "MATH/GaussJordanElimination.h"

#include <vector>

/*
[P2447 [SDOI2010] 外星千足虫](https://www.luogu.com.cn/problem/P2447)
*/
/**
 * 本题为高斯消元法模板题
 */

int main() {
    uint32_t unknown, equation_total;
    cin >> unknown >> equation_total;
    // 因为要二分，多次进行消元。所以先把所有等式存一下
    std::vector<std::bitset<1001>> equations(equation_total);
    for (uint32_t i = 0; i < equation_total; i++) {
        for (uint32_t j = 0; j <= unknown; j++) {
            char c;
            cin >> c;
            if (c == '1') equations[i].set(j);
        }
    }

    // 二分找出最小的等式
    auto check = [&](uint32_t k) {
        OY::GaussJordanXorElimination<1000, 2000> GE(unknown, k);
        for (uint32_t i = 0; i < k; i++) GE.set_equation(i, equations[i]);
        GE.calc();
        if (GE.has_multi_solution()) return false;
        return true;
    };
    uint32_t low = unknown, high = equation_total + 1;
    while (low < high) {
        uint32_t mid = (low + high) / 2;
        if (check(mid))
            high = mid;
        else
            low = mid + 1;
    }
    if (low > equation_total) {
        cout << "Cannot Determine";
        return 0;
    }
    cout << low << endl;

    // 最后再跑一次高斯消元，输出答案
    OY::GaussJordanXorElimination<1000, 2000> GE(unknown, low);
    for (uint32_t i = 0; i < low; i++) GE.set_equation(i, equations[i]);
    GE.calc();
    for (uint32_t i = 0; i < unknown; i++)
        if (GE.get_solution(i))
            cout << "?y7M#\n";
        else
            cout << "Earth\n";
}
