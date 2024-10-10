#include "DS/HistoryBIT.h"
#include "IO/FastIO.h"

void test() {
    OY::VectorHistoryBIT<int> S(3, [&](int i) {
        return (i + 1) * 100;
    });
    cout << S << endl;

    S.copy_version();
    S.add(0, 10);
    S.add(1, 10);
    cout << S << endl;

    S.copy_version();
    S.add(1, 70);
    S.add(2, 70);
    cout << S << endl;

    for (int i = 0; i < 3; i++)
        cout << "history of index " << i << " = " << S.history_query(i) << endl;
}

int main() {
    test();
}
/*
#输出如下
[100, 200, 300]
[110, 210, 300]
[110, 280, 370]
history of index 0 = 320
history of index 1 = 690
history of index 2 = 970

*/