#include "DS/OfflineRectAddRectSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::OFFLINERARSC2D::Solver<int, bool, int> S;
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.add_query(-1, 1, -1, 1);
    S.add_query(-1, 0, 0, 1);
    S.add_query(0, 1, -1, 0);

    auto res = S.solve();
    cout << "val of S{-1~1, -1~1}: " << res[0] << endl;
    cout << "val of S{-1~0, 0~1}: " << res[1] << endl;
    cout << "val of S{0~1, -1~0}: " << res[2] << endl;
}

int main() {
    test();
}
/*
#输出如下
val of S{-1~1, -1~1}: 8
val of S{-1~0, 0~1}: 5
val of S{0~1, -1~0}: 5

*/