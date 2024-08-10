#include "DS/RectUnion.h"
#include "IO/FastIO.h"

int main() {
    OY::RU::Solver<int> sol;
    sol.add_rect(-2, 0, -2, 0);
    sol.add_rect(0, 2, 0, 2);
    // 两个 3*3 的矩形，重叠了一个角
    cout << sol.solve();
}
/*
#输出如下
17

*/