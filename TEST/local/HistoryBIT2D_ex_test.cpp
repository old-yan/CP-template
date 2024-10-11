#include "DS/HistoryBIT2D_ex.h"
#include "IO/FastIO.h"

void test() {
    OY::HBIT2DEX::Tree<int> S(2, 3, [&](int r, int c) {
        return (r + 1) * 100 + (c + 1);
    });
    cout << S << endl;

    S.copy_version();
    S.add(0, 0, 0, 1, 10);
    cout << S << endl;

    S.copy_version();
    S.add(0, 1, 1, 2, 1000);
    cout << S << endl;

    cout << "history sum of each position:\n";
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 3; c++)
            cout << S.history_query(r, c) << ",\n"[c == 2];
}

int main() {
    test();
}
/*
#输出如下
[[101, 102, 103], [201, 202, 203]]
[[111, 112, 103], [201, 202, 203]]
[[111, 1112, 1103], [201, 1202, 1203]]
history sum of each position:
323,1326,1309
603,1606,1609

*/