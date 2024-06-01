#include "DS/StaticBitset.h"
#include "IO/FastIO.h"

int main() {
    OY::StaticBitset<100000> B;
    // 将区间置为 1
    B.set(10000, 30000);
    cout << "B[10000~20000]=" << B.count(10000, 20000) << endl;
    // 将区间置为 0
    B.reset(5000, 14000);
    cout << "B[10000~20000]=" << B.count(10000, 20000) << endl;
    // 将整个位集翻转
    B.flip();
    cout << "B[10000~20000]=" << B.count(10000, 20000) << endl;

    for (int i = 13950; i < 14050; i += 10)
        cout << "index " << i << ":" << B.at(i) << endl;

    // 全部置为 0
    B.reset();

    B.set(160);
    B.set(9800);
    B.set(3570);
    B.set(20);
    for (int i = B.first_one(); i >= 0; i = B.next_one(i))
        cout << i << ' ';
    cout << endl;

    // 进行一个背包左移
    B.bitor_lshift(1);
    for (int i = B.last_one(); i >= 0; i = B.prev_one(i))
        cout << i << ' ';
    cout << endl;
}
/*
#输出如下
B[10000~20000]=10001
B[10000~20000]=6000
B[10000~20000]=4001
index 13950:1
index 13960:1
index 13970:1
index 13980:1
index 13990:1
index 14000:1
index 14010:0
index 14020:0
index 14030:0
index 14040:0
20 160 3570 9800 
9801 9800 3571 3570 161 160 21 20 

*/