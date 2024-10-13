### 一、模板类别

​	数据结构： `MonoSplay` 树。

​	练习题目：

1. [3321. 计算子数组的 x-sum II](https://leetcode.cn/problems/find-x-sum-of-all-k-long-subarrays-ii)
2. [P1503 鬼子进村](https://www.luogu.com.cn/problem/P1503)
3. [P1886 滑动窗口 /【模板】单调队列](https://www.luogu.com.cn/problem/P1886)
4. [P3369 【模板】普通平衡树](https://www.luogu.com.cn/problem/P3369)
5. [P3391 【模板】文艺平衡树](https://www.luogu.com.cn/problem/P3391)
6. [P4036 [JSOI2008] 火星人](https://www.luogu.com.cn/problem/P4036)
7. [P4774 [NOI2018] 屠龙勇士](https://www.luogu.com.cn/problem/P4774)
8. [P6136 【模板】普通平衡树（数据加强版）](https://www.luogu.com.cn/problem/P6136)
9. [U361730 【模板】完全体·堆](https://www.luogu.com.cn/problem/U361730)
10. [翻转排序](https://ac.nowcoder.com/acm/problem/275173)
11. [旅途的终点](https://ac.nowcoder.com/acm/problem/275989)
12. [正义从不打背身](https://ac.nowcoder.com/acm/problem/277862)
13. [Range Reverse Range Sum](https://judge.yosupo.jp/problem/range_reverse_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/538)


### 二、模板功能

​		本模板与 `MonoAVL` 功能相同，仅仅是选择的平衡树类型不同。

​		特别的，本模板支持区间翻转操作。

​		本模板通过模板参数 `typename Monoid` 确定幺半群。幺半群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 声明 `sum_type` 为值类型，且可以和 `value_type` 双向类型转换。若无此声明，默认等同于 `value_type` ；

3. 定义静态函数 `op` ，接受两个 `sum_type` 参数，返回它们的聚合值；

4. 定义静态函数 `identity` ，无输入参数，返回幺元。

5. 定义静态函数 `reversed` ，输入一个 `sum_type` 参数，返回其翻转值。所谓翻转值，即若 `S1 = op(a, b, c)` ， `S2 = op(c, b, a)` ，则 `S2` 和 `S1` 互为翻转值。


​		幺半群的 `value_type` 不可以为 `void` 。

​		代数结构可以没有 `op` 函数。此时也不需要 `identity` 以及 `reversed`  函数，本模板仅维护每个位置的单点信息。

​		在幺半群有 `op` 函数时，必须有 `identity` ，但是 `reversed` 函数为可选项。若有 `reversed` 函数，表示本半群可以通过 `reversed` 函数快速获取翻转值，从而避免了维护翻转值信息的时空开销；若无 `reversed` 函数，表示本半群无法快速计算翻转值，所以必须在模板内付出开销维护翻转值。

​		当模板参数 `MaintainReverse` 为 `false` 时，表示本模板不支持 `reverse` 操作，此时不需要考虑翻转信息的相关设置。

​		本模板要求区间操作函数的运算符满足**结合律**。

​		为了方便使用，本模板提供了 `MinSplay` ， `MaxSplay` ， `GcdSplay` ， `LcmSplay` ， `BitAndSplay` ， `BitOrSplay` ， `BitXorSplay` ， `SumSplay` 八种特化形式，需要填写的参数更少。



### 三、模板示例

```c++
#include "DS/MonoSplay.h"
#include "IO/FastIO.h"

void test() {
    cout << "test of no pushup:\n";
    OY::MonoSplaySequence<int, true> S;
    S.insert(0, 100);
    S.insert(1, 30);
    S.insert(1, 50);
    S.insert(1, 120);
    cout << "S : " << S << endl;

    S.reverse(1, 2);
    cout << "S : " << S << endl;
    S.insert(4, 70);
    cout << "S : " << S << endl;
    S.modify(1, 11);
    cout << "S : " << S << endl;
    S.erase(2);
    cout << "S : " << S << endl;

    auto S2 = OY::MonoSplaySequence<int, true>::from_mapping(3, [&](int i) { return (i + 1) * 40; });
    cout << "S2 : " << S2 << endl;

    S2.join(S.split(2));
    cout << "S : " << S << endl;
    cout << "S2 : " << S2 << endl
         << endl;
}

void test_fast_reverse() {
    cout << "test of fast reverse:\n";
    struct Monoid {
        using value_type = int;
        using sum_type = int64_t;
        static value_type identity() { return 1; }
        static sum_type op(sum_type x, sum_type y) { return x * y; }
        static sum_type reversed(sum_type x) { return x; }
    };
    OY::MONOSPLAY::Tree<Monoid, true> S;
    cout << "prod = " << S.query_all() << endl;

    S.insert(0, 100);
    S.insert(1, 30);
    S.insert(1, 50);
    S.insert(1, 120);
    cout << "S : " << S << endl;
    cout << "prod = " << S.query_all() << endl;
    S.reverse(1, 2);
    cout << "S : " << S << endl;
    S.insert(4, 70);
    cout << "S : " << S << endl;
    S.modify(1, 11);
    cout << "S : " << S << endl;
    S.erase(2);
    cout << "S : " << S << endl;
    cout << "prod(S[1~2]) = " << S.query(1, 2) << endl
         << endl;
}

void test_slow_reverse() {
    cout << "test of slow reverse:\n";
    struct Monoid {
        using value_type = std::string;
        static value_type identity() { return ""; }
        static value_type op(value_type x, value_type y) { return x + y; }
    };
    OY::MONOSPLAY::Tree<Monoid, true> S;

    S.insert(0, "a");
    S.insert(1, "c");
    S.insert(1, "f");
    S.insert(1, "e");
    S.insert(3, "d");
    S.insert(0, "b");
    cout << "S : " << S << endl;
    cout << "prod = " << S.query_all() << endl;
    S.reverse(1, 3);
    cout << "S : " << S << endl;
    S.modify(1, "z");
    cout << "S : " << S << endl;
    cout << "prod(S[0~2]) = " << S.query(0, 2) << endl;
    cout << "prod(S[1~4]) = " << S.query(1, 4) << endl;
    S.reverse(0, 5);
    cout << "S : " << S << endl;
    cout << "prod(S[1~4]) = " << S.query(1, 4) << endl;
}

int main() {
    test();
    test_fast_reverse();
    test_slow_reverse();
}
```

```
#输出如下
test of no pushup:
S : {100, 120, 50, 30}
S : {100, 50, 120, 30}
S : {100, 50, 120, 30, 70}
S : {100, 11, 120, 30, 70}
S : {100, 11, 30, 70}
S2 : {40, 80, 120}
S : {100, 11}
S2 : {40, 80, 120, 30, 70}

test of fast reverse:
prod = 1
S : {100, 120, 50, 30}
prod = 18000000
S : {100, 50, 120, 30}
S : {100, 50, 120, 30, 70}
S : {100, 11, 120, 30, 70}
S : {100, 11, 30, 70}
prod(S[1~2]) = 330

test of slow reverse:
S : {b, a, e, f, d, c}
prod = baefdc
S : {b, f, e, a, d, c}
S : {b, z, e, a, d, c}
prod(S[0~2]) = bze
prod(S[1~4]) = zead
S : {c, d, a, e, z, b}
prod(S[1~4]) = daez

```

