#include "DS/MonoBIT.h"
#include "IO/FastIO.h"

void test() {
    // MonoBIT 和 BIT 在只有单点修改情况下是完全一样的，所以相同之处不再展示
    // 展示一个单点修改的求乘积树的写法
    int A[] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
#if CPP_STANDARD >= 202002L
    // 幺元为 1
    OY::MONOBIT::Tree<uint32_t, decltype([] { return 1; }), std::multiplies<uint32_t>> S(A, A + 9);
    cout << "prod(A[0~2]) = " << S.presum(2) << endl;
    cout << "prod(A[0~3]) = " << S.presum(3) << endl;
    S.add(3, 100);
    cout << "prod(A[0~2]) = " << S.presum(2) << endl;
    cout << "prod(A[0~3]) = " << S.presum(3) << endl;
#else
    struct One {
        uint32_t operator()() const { return 1; }
    };
    OY::MONOBIT::Tree<uint32_t, One, std::multiplies<uint32_t>> S(A, A + 9);
    cout << "prod(A[0~2]) = " << S.presum(2) << endl;
    cout << "prod(A[0~3]) = " << S.presum(3) << endl;
    S.add(3, 100);
    cout << "prod(A[0~2]) = " << S.presum(2) << endl;
    cout << "prod(A[0~3]) = " << S.presum(3) << endl;
#endif
}

int main() {
    test();
}
/*
#输出如下
prod(A[0~2]) = 72
prod(A[0~3]) = 216
prod(A[0~2]) = 72
prod(A[0~3]) = 21600

*/