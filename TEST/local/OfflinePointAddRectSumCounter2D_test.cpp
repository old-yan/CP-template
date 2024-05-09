#include "DS/OfflinePointAddRectSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::OFFLINEPARSC2D::Solver<int, bool> S;
    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);

    S.add_query(-1, 1, -1, 1);
    S.add_query(0, 1, -1, 1);

    auto res = S.solve();

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << res[0] << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << res[1] << endl;
}

int main() {
    test();
}
/*
#输出如下
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2

*/