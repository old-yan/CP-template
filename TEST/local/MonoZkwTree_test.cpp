#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

void test() {
    // MonoZkw 和 Zkw 在只有单点修改情况下是完全一样的，所以相同之处不再展示
    // 展示一个单点修改的求乘积树的写法
    int A[] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
#if CPP_STANDARD >= 202002L
    // 幺元为 1
    OY::MONOZKW::Tree<uint32_t, decltype([] { return 1; }), std::multiplies<uint32_t>> S(A, A + 9);
    cout << S << endl;
    S.modify(3, 100);
    cout << S << endl;
    cout << "prod = " << S.query_all() << endl;
#else
    struct One {
        uint32_t operator()() const { return 1; }
    };
    OY::MONOZKW::Tree<uint32_t, One, std::multiplies<uint32_t>> S(A, A + 9);
    cout << S << endl;
    S.modify(3, 100);
    cout << S << endl;
    cout << "prod = " << S.query_all() << endl;
#endif
}

int main() {
    test();
}
/*
#输出如下
[4, 9, 2, 3, 5, 7, 8, 1, 6]
[4, 9, 2, 100, 5, 7, 8, 1, 6]
prod = 12096000

*/