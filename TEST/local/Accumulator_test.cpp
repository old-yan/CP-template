#include "DS/Accumulator.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

void test() {
    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};

    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    // 建立一个区间最大值累加器
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    auto acc_max = OY::make_Accumulator<true, true>(A, A + 10, mymax);
    cout << acc_max << endl;
    cout << "max(A[3~6])     =" << acc_max.query(3, 6) << endl;
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } mymax;
    auto acc_max = OY::make_Accumulator<true, true>(A, A + 10, mymax);
    cout << acc_max << endl;
    cout << "max(A[3~6])     =" << acc_max.query(3, 6) << endl;
#endif

    // 建立一个区间最大公约数累加器
    struct Gcd {
        int operator()(int x, int y) const { return std::gcd(x, y); }
    };
    auto acc_gcd = OY::make_Accumulator<true, true>(A, A + 10, Gcd());
    cout << "gcd(A[3~6])     =" << acc_gcd.query(3, 6) << endl;

    // 建立一个区间按位与累加器
    // 按位与的函数类具有默认构造，可以忽略构造参数
    auto acc_bit_and = OY::make_Accumulator<true, true>(A, A + 10, std::bit_and<int>());
    cout << "bit_and(A[3~6]) =" << acc_bit_and.query(3, 6) << endl;

    // 最普通的加法累加器
    auto acc_sum = OY::make_Accumulator<true, true>(A, A + 10, std::plus<int>());
    cout << acc_sum << endl;

    // 查找从下标 1 开始总和小于等于 14 的最远边界
    auto right = acc_sum.max_right(1, [](int s) { return s <= 14; });
    cout << "right = " << right << '\n';
}

void test_semigroup() {
    // 怎样设计一个维护区间乘积的半群
    struct SemiGroup {
        using value_type = int;
        static value_type op(value_type x, value_type y) { return x * y; }
    };
    OY::PreSufTable<SemiGroup> S(10, [&](int i) { return i + 1; });
    cout << S << endl;
    cout << "prod(S[2~4]) = " << S.query(2, 4) << endl;
}

int main() {
    test();
    test_semigroup();
}
/*
#输出如下
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
max(A[3~6])     =12
gcd(A[3~6])     =2
bit_and(A[3~6]) =0
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
right = 2
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
prod(S[2~4]) = 60

*/