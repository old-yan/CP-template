#include "DS/LazyBitset.h"
#include "IO/FastIO.h"

void test() {
    OY::VectorLazyBitset<uint32_t> B(100000000);
    // 将区间置为 1
    B.set(10000000, 30000000);
    cout << "B[10000000~20000000]=" << B.count(10000000, 20000000) << endl;
    // 将区间置为 0
    B.reset(5000000, 14000000);
    cout << "B[10000000~20000000]=" << B.count(10000000, 20000000) << endl;
    // 将整个位集翻转
    B.flip();
    cout << "B[10000000~20000000]=" << B.count(10000000, 20000000) << endl;

    for (int i = 13950000; i < 14050000; i += 10000)
        cout << "index " << i << ":" << B.at(i) << endl;

    // 全部置为 0
    B.reset();

    B.set(160000);
    B.set(9800000);
    B.set(3570000);
    B.set(20000);
    for (int i = B.first_one(); i >= 0; i = B.next_one(i))
        cout << i << ' ';
    cout << endl;
    for (int i = B.last_one(); i >= 0; i = B.prev_one(i))
        cout << i << ' ';
    cout << endl;
}
void test_merge() {
    OY::VectorLazyBitset<uint32_t> S1(10), S2(10), S3(10);
    for (int i : {2, 3, 4, 6, 9}) S1.set(i);
    for (int i : {1, 2, 5, 6, 8}) S2.set(i);
    cout << "S1:" << S1 << endl;
    cout << "S2:" << S2 << endl;
    cout << "after S1&=S2:" << (S1 &= S2) << endl;

    for (int i : {0, 4, 6, 8}) S3.set(i);
    cout << "S3:" << S3 << endl;
    cout << "after S1&=S3:" << (S1 |= S3) << endl;
}

int main() {
    test();
    test_merge();
}
/*
#输出如下
B[10000000~20000000]=10000001
B[10000000~20000000]=6000000
B[10000000~20000000]=4000001
index 13950000:1
index 13960000:1
index 13970000:1
index 13980000:1
index 13990000:1
index 14000000:1
index 14010000:0
index 14020000:0
index 14030000:0
index 14040000:0
20000 160000 3570000 9800000 
9800000 3570000 160000 20000 
S1:[2, 3, 4, 6, 9]
S2:[1, 2, 5, 6, 8]
after S1&=S2:[2, 6]
S3:[0, 4, 6, 8]
after S1&=S3:[0, 2, 4, 6, 8]

*/