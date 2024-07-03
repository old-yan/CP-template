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

    auto read_double = [](auto...) -> double {
        int x;
        cin >> x;
        return x;
    };
    auto write_double = [](double _x) {
        if (_x < 0) cout << '-', _x = -_x;
        int x = std::round(_x * 100);
        cout << x / 100 << '.';
        if (x % 100 < 10) cout << '0';
        cout << x % 100;
    };
    OY::GaussJordanElimination<double, 100, 100> GE(n, n);
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) GE.coef(i, j) = read_double();
        GE.result(i) = read_double();
    }

    if (!GE.calc() || GE.has_multi_solution())
        cout << "No Solution";
    else
        for (uint32_t i = 0; i < n; i++) write_double(GE.get_solution(i)), cout << endl;
}
