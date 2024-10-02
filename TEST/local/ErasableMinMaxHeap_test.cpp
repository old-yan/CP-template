#include "DS/ErasableMinMaxHeap.h"
#include "IO/FastIO.h"

void test() {
    OY::EMMHeap::Heap<int> S;
    S.push(100);
    S.push(400);
    S.push(200);
    S.push(300);
    S.push(500);
    S.erase(200);
    S.erase(400);
    cout << "size = " << S.size() << endl;
    S.pop_min();
    cout << "min = " << S.top_min() << endl;
}

int main() {
    test();
}
/*
#输出如下
size = 3
min = 300

*/