#include "DS/SparseTable.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

void test() {
    // 模板内置了 Min Max Gcd Lcm BitAnd BitOr 八种特化
    // 这些运算可以拿来就用
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    OY::STMaxTable<int> st_max(A, A + 10);
    cout << st_max << endl;
    cout << "max(A[2~6]) = " << st_max.query(2, 6) << endl;

    OY::STBitAndTable<int> st_bit_and(A, A + 10);
    cout << "bit_and(A[2~6]) = " << st_bit_and.query(2, 6) << endl
         << endl;
}

void test_make() {
    // 通过 make 声明一颗区间逻辑与表
    int A[5] = {1, 1, 0, 0, 1};
    struct {
        int operator()(int x, int y) const { return x && y; }
    } op;
    auto st_and = OY::make_STTable(A, A + 5, op);
    cout << st_and << endl;
    cout << "and(A[1~2]) = " << st_and.query(1, 2) << endl;
    cout << "and(A[2~3]) = " << st_and.query(2, 3) << endl
         << endl;
}

void test_group() {
    // 通过半群的重写，实现各种意义上的取最值
    struct GetLongest {
        using value_type = std::string;
        static value_type op(const std::string &x, const std::string &y) {
            return x.size() > y.size() ? x : y;
        }
    };
    std::vector<std::string> s{"hello", "cat", "world", "dajiahao", "ok"};
    auto st_longest = OY::ST::Table<GetLongest>(5, [&](int i) { return s[i]; });
    cout << st_longest << endl;
    cout << "longest is " << st_longest.query_all() << endl;

    // 自带的二分要比自己手写二分略快
    // 查找从下标 1 开始字符串长度在 5 以内的最远边界
    auto right = st_longest.max_right(1, [](const std::string &s) { return s.size() <= 5; });
    cout << "right = " << right << '\n';
}

int main() {
    test();
    test_make();
    test_group();
}
/*
#输出如下
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
max(A[2~6]) = 12
bit_and(A[2~6]) = 0

[1, 1, 0, 0, 1]
and(A[1~2]) = 0
and(A[2~3]) = 0

[hello, cat, world, dajiahao, ok]
longest is dajiahao
right = 2

*/