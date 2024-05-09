#include "DS/RectAddPointSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::RAPSC2D::Table<int, bool> S;
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.prepare();

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;
}

void test_add() {
    OY::RAPSC2D::Table<int, uint32_t, uint32_t, true> S;
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.prepare();

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;

    // 编号为 1 的矩形的权值增加一百
    S.add_rect_value(1, 100);

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;
}

int main() {
    test();
    test_add();
}
/*
#输出如下
val of S{1, -1}: 1
val of S{0, 0}: 2
val of S{1, -1}: 1
val of S{0, 0}: 2
val of S{1, -1}: 101
val of S{0, 0}: 102

*/