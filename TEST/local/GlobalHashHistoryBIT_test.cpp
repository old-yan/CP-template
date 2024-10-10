#include "DS/GlobalHashHistoryBIT.h"
#include "IO/FastIO.h"

OY::GHBIT::Tree<uint32_t, int, true, false, 1000> S(3);
void test() {
    auto print = [&] {
        cout << "S: ";
        for (int i = 0; i < S.size(); i++) cout << S.query(i) << ' ';
        cout << endl;
    };
    S.copy_version();
    S.add(0, 10);
    S.add(1, 10);
    print();

    S.copy_version();
    S.add(1, 2, 70);
    print();

    for (int i = 0; i < 3; i++)
        cout << "history of index " << i << " = " << S.history_query(i) << endl;
}

int main() {
    test();
}
/*
#输出如下
S: 10 10 0 
S: 10 80 70 
history of index 0 = 20
history of index 1 = 90
history of index 2 = 70

*/