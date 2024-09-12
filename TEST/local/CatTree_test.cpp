#include "DS/CatTree.h"
#include "IO/FastIO.h"

void test() {
    // 模板内置了 Min Max Gcd Lcm BitAnd BitOr BitXor Sum 八种特化
    // 这些运算可以拿来就用
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    OY::CatMaxTable<int> cat_max(A, A + 10);
    cout << cat_max << endl;
    cout << "max(A[2~6]) = " << cat_max.query(2, 6) << endl;

    OY::CatSumTable<int> cat_sum(A, A + 10);
    cout << "sum(A[2~6]) = " << cat_sum.query(2, 6) << endl
         << endl;
}

void test_make() {
    // 通过 make 声明一颗区间乘积表
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    auto cat_prod = OY::make_CatTree(A, A + 10, std::multiplies<int>());
    cout << cat_prod << endl;
    cout << "prod(A[2~6]) = " << cat_prod.query(2, 6) << endl
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
    auto cat_longest = OY::CAT::Table<GetLongest>(5, [&](int i) { return s[i]; });
    cout << cat_longest << endl;
    cout << "longest is " << cat_longest.query_all() << endl;

    // 自带的二分要比自己手写二分略快
    // 查找从下标 1 开始字符串长度在 5 以内的最远边界
    auto right = cat_longest.max_right(1, [](const std::string &s) { return s.size() <= 5; });
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
sum(A[2~6]) = 39

[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
prod(A[2~6]) = 20736

[hello, cat, world, dajiahao, ok]
longest is dajiahao
right = 2

*/