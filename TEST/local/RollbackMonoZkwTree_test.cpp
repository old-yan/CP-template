#include "DS/RollbackMonoZkwTree.h"
#include "IO/FastIO.h"

void test() {
    // 仅展示与普通 MonoZkw 线段树不同的功能
    using Tree = OY::RollbackMonoSumTree<int64_t>;
    Tree S;
    S.reserve(10);
    for (int i = 0; i < 10; i++) S.push_back(i * i);
    cout << S << endl;
    cout << "sum = " << S.query_all() << endl;
    for (int i = 0; i < 5; i++) S.pop_back();
    for (int i = 5; i < 10; i++) S.push_back(i * i + 100);
    cout << S << endl;
    cout << "sum = " << S.query_all() << endl;
}

int main() {
    test();
}
/*
#输出如下
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
sum = 285
[0, 1, 4, 9, 16, 125, 136, 149, 164, 181]
sum = 785

*/