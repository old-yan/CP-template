#include "DS/RectAddPointSumMaintainer2D.h"
#include "IO/FastIO.h"

void test() {
    OY::RAPSM2D::Table<int, int> S;

    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;

    S.add_rect(-1, 0, 0, 1, 100);
    S.add_rect(0, 1, -1, 0, 20);

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;
}

int main() {
    test();
}
/*
#输出如下
val of S{1, -1}: 1
val of S{0, 0}: 2
val of S{1, -1}: 21
val of S{0, 0}: 122

*/