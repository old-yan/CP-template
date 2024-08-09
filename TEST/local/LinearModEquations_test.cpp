#include "IO/FastIO.h"
#include "MATH/LinearModEquations.h"

int main() {
    // 假设求解以下方程组
    // 3 * x = 7 mod 10
    // 6 * x = 5 mod 13
    OY::LinearModEquations lme;
    lme.add_equation(3, 7, 10);
    lme.add_equation(6, 5, 13);

    auto res = lme.query();
    // 最小答案
    auto min = res.m_rem;
    // 答案的周期
    auto cycle = res.m_div;
    // min, min+cycle, min+cycle*2 ... 均为可能的答案
    cout << "first result = " << min << endl;
    cout << "second result = " << min + cycle << endl;
    cout << "third result = " << min + cycle * 2 << endl;
}
/*
#输出如下
first result = 29
second result = 159
third result = 289

*/