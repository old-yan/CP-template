#include "DS/AssignZkwTree.h"
#include "IO/FastIO.h"

void test() {
    std::string s[] = {"apple", "banana", "peal", "orange", "banana"};
    // 一颗不维护信息聚合的树
    auto S = OY::make_AssignZkwTree(s, s + 5);
    cout << S << endl;
    S.modify(1, 3, "app");
    cout << S << endl;
    S.modify(2, 4, "bank");
    cout << S << endl;
    cout << endl;
}

void test_sum() {
    int arr[] = {1, 100, 1000, 10, 10000};
    // 一颗维护信息聚合的树
    // 因为和可以很快算出翻倍的值，所以可以用如下特化
    auto S = OY::make_fast_square_AssignZkwTree<int>(5, [&](int i) { return arr[i]; });
    cout << S << endl;
    S.modify(1, 3, 20);
    S.modify(2, 4, 5);
    cout << S << endl;
    cout << "sum(S[0~2]) = " << S.query(0, 2) << endl;
    cout << "sum(S[1~3]) = " << S.query(1, 3) << endl;
    cout << "sum(S[2~4]) = " << S.query(2, 4) << endl;
    cout << endl;

    std::string s[] = {"apple", "banana", "peal", "orange", "banana"};
    // 如果不能很快算出翻倍的值，就用如下特化
    auto S2 = OY::make_lazy_AssignZkwTree<std::string>(5, [&](int i) { return s[i]; });
    cout << S2 << endl;
    S2.modify(1, 3, "app");
    S2.modify(2, 4, "bank");
    cout << S2 << endl;
    cout << "sum(S2[0~2]) = " << S2.query(0, 2) << endl;
    cout << "sum(S2[1~3]) = " << S2.query(1, 3) << endl;
    cout << "sum(S2[2~4]) = " << S2.query(2, 4) << endl;
    cout << endl;
}

void test_xor() {
    // 如何实现异或查询？
    struct xor_node {
        int m_val;
        xor_node() = default;
        xor_node(int val) : m_val{val} {}
        xor_node operator+(const xor_node &rhs) const {
            return m_val ^ rhs.m_val;
        }
        // 显然，一个数异或翻倍也很容易速算
        xor_node operator*(int n) const {
            return n % 2 ? *this : 0;
        }
        // 判断懒标记是否为空
        explicit operator bool() const { return m_val; }
    };
    int arr[] = {1, 100, 1000, 10, 10000};
    // 一颗维护信息聚合的树
    // 因为和可以很快算出翻倍的值，所以可以用如下特化
    auto S = OY::make_fast_square_AssignZkwTree<xor_node>(5, [&](int i) { return arr[i]; });
    auto print = [&]() {
        using node = decltype(S)::node;
        S.do_for_each([](node *p) {
            cout << p->get().m_val << ' ';
        });
        cout << endl;
    };
    print();
    S.modify(1, 3, 20);
    print();
    S.modify(2, 4, 5);
    print();
    cout << "xor_sum(S[0~2]) = " << S.query(0, 2).m_val << endl;
    cout << "xor_sum(S[1~3]) = " << S.query(1, 3).m_val << endl;
    cout << "xor_sum(S[2~4]) = " << S.query(2, 4).m_val << endl;
    cout << endl;
}

void test_mul() {
    // 如何实现乘积查询？
    struct mul_node {
        int64_t m_val;
        mul_node() : m_val(1) {}
        mul_node(int64_t val) : m_val{val} {}
        mul_node operator+(const mul_node &rhs) const {
            return m_val * rhs.m_val;
        }
        // 显然，一个数乘法翻倍需要快速幂，所以比较慢，我们就不实现乘号了
    };
    int64_t arr[] = {1, 100, 1000, 10, 10000};
    // 一颗维护信息聚合的树
    // 因为和可以很快算出翻倍的值，所以可以用如下特化
    auto S = OY::make_lazy_AssignZkwTree<mul_node>(
        5, [&](int i) { return arr[i]; });
    auto print = [&]() {
        using node = decltype(S)::node;
        S.do_for_each([](node *p) {
            cout << p->get().m_val << ' ';
        });
        cout << endl;
    };
    print();
    S.modify(1, 3, 20);
    print();
    S.modify(2, 4, 5);
    print();
    cout << "prod(S[0~2]) = " << S.query(0, 2).m_val << endl;
    cout << "prod(S[1~3]) = " << S.query(1, 3).m_val << endl;
    cout << "prod(S[2~4]) = " << S.query(2, 4).m_val << endl;
    cout << endl;
}

int main() {
    test();
    test_sum();
    test_xor();
    test_mul();
}
/*
#输出如下
[apple, banana, peal, orange, banana]
[apple, app, app, app, banana]
[apple, app, bank, bank, bank]

[1, 100, 1000, 10, 10000]
[1, 20, 5, 5, 5]
sum(S[0~2]) = 26
sum(S[1~3]) = 30
sum(S[2~4]) = 15

[apple, banana, peal, orange, banana]
[apple, app, bank, bank, bank]
sum(S2[0~2]) = appleappbank
sum(S2[1~3]) = appbankbank
sum(S2[2~4]) = bankbankbank

1 100 1000 10 10000
1 20 20 20 10000
1 20 5 5 5
xor_sum(S[0~2]) = 16
xor_sum(S[1~3]) = 20
xor_sum(S[2~4]) = 5

1 100 1000 10 10000
1 20 20 20 10000
1 20 5 5 5
prod(S[0~2]) = 100
prod(S[1~3]) = 500
prod(S[2~4]) = 125

*/