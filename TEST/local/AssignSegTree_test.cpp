#include "DS/AssignSegTree.h"
#include "IO/FastIO.h"

void test() {
    const char *s[] = {"apple", "banana", "peal", "orange", "banana"};
    // 一颗不维护信息聚合的树
    OY::VectorAssignSeg<const char *, nullptr> S(5);
    for (int i = 0; i < 5; i++) S.modify(i, s[i]);
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
    // 对于数字类型的信息，而且还是求和信息
    // 因为通过乘法，可以很快算出翻倍的值，所以可以用如下特化
    OY::VectorAssignSumSeg<int> S(5);
    for (int i = 0; i < 5; i++) S.modify(i, arr[i]);
    cout << S << endl;
    S.modify(1, 3, 20);
    S.modify(2, 4, 5);
    cout << S << endl;
    cout << "sum(S[0~2]) = " << S.query(0, 2) << endl;
    cout << "sum(S[1~3]) = " << S.query(1, 3) << endl;
    cout << "sum(S[2~4]) = " << S.query(2, 4) << endl;
    cout << endl;
}

void test_min() {
    int arr[] = {1, 100, 1000, 10, 10000};
    // 一颗维护区间最小值的树
    OY::VectorAssignMinSeg<int> S(5);
    for (int i = 0; i < 5; i++) S.modify(i, arr[i]);
    cout << S << endl;
    S.modify(1, 3, 20);
    S.modify(2, 4, 5);
    cout << S << endl;
    cout << "min(S[0~2]) = " << S.query(0, 2) << endl;
    cout << "min(S[1~3]) = " << S.query(1, 3) << endl;
    cout << "min(S[2~4]) = " << S.query(2, 4) << endl;
    cout << endl;
}

void test_fast_pow() {
    int arr[] = {1, 2, 1, 1, 3, 1, 2};
    // 假设维护一个数字区间，维护区间乘积
#if CPP_STANDARD >= 202002L
    auto op = [](int x, int y) { return x * y; };
    auto pow = [](int x, int n) { return ::pow(x, n); };
#else
    struct {
        int operator()(int x, int y) const { return x * y; }
    } op;
    struct {
        int operator()(int x, int n) const { return ::pow(x, n); }
    } pow;
#endif
    auto S = OY::make_fast_pow_AssignSegTree<int, 1>(7u, op, pow);
    for (int i = 0; i < 7; i++) S.modify(i, arr[i]);
    cout << S << endl;
    S.modify(1, 3, 3);
    cout << S << endl;
    S.modify(2, 5, 4);
    cout << S << endl;
    cout << "prod(S2[0~2]) = " << S.query(0, 2) << endl;
    cout << "prod(S2[1~3]) = " << S.query(1, 3) << endl;
    cout << "prod(S2[2~4]) = " << S.query(2, 4) << endl;
    S.modify(2, 5, 2);
    cout << S << endl;
    cout << "prod_all = " << S.query_all() << endl;
    cout << endl;
}

void test_slow_pow() {
    int64_t arr[] = {12, 2, 1, 3, 2, 0, 10};
    // 假设维护一个长整数区间，维护区间乘积
    // 由于长整数的 pow 可能有精度问题，所以只能采用 Lazy 树
#if CPP_STANDARD >= 202002L
    auto op = [](int64_t x, int64_t y) { return x * y; };
#else
    struct {
        double operator()(int64_t x, int64_t y) const { return x * y; }
    } op;
#endif
    auto S = OY::make_lazy_AssignSegTree<int64_t, 1>(7u, op);
    for (int i = 0; i < 7; i++) S.modify(i, arr[i]);
    cout << S << endl;
    S.modify(1, 3, 3);
    cout << S << endl;
    S.modify(2, 5, 1);
    cout << S << endl;
    cout << "prod(S2[0~2]) = " << S.query(0, 2) << endl;
    cout << "prod(S2[1~3]) = " << S.query(1, 3) << endl;
    cout << "prod(S2[2~4]) = " << S.query(2, 4) << endl;
    S.modify(2, 5, 2);
    cout << S << endl;
    cout << "prod_all = " << S.query_all() << endl;
    cout << endl;
}

int main() {
    test();
    test_sum();
    test_min();
    test_fast_pow();
    test_slow_pow();
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

[1, 100, 1000, 10, 10000]
[1, 20, 5, 5, 5]
min(S[0~2]) = 1
min(S[1~3]) = 5
min(S[2~4]) = 5

[1, 2, 1, 1, 3, 1, 2]
[1, 3, 3, 3, 3, 1, 2]
[1, 3, 4, 4, 4, 4, 2]
prod(S2[0~2]) = 12
prod(S2[1~3]) = 48
prod(S2[2~4]) = 64
[1, 3, 2, 2, 2, 2, 2]
prod_all = 96

[12, 2, 1, 3, 2, 0, 10]
[12, 3, 3, 3, 2, 0, 10]
[12, 3, 1, 1, 1, 1, 10]
prod(S2[0~2]) = 36
prod(S2[1~3]) = 3
prod(S2[2~4]) = 1
[12, 3, 2, 2, 2, 2, 10]
prod_all = 5760

*/