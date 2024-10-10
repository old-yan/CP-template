#include "DS/RollbackAdjDiff.h"
#include "IO/FastIO.h"

int main() {
    using Vec = OY::RollbackSumTable<int>;

    Vec a;
    a.push_back(100);
    a.push_back(80);
    a.push_back(50);

    a.pop_back();
    a.push_back(90);

    cout << a << endl;

    a.push_back(1000);
    cout << a << endl;
}
/*
#输出如下
[100, 80, 90]
[100, 80, 90, 1000]

*/