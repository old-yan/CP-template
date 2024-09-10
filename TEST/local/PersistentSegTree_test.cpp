#include "DS/PersistentSegTree.h"
#include "IO/FastIO.h"

void test_no_lock() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    // 写一个默认的支持区间加的求和树
    OY::PerSeg::Tree<OY::PerSeg::LazyNode<int, int, uint32_t>, OY::PerSeg::Ignore, false, false, uint32_t> T(A, A + 5);
    cout << T << endl
         << endl;

    auto T2 = T.copy();
    T2.add(1, 1000);

    auto T3 = T2.copy();
    T3.add(2, -1000);

    auto T4 = T.copy();
    T4.add(0, 3, 10000);

    T.add(0, 3, 100000);
    cout << "T :" << T << endl;
    cout << "T2:" << T2 << endl;
    cout << "T3:" << T3 << endl;
    cout << "T4:" << T4 << endl;
}

void test_with_lock() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    // 写一个默认的求和树
    OY::PerSeg::Tree<OY::PerSeg::BaseNode<int>, OY::PerSeg::Ignore, true, true, uint32_t> T(A, A + 5);
    cout << T << endl
         << endl;

    auto T2 = T.copy();
    // 静态函数解锁
    decltype(T2)::unlock();
    T2.add(1, 1000);
    decltype(T2)::lock();

    auto T3 = T2.copy();
    // 可以通过普通函数方式调用解锁
    T3.unlock();
    T3.add(2, -1000);
    T3.lock();

    auto T4 = T.copy();
    // 同类型共享锁
    T2.unlock();
    T.add(0, 10000);
    T2.lock();

    cout << "T :" << T << endl;
    cout << "T2:" << T2 << endl;
    cout << "T3:" << T3 << endl;
    cout << "T4:" << T4 << endl;
}

int main() {
    test_no_lock();
    test_with_lock();
}
/*
#输出如下
[100, 200, 300, 400, 500]

T :[100100, 100200, 100300, 100400, 500]
T2:[100, 1200, 300, 400, 500]
T3:[100, 1200, -700, 400, 500]
T4:[10100, 10200, 10300, 10400, 500]
[100, 200, 300, 400, 500]

T :[10100, 200, 300, 400, 500]
T2:[100, 1200, 300, 400, 500]
T3:[100, 1200, -700, 400, 500]
T4:[100, 200, 300, 400, 500]
*/