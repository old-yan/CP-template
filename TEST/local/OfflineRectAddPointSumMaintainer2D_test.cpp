#include "DS/OfflineRectAddPointSumMaintainer2D.h"
#include "IO/FastIO.h"

void test() {
    OY::OFFLINERAPSM2D::Solver<int, int> S;

    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);

    S.add_query(1, -1);
    S.add_query(0, 0);

    S.add_rect(-1, 0, 0, 1, 100);
    S.add_rect(0, 1, -1, 0, 20);

    S.add_query(1, -1);
    S.add_query(0, 0);

    auto res = S.solve();
    cout << "before add rects:\n";
    cout << "val of S{1, -1}: " << res[0] << endl;
    cout << "val of S{0, 0}: " << res[1] << endl;
    
    cout << "after add rects:\n";
    cout << "val of S{1, -1}: " << res[2] << endl;
    cout << "val of S{0, 0}: " << res[3] << endl;
}

int main() {
    test();
}
/*
#输出如下
before add rects:
val of S{1, -1}: 0
val of S{0, 0}: 1
after add rects:
val of S{1, -1}: 0
val of S{0, 0}: 101

*/
