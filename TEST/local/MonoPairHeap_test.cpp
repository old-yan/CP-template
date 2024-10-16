#include "DS/MonoPairHeap.h"
#include "IO/FastIO.h"

void test() {
    // 一个小根堆
    // 值类型 int，和类型 int64
    OY::VectorMonoSumPairHeap<int, int64_t, 0, std::greater<int>> Q;
    for (int v = 1; v <= 10; v++) Q.push(v);
    Q.pop();
    Q.pop();
    cout << Q.query_all() << endl;
}

int main() {
    test();
}
/*
#输出如下
52

*/