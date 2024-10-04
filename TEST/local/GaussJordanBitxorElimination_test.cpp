#include "IO/FastIO.h"
#include "MATH/GaussJordanBitxorElimination.h"
#include "MATH/StaticModInt32.h"

int main() {
    // 异或高斯消元
    OY::GJBE::GaussJordanBitxorElimination<10, 10> GE(3, 1);
    // 0 x + 0 y + 0 z = 1
    GE.set_equation(0, std::bitset<11>{"1000"});
    // 计算
    if (!GE.calc()) {
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

    // 异或高斯消元
    OY::GJBE::GaussJordanBitxorElimination<10, 10> GE2(3, 1);
    // 1 x + 1 y + 1 z = 1
    GE2.set_equation(0, std::bitset<11>{"1111"});
    // 计算
    if (!GE2.calc()) {
        cout << "No Solution\n";
    } else if (GE2.has_multi_solution()) {
        cout << "Multi Solution. Possible solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << GE2.get_solution(i) << endl;
        cout << "Rank of solution: " << GE2.rank() << endl;
        // 一组基 x + y = 0，说明可以给解的 x 和 y 同时做修改，修改后仍为一组解
        // 一组基 x + z = 0，说明可以给解的 x 和 z 同时做修改，修改后仍为一组解
        GE2.enumerate_base([](decltype(GE2)::bitset_type x) {
            cout << x.to_string().substr(7) << endl;
        });
    } else {
        cout << "Unique Solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << GE2.get_solution(i) << endl;
    }
}
/*
#输出如下
No Solution
Multi Solution. Possible solution:
x0 = 1
x1 = 0
x2 = 0
Rank of solution: 2
0011
0101

*/