#include "DS/PointAddRectSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::PARSC2D::Table<int, bool> S;
    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);
    S.prepare();

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;
}

void test_add() {
    OY::PARSC2D::Table<int, uint32_t, uint32_t, true> S;
    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);
    S.prepare();

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;

    // 编号为 3 的点的权值增加一百
    S.add_point_value(3, 100);

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;
}

int main() {
    test();
    test_add();
}
/*
#输出如下
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2
sum of S{-1<=x<=1, -1<=y<=1}: 104
sum of S{0<=x<=1, -1<=y<=1}: 102

*/