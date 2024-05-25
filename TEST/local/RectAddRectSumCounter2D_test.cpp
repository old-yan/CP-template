#include "DS/RectAddRectSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::RARSC2D::Table<int, bool, int> S;
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.prepare();

    cout << "val of S{-1~1, -1~1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "val of S{-1~0, 0~1}: " << S.query(-1, 0, 0, 1) << endl;
    cout << "val of S{0~1, -1~0}: " << S.query(0, 1, -1, 0) << endl;
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