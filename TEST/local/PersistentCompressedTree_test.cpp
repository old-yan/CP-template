#include "DS/PersistentCompressedTree.h"
#include "IO/FastIO.h"

void test() {
    // PerCompressed 可以解决各种各样的可持久化问题
    OY::VectorPerCompressedMaxTree<int> S;
    S.modify(2, 100);
    cout << S << endl;

    auto S2 = S.copy();
    S2.modify(5, 60);
    S2.modify(1, 80);
    cout << S2 << endl;

    auto S3 = S2.copy();
    S3.modify(2, 90);
    cout << S3 << endl;
}

int main() {
    test();
}
/*
#输出如下
[2:100]
[1:80,2:100,5:60]
[1:80,2:90,5:60]

*/