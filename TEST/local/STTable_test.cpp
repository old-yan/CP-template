#include <vector>

#include "DS/STTable.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

int main() {
    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};

    // 建立一个区间最大值 ST 表
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    auto st_max = OY::make_STTable<1000>(A, A + 10, mymax);
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } mymax;
    auto st_max = OY::make_STTable<1000>(A, A + 10, mymax);
#endif
    cout << st_max << endl;
    cout << "max(A[3~6])     =" << st_max.query(3, 6) << endl;

    // 建立一个区间最小值 ST 表
    // 甚至可以适用 stl 的最值函数
    auto st_min = OY::make_STTable<1000>(A, A + 10, std::min);
    cout << "min(A[3~6])     =" << st_min.query(3, 6) << endl;

    // 建立一个区间最大公约数 ST 表
    // 可以在参数框里写 lambda
    auto st_gcd = OY::make_STTable<1000>(A, A + 10, std::gcd);
    cout << "gcd(A[3~6])     =" << st_gcd.query(3, 6) << endl;

    // 建立一个区间按位与 ST 表
    // 按位与的函数类具有默认构造，可以忽略构造参数
    auto st_bit_and = OY::make_STTable<1000>(A, A + 10, std::bit_and<int>());
    cout << "bit_and(A[3~6]) =" << st_bit_and.query(3, 6) << endl;

    // 建立一个区间按位或 ST 表
    // 一开始可以是空的
    auto st_bit_or = OY::make_STTable<int, 1000>(0, std::bit_or<int>());
    st_bit_or.reset(A, A + 10);
    cout << "bit_or(A[3~6])  =" << st_bit_or.query(3, 6) << endl;

    // 便利化措施：由于实际使用的时候，往往是最值较多，所以最大值最小值有特化
    auto st_default = OY::STMaxTable<int, 1000>();
    st_default.reset(A, A + 10);
    cout << "max(A[0~9])     =" << st_default.query(0, 9) << endl;

    auto st_default2 = OY::STMinTable<int, 1000>();
    st_default2.reset(A, A + 10);
    cout << "min(A[0~9])     =" << st_default2.query(0, 9) << endl;

    // 通过比较函数的重载，实现各种意义上的取最值
    struct Cmp {
        bool operator()(const std::string &x, const std::string &y) const {
            return x.size() < y.size();
        }
    };
    std::vector<std::string> ss{"hello", "cat", "world", "dajiahao", "ok"};
    auto st_longest = OY::ST::Table<OY::ST::BaseNode<std::string, Cmp>, 1 << 10>(5);
    for (int i = 0; i < 5; i++) {
        st_longest.modify(i, ss[i]);
    }
    cout << st_longest << endl;
    cout << "longest is " << st_longest.query_all() << endl;

    // 自带的二分要比自己手写二分略快
    // 查找从下标 1 开始字符串长度在 5 以内的最远边界
    auto right = st_longest.max_right(1, [](const std::string &s) { return s.size() <= 5; });
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
max(A[0~9])     =15
min(A[0~9])     =4
[hello, cat, world, dajiahao, ok]
longest is dajiahao
right = 2
*/