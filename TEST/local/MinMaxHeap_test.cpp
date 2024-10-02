#include "DS/MinMaxHeap.h"
#include "IO/FastIO.h"

void test() {
    OY::MMHeap::Heap<int> S;
    S.push(100);
    S.push(300);
    S.push(50);
    S.push(600);
    S.push(250);
    cout << "min = " << S.top_min() << endl;
    S.pop_min();
    cout << "max = " << S.top_max() << endl;
    S.pop_max();
    cout << "min = " << S.top_min() << endl;
    S.pop_min();
    cout << "max = " << S.top_max() << endl;
    S.pop_max();
}

int main() {
    test();
}
/*
#输出如下
min = 50
max = 600
min = 100
max = 300

*/