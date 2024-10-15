### 一、模板类别

​	数据结构：标记永久化线段树。

​	练习题目：

1. [#P4636. 蒟蒻的数列](https://new.bzoj.org:88/p/P4636)
2. [#132. 树状数组 3 ：区间修改，区间查询](https://loj.ac/p/132)
3. [P3372 【模板】线段树 1](https://www.luogu.com.cn/problem/P3372)
4. [P5057 [CQOI2006] 简单题](https://www.luogu.com.cn/problem/P5057)
5. [U187320 【模板】树状数组 3](https://www.luogu.com.cn/problem/U187320)

### 二、模板功能

​		本数据结构为标记永久化版本的线段树。当修改类型和查询的信息类型为同一类型，运算也相同时，使用本模板。

​		本模板支持区间修改。

​		本模板通过模板参数 `typename CommutativeMonoid` 确定幺半群。幺半群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；

3. 定义静态函数 `identity` ，无输入参数，返回幺元。

4. 定义静态函数 `pow` ，接受一个 `value_type x` 参数和一个 `size_type n` 翻倍倍数，返回 `n` 个 `x` 的聚合值。


    本模板要求区间操作函数的运算符满足**结合律** 和 **交换律**。

​		为了方便使用，本模板提供了 `MinTree` ， `MaxTree` ， `GcdTree` ， `LcmTree` ， `BitAndTree` ， `BitOrTree` ， `BitXorTree` ， `SumTree` 八种特化形式，需要填写的参数更少。

### 三、模板示例

```c++
#include "DS/TagSegTree.h"
#include "IO/FastIO.h"

void test_sum_tree() {
    // 最可能用到的可区间加的求和树
    cout << "test sum tree:\n";

    using SumTree = OY::VectorTagSumSeg<int64_t>;
    SumTree S(100);
    cout << S.query_all() << endl;

    S.add(0, 80, 1);
    cout << S.query(75, 90) << endl
         << endl;
}

void test_max_tree() {
    // 可能用到的可区间最大化的最值树，默认值设为 -1
    cout << "test max tree:\n";

    using MaxTree = OY::VectorTagMaxSeg<int64_t, -1>;
    MaxTree S(100);
    cout << S.query_all() << endl;

    S.add(0, 80, 100);
    cout << S.query(75, 90) << endl
         << endl;
}

void test_prod_tree() {
    // 展示用法的可区间乘的求积树
    cout << "test prod tree:\n";

    struct MulMonoid {
        using value_type = double;
        static value_type op(value_type x, value_type y) { return x * y; }
        static value_type identity() { return 1; }
        static value_type pow(value_type x, uint64_t n) { return ::pow(x, n); }
    };
    using ProdTree = OY::TAGSEG::Tree<MulMonoid>;
    ProdTree S(5);
    cout.precision(1);
    cout << S << endl;
    cout << S.query_all() << endl;

    S.add(1, 3, 2);
    cout << S << endl;
    cout << S.query(0, 2) << endl;
}

int main() {
    test_sum_tree();
    test_max_tree();
    test_prod_tree();
}
```

```
#输出如下
test sum tree:
0
6

test max tree:
-1
100

test prod tree:
[1.0, 1.0, 1.0, 1.0, 1.0]
1.0
[1.0, 2.0, 2.0, 2.0, 1.0]
4.0

```

