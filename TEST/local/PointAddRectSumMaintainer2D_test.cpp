#include "DS/PointAddRectSumMaintainer2D.h"
#include "IO/FastIO.h"

void test_add() {
    OY::PARSM2D::Table<int, int, int> S;
    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;

    S.add_point(1, 1, 100);

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;
}

int main() {
    test_add();
}
/*
#输出如下
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2
sum of S{-1<=x<=1, -1<=y<=1}: 104
sum of S{0<=x<=1, -1<=y<=1}: 102

*/