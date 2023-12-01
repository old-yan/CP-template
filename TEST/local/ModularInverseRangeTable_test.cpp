#include "IO/FastIO.h"
#include "MATH/ModularInverseRangeTable.h"
#include "MATH/StaticModInt32.h"

int main() {
    using mint = OY::mint1000000007;
    OY::ModularInverseRangeTable<mint, 1000> table(100);
    for (int i = 1; i <= 10; i++) {
        auto inv = table[i];
        cout << i << " * " << inv << " = " << inv * i << endl;
    }
}
/*
#输出如下
1 * 1 = 1
2 * 500000004 = 1
3 * 333333336 = 1
4 * 250000002 = 1
5 * 400000003 = 1
6 * 166666668 = 1
7 * 142857144 = 1
8 * 125000001 = 1
9 * 111111112 = 1
10 * 700000005 = 1

*/