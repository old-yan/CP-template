#include "DS/PotentializedDSU.h"
#include "IO/FastIO.h"

int main() {
    OY::AddPDSUTable<int, true> u(4);

    // 假设结点 i 处的势能函数为 f(i)
    // f(1) - f(0) = 100
    u.unite_by_size(0, 1, 100);

    // f(3) - f(2) = 40
    u.unite_by_size(2, 3, 40);

    // f(1) - f(2) = 10
    u.unite_by_size(2, 1, 10);

    // 此时，四个结点间的相互距离已经被确定
    cout << "dis from 0~2 = " << u.calc(0, 2).second << endl;
    cout << "dis from 2~1 = " << u.calc(2, 1).second << endl;
    cout << "dis from 1~3 = " << u.calc(1, 3).second << endl;
}
/*
#输出如下
dis from 0~2 = 90
dis from 2~1 = 10
dis from 1~3 = 30

*/