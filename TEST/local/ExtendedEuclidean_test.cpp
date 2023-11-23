#include "IO/FastIO.h"
#include "MATH/ExtendedEuclidean.h"

int main() {
    int a = 100;
    int b = -85;
    // 只找出最大公约数
    auto res = OY::ExtenedEuclideanSolver::solve(a, b);
    cout << "gcd of " << a << " and " << b << " is: " << res.m_gcd << endl;
    cout << a << " * " << res.m_coef1 << " + " << b << " * " << res.m_coef2 << " = " << res.m_gcd << endl;

    // 尝试配平 -20
    int c = -20;
    auto res2 = OY::ExtenedEuclideanSolver::solve(a, b, c);
    if (res2.m_flag) {
        auto coef1 = res2.m_coef1;
        auto coef2 = (c - coef1 * a) / b;
        cout << "gcd of " << a << " and " << b << " is: " << res2.m_gcd << endl;
        cout << a << " * " << coef1 << " + " << b << " * " << coef2 << " = " << c << endl;
    } else {
        cout << "failed\n";
    }
}
/*
#输出如下
gcd of 100 and -85 is: 5
100 * 6 + -85 * 7 = 5
gcd of 100 and -85 is: 5
100 * 10 + -85 * 12 = -20

*/