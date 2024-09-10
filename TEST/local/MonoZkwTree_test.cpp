#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

void test() {
    // 模板内置了 Min Max Gcd Lcm BitAnd BitOr BitXor Sum 八种特化
    // 这些运算可以拿来就用
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    OY::MonoMaxTree<int> s_max(A, A + 10);
    cout << s_max << endl;
    cout << "max(A[2~6]) = " << s_max.query(2, 6) << endl;

    OY::MonoSumTree<int> s_sum(A, A + 10);
    cout << s_sum << endl;
    cout << "sum(A[2~6]) = " << s_sum.query(2, 6) << endl
         << endl;
}

void test_monoid() {
    // 通过半群的重写，实现各种意义上的取最值
    struct GetLongest {
        using value_type = std::string;
        static value_type op(const std::string &x, const std::string &y) {
            return x.size() > y.size() ? x : y;
        }
        static std::string identity() { return ""; }
    };
    std::vector<std::string> s{"hello", "cat", "world", "dajiahao", "ok"};
    auto s_longest = OY::MONOZKW::Tree<GetLongest>(5, [&](int i) { return s[i]; });
    cout << s_longest << endl;
    cout << "longest is " << s_longest.query_all() << endl;

    // 自带的二分要比自己手写二分快
    // 查找从下标 1 开始字符串长度在 5 以内的最远边界
    auto right = s_longest.max_right(1, [](const std::string &s) { return s.size() <= 5; });
    cout << "right = " << right << '\n';
}

int main() {
    test();
    test_monoid();
}
/*
#输出如下
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
max(A[2~6]) = 12
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
sum(A[2~6]) = 39

[hello, cat, world, dajiahao, ok]
longest is dajiahao
right = 2

*/