### 一、模板类别

​	数据结构：区间推平 `zkw` 线段树。

​	练习题目：


1. [Range Set Range Composite](https://judge.yosupo.jp/problem/range_set_range_composite)(https://github.com/yosupo06/library-checker-problems/issues/829)



### 二、模板功能

​		本数据结构的功能与实现与 `ZkwTree` 几乎一致，唯一的特化就是本模板专门针对区间推平做了特化优化，所以与 `ZkwTree` 相同部分的内容不再介绍。

​		本数据结构对于结点的要求，与 `ZkwTree` 的要求有所不同。本模板不再要求结点内定义 `modify_type` ；把结点的修改类型和信息聚合类型视为一种类型 `value_type` 。

​		如果不要求进行区间信息查询，只需要对结点实现 `get` ， `set` 方法即可。

​		如果要求进行区间信息查询，除对结点实现 `get` ， `set` 方法外，只需令模板参数 `Info` 为 `true` 即可。显然，本模板不再要求传递信息聚合函数 `op` ，这是因为本模板默认用加号实现信息聚合。如想实现其他运算，请通过封装结构体实现（见文档示例）。

​		对于 `Info` 为 `true` 的情况，也有两种实现：第一种是当信息翻倍较慢的情况下，第二种是信息翻倍较快的情况下。第一种情况的例子是，如果元素类型是自取模整数，信息聚合是乘法运算。那么当对某个区间推平为某个值 `x` 时，需要 $O(\log n)$ 的时间计算出这个区间的信息聚合值。在这种情况下，每个结点会维护一个打表，分别表示一个 `x` 、两个 `x` 、四个 `x` 、八个 `x` ……的信息聚合值，最终的单次修改和查询的时间复杂度只有一层对数。第二种情况的例子是，如果元素类型是自取模整数，信息聚合是加法运算。那么当对某个区间推平为某个值 `x` 时，只需要 $O(1)$ 的时间计算出这个区间的信息聚合值。这种情况下，不需要维护打表，最终的单次修改和查询的时间复杂度也只有一层对数。

### 三、模板示例

```c++
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
    auto S = OY::make_fast_square_AssignZkwTree<int, 32, false>(
        5, [&](int i) { return arr[i]; }, 0);
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
    auto S = OY::make_fast_square_AssignZkwTree<xor_node, 32, false>(
        5, [&](int i) { return arr[i]; }, 0);
    S.do_for_each([](auto p) {
        cout << p->get().m_val << ' ';
    });
    cout << endl;
    S.modify(1, 3, 20);
    S.do_for_each([](auto p) {
        cout << p->get().m_val << ' ';
    });
    cout << endl;
    S.modify(2, 4, 5);
    S.do_for_each([](auto p) {
        cout << p->get().m_val << ' ';
    });
    cout << endl;
    cout << "xor_sum(S[0~2]) = " << S.query(0, 2).m_val << endl;
    cout << "xor_sum(S[1~3]) = " << S.query(1, 3).m_val << endl;
    cout << "xor_sum(S[2~4]) = " << S.query(2, 4).m_val << endl;
    cout << endl;
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
    auto S = OY::make_lazy_AssignZkwTree<mul_node, 32>(
        5, [&](int i) { return arr[i]; });
    S.do_for_each([](auto p) {
        cout << p->get().m_val << ' ';
    });
    cout << endl;
    S.modify(1, 3, 20);
    S.do_for_each([](auto p) {
        cout << p->get().m_val << ' ';
    });
    cout << endl;
    S.modify(2, 4, 5);
    S.do_for_each([](auto p) {
        cout << p->get().m_val << ' ';
    });
    cout << endl;
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
```

```
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

```

