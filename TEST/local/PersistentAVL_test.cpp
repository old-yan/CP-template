#include "DS/PersistentAVL.h"
#include "IO/FastIO.h"

void test_no_lock() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    // 用最简单的方法制造一颗可持久化可区间加的求和树
    // op,map,com 中只有 map 必须要手写
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto map = [](int x, int y, int size) { return y + x * size; };
    auto T = OY::make_lazy_PerAVL<int, int, false, false>(std::plus<int>(), map, std::plus<int>());
#else
    struct {
        int operator()(int x, int y, int size) const { return y + x * size; };
    } map;
    auto T = OY::make_lazy_PerAVL<int, int, false, false>(std::plus<int>(), map, std::plus<int>());
#endif
    for (int a : A) T.insert_by_key(a);
    cout << T << endl
         << endl;

    auto T2 = T.copy();
    T2.insert_by_key(250);

    auto T3 = T2.copy();
    T3.insert_by_key(150);

    auto T4 = T.copy();
    T4.root()->modify(1000);

    T.root()->modify(100000);

    cout << "T :" << T << endl;
    cout << "T2:" << T2 << endl;
    cout << "T3:" << T3 << endl;
    cout << "T4:" << T4 << endl;
}

void test_with_lock() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    // 用最简单的方法制造一颗可持久化求和树
    auto T = OY::make_PerAVL<int, std::less<int>, true>(std::plus<int>());
    // 初始状态下只有一个版本，就算是写操作也没必要解锁
    for (int a : A) T.insert_by_key(a);
    cout << T << endl
         << endl;

    auto T2 = T.copy();
    // 静态函数解锁
    decltype(T2)::unlock();
    T2.insert_by_key(250);
    decltype(T2)::lock();

    auto T3 = T2.copy();
    // 可以通过普通函数方式调用解锁
    T3.unlock();
    T3.insert_by_key(150);
    T3.unlock();

    auto T4 = T.copy();
    // 同类型共享锁
    T2.unlock();
    T4.erase_by_rank(0);
    T3.lock();

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
{100, 200, 300, 400, 500}

T :{100100, 100200, 100300, 100400, 100500}
T2:{100, 200, 250, 300, 400, 500}
T3:{100, 150, 200, 250, 300, 400, 500}
T4:{1100, 1200, 1300, 1400, 1500}
{100, 200, 300, 400, 500}

T :{100, 200, 300, 400, 500}
T2:{100, 200, 250, 300, 400, 500}
T3:{100, 150, 200, 250, 300, 400, 500}
T4:{200, 300, 400, 500}

*/