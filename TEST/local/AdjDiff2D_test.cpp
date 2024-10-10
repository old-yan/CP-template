#include "DS/AdjDiff2D.h"
#include "IO/FastIO.h"

int main() {
    // 当 AutoSwitch 为 true 时，其实可以不用手动转换状态
    OY::AdjSumTable2D<int, true> ad(4, 5);

    cout << ad << endl;

    // ad.switch_to_value();
    ad.add(1, 2, 10);
    ad.add(2, 4, 100);
    cout << ad << endl;

    // ad.switch_to_difference();
    ad.add(1, 3, 0, 2, 1000);

    // ad.switch_to_value();
    cout << ad << endl;
}
/*
#输出如下
[[0, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0]]
[[0, 0, 0, 0, 0], [0, 0, 10, 0, 0], [0, 0, 0, 0, 100], [0, 0, 0, 0, 0]]
[[0, 0, 0, 0, 0], [1000, 1000, 1010, 0, 0], [1000, 1000, 1000, 0, 100], [1000, 1000, 1000, 0, 0]]

*/