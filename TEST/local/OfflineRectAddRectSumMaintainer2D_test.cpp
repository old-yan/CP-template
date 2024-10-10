#include "DS/GlobalHashBIT2D.h"
#include "DS/OfflineRectAddRectSumMaintainer2D.h"
#include "IO/FastIO.h"

void test() {
    OY::OFFLINERARSM2D::Solver<int, bool, int> S;
    S.add_query(-1, 1, -1, 1);
    S.add_query(-1, 0, 0, 1);
    S.add_query(0, 1, -1, 0);
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.add_query(-1, 1, -1, 1);
    S.add_query(-1, 0, 0, 1);
    S.add_query(0, 1, -1, 0);

    OY::GBIT2D::Tree<uint32_t, decltype(S)::node, false, false, 1 << 10> bit{};
    // 给 solve 传递一个辅助用的 bit
    auto res = S.solve(bit);
    cout << "before change:\n";
    cout << "val of S{-1~1, -1~1}: " << res[0] << endl;
    cout << "val of S{-1~0, 0~1}: " << res[1] << endl;
    cout << "val of S{0~1, -1~0}: " << res[2] << endl;

    cout << "after change:\n";
    cout << "val of S{-1~1, -1~1}: " << res[3] << endl;
    cout << "val of S{-1~0, 0~1}: " << res[4] << endl;
    cout << "val of S{0~1, -1~0}: " << res[5] << endl;
}

int main() {
    test();
}
/*
#输出如下
before change:
val of S{-1~1, -1~1}: 0
val of S{-1~0, 0~1}: 0
val of S{0~1, -1~0}: 0
after change:
val of S{-1~1, -1~1}: 8
val of S{-1~0, 0~1}: 5
val of S{0~1, -1~0}: 5

*/