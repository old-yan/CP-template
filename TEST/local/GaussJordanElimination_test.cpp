#include "IO/FastIO.h"
#include "MATH/GaussJordanElimination.h"
#include "MATH/StaticModInt32.h"

int main() {
    // double 版本高斯消元，三个未知数，三个方程
    OY::GJE::GaussJordanElimination<double, 10, 10> GE(3, 3);
    // 1 x + 3 y + 4 z = 5
    GE.set_equation(0, {1, 3, 4, 5});
    // 1 x + 4 y + 7 z = 3
    GE.set_equation(1, {1, 4, 7, 3});
    // 9 x + 3 y + 2 z = 2
    GE.set_equation(2, {9, 3, 2, 2});
    // 计算
    if (!GE.calc({}, [](double x) { return std::abs(x) < 1e-9; })) {
        cout << "No Solution\n";
    } else if (GE.has_multi_solution()) {
        cout << "Multi Solution. Possible solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << GE.get_solution(i) << endl;
    } else {
        cout << "Unique Solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << GE.get_solution(i) << endl;
    }

    // 也可以适用于自取模数类
    // mint 版本高斯消元，两个未知数，四个方程
    using mint = OY::mint998244353;
    OY::GJE::GaussJordanElimination<mint, 10, 10> GE2(2, 3);
    // 1 x + 3 y = 5
    GE2.set_equation(0, {1, 3, 5});
    // 2 x + 6 y = 10
    GE2.set_equation(1, {2, 6, 10});
    // 0 x + 0 y = 0
    GE2.set_equation(2, {0, 0, 0});
    // 计算
    if (!GE2.calc()) {
        cout << "No Solution\n";
    } else if (GE2.has_multi_solution()) {
        cout << "Multi Solution. Possible solution:\n";
        for (int i = 0; i < 2; i++)
            cout << "x" << i << " = " << GE2.get_solution(i) << endl;
    } else {
        cout << "Unique Solution:\n";
        for (int i = 0; i < 2; i++)
            cout << "x" << i << " = " << GE2.get_solution(i) << endl;
    }
}
/*
#输出如下
Unique Solution:
x0 = -0.973684
x1 = 5.184211
x2 = -2.394737
Multi Solution. Possible solution:
x0 = 5
x1 = 0

*/