#include "DS/Accumulator.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

int main() {
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

    // 建立一个区间最小值累加器
    // 甚至可以适用 catl 的最值函数
    auto acc_min = OY::make_Accumulator<true, true>(A, A + 10, std::min);
    cout << "min(A[3~6])     =" << acc_min.query(3, 6) << endl;

    // 建立一个区间最大公约数累加器
    // 可以在参数框里写 lambda
    auto acc_gcd = OY::make_Accumulator<true, true>(A, A + 10, std::gcd);
    cout << "gcd(A[3~6])     =" << acc_gcd.query(3, 6) << endl;

    // 建立一个区间按位与累加器
    // 按位与的函数类具有默认构造，可以忽略构造参数
    auto acc_bit_and = OY::make_Accumulator<true, true>(A, A + 10, std::bit_and<int>());
    cout << "bit_and(A[3~6]) =" << acc_bit_and.query(3, 6) << endl;

    // 建立一个区间按位或累加器
    // 一开始可以是空的
    OY::ACC::Table<OY::ACC::CustomNode<int, std::bit_or<int>>, true, true> acc_bit_or;
    acc_bit_or.reset(A, A + 10);
    cout << "bit_or(A[3~6])  =" << acc_bit_or.query(3, 6) << endl;

    // 最普通的加法累加器
    auto acc_sum = OY::make_Accumulator<true, true>(A, A + 10, std::plus<int>());
    cout << acc_sum << endl;

    // 查找从下标 1 开始总和小于等于 14 的最远边界
    auto right = acc_sum.max_right(1, [](int s) { return s <= 14; });
    cout << "right = " << right << '\n';
}
/*
#输出如下
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
max(A[3~6])     =12
min(A[3~6])     =4
gcd(A[3~6])     =2
bit_and(A[3~6]) =0
bit_or(A[3~6])  =14
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
right = 2

*/