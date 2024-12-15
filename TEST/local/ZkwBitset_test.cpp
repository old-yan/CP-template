#include "DS/ZkwBitset.h"
#include "IO/FastIO.h"

void test() {
    OY::ZkwTreeBitset<> B(10000);
    // 将区间置为 1
    B.set(1000, 3000);
    cout << "B[1000~2000]=" << B.count(1000, 2000) << endl;
    // 将区间置为 0
    B.reset(500, 1400);
    cout << "B[1000~2000]=" << B.count(1000, 2000) << endl;
    // 将整个位集翻转
    B.flip();
    cout << "B[1000~2000]=" << B.count(1000, 2000) << endl;

    for (int i = 1395; i < 1405; i++)
        cout << "index " << i << ":" << B.at(i) << endl;

    // 全部置为 0
    B.reset();

    B.set(16);
    B.set(980);
    B.set(357);
    B.set(2);
    for (int i = B.first_one(); i >= 0; i = B.next_one(i))
        cout << i << ' ';
    cout << endl;
    for (int i = B.last_one(); i >= 0; i = B.prev_one(i))
        cout << i << ' ';
    cout << endl;
}

int main() {
    test();
}
/*
#输出如下
B[1000~2000]=1001
B[1000~2000]=600
B[1000~2000]=401
index 1395:1
index 1396:1
index 1397:1
index 1398:1
index 1399:1
index 1400:1
index 1401:0
index 1402:0
index 1403:0
index 1404:0
2 16 357 980 
980 357 16 2 

*/