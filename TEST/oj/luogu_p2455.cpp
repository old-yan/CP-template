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
    OY::GJE::GaussJordanElimination<double, 50, 50> GE(n, n);
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) GE.coef(i, j) = read_double();
        GE.result(i) = read_double();
    }

    if (!GE.calc())
        cout << "-1";
    else if (GE.has_multi_solution())
        cout << "0";
    else {
        for (uint32_t i = 0; i < n; i++) {
            cout << 'x' << i + 1 << '=';
            write_double(GE.get_solution(i));
            cout << endl;
        }
    }
}
