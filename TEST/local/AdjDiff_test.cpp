#include "DS/AdjDiff.h"
#include "IO/FastIO.h"

int main() {
    OY::AdjDiff::Table<int, true> ad(5);

    cout << ad << endl;

    ad.switch_to_value();
    ad.add(1, 10);
    ad.add(2, 100);
    cout << ad << endl;

    ad.switch_to_difference();
    ad.add(2, 4, 1000);

    ad.switch_to_value();
    cout << ad << endl;
}
/*
#输出如下
[0, 0, 0, 0, 0]
[0, 10, 100, 0, 0]
[0, 10, 1100, 1000, 1000]
*/