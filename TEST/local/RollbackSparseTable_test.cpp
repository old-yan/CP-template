#include "DS/RollbackSparseTable.h"
#include "IO/FastIO.h"

void test() {
    // 仅展示与普通猫树不同的功能
    using Tree = OY::RollbackSTMaxTable<int>;
    Tree S;
    S.reserve(10);
    for (int i = 0; i < 10; i++) S.push_back(i * i);
    cout << S << endl;
    for (int i = 0; i < 5; i++) S.pop_back();
    for (int i = 5; i < 10; i++) S.push_back(i * i + 100);
    cout << S << endl;
}

int main() {
    test();
}
/*
#输出如下
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
[0, 1, 4, 9, 16, 125, 136, 149, 164, 181]

*/