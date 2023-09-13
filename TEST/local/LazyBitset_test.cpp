#include "DS/LazyBitset.h"
#include "IO/FastIO.h"

int main() {
    OY::LazyBitset::Tree<uint32_t, 10000> B(100000000);
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

*/