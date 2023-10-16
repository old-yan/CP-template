#include "DS/SqrtTree.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

int main() {
    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};

    // 建立一个区间最大值根树
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    auto sqrt_max = OY::make_SqrtTree<1000>(A, A + 10, mymax);
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } mymax;
    auto sqrt_max = OY::make_SqrtTree<1000>(A, A + 10, mymax);
#endif
    cout << sqrt_max << endl;
    cout << "max(A[3~6])     =" << sqrt_max.query(3, 6) << endl;

    // 建立一个区间最小值根树
    // 甚至可以适用 stl 的最值函数
    auto sqrt_min = OY::make_SqrtTree<1000>(A, A + 10, std::min);
    cout << "min(A[3~6])     =" << sqrt_min.query(3, 6) << endl;

    // 建立一个区间最大公约数根树
    // 可以在参数框里写 lambda
    auto sqrt_gcd = OY::make_SqrtTree<1000>(A, A + 10, std::gcd);
    cout << "gcd(A[3~6])     =" << sqrt_gcd.query(3, 6) << endl;

    // 建立一个区间按位与根树
    // 按位与的函数类具有默认构造，可以忽略构造参数
    auto sqrt_bit_and = OY::make_SqrtTree<1000>(A, A + 10, std::bit_and<int>());
    cout << "bit_and(A[3~6]) =" << sqrt_bit_and.query(3, 6) << endl;

    // 建立一个区间按位或根树
    // 一开始可以是空的
    auto sqrt_bit_or = OY::make_SqrtTree<int, 1000>(0, std::bit_or<int>());
    sqrt_bit_or.reset(A, A + 10);
    cout << "bit_or(A[3~6])  =" << sqrt_bit_or.query(3, 6) << endl;

    // 便利化措施：由于实际使用的时候，往往是最值较多，所以最大值最小值有特化
    auto sqrt_default = OY::SqrtMaxTable<int, 1000>();
    sqrt_default.reset(A, A + 10);
    cout << "max(A[0~9])     =" << sqrt_default.query(0, 9) << endl;

    auto sqrt_default2 = OY::SqrtMinTable<int, 1000>();
    sqrt_default2.reset(A, A + 10);
    cout << "min(A[0~9])     =" << sqrt_default2.query(0, 9) << endl;

    // 通过比较函数的重载，实现各种意义上的取最值
    struct Cmp {
        bool operator()(const std::string &x, const std::string &y) const {
            return x.size() < y.size();
        }
    };
    std::vector<std::string> ss{"hello", "cat", "world", "dajiahao", "ok"};
    auto sqrt_longest = OY::Sqrt::Table<OY::Sqrt::BaseNode<std::string, Cmp>, 1 << 10>(5);
    for (int i = 0; i < 5; i++) {
        sqrt_longest.modify(i, ss[i]);
    }
    cout << sqrt_longest << endl;
    cout << "longest is " << sqrt_longest.query_all() << endl;

    // 自带的二分要比自己手写二分略快
    // 查找从下标 1 开始字符串长度在 5 以内的最远边界
    auto right = sqrt_longest.max_right(1, [](const std::string &s) { return s.size() <= 5; });
    cout << "right = " << right << '\n';

    // 做个性能测试，来个很大的表，全部单点更新一遍，看会不会超时
    // 显然没超时，所以根树单点修改也蛮快，注意 add 要比 modify 更快
    auto sqrt_sum = OY::make_SqrtTree<uint64_t, 1500000>(500000, std::plus<uint64_t>(), [](uint32_t i) { return i; });
    cout << sqrt_sum.query_all() << '\n';

    for (uint32_t i = 0; i < sqrt_sum.m_size; i++) sqrt_sum.add(i, 1);
    cout << sqrt_sum.query_all() << '\n';

    for (uint32_t i = 0; i < sqrt_sum.m_size; i++) sqrt_sum.modify(i, sqrt_sum.query(i) + 1);
    cout << sqrt_sum.query_all() << '\n';
}
/*
#输出如下
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
max(A[3~6])     =12
min(A[3~6])     =4
gcd(A[3~6])     =2
bit_and(A[3~6]) =0
bit_or(A[3~6])  =14
max(A[0~9])     =15
min(A[0~9])     =4
[hello, cat, world, dajiahao, ok]
longest is dajiahao
right = 2
124999750000
125000250000
125000750000

*/