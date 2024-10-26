#include "DS/ErasableMonoPairHeap.h"
#include "IO/FastIO.h"

void test() {
    OY::VectorErasableMonoSumPairHeap<int> S;
    S.push(100);
    S.push(400);
    S.push(200);
    S.push(300);
    S.push(500);
    S.erase(200);
    S.erase(400);
    cout << "size = " << S.size() << endl;
    S.pop();
    cout << "sum = " << S.query_all() << endl;
}

int main() {
    test();
}
/*
#输出如下
size = 3
sum = 400

*/