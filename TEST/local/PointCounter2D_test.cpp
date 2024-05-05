#include "DS/PointCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::PC2D::Table<int, bool> S;
    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);
    S.prepare();

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;
}

int main() {
    test();
}
/*
#输出如下
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2

*/