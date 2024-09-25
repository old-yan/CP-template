### 一、模板类别

​	数据结构： `W` 树（ `SIMD` 线段树）。

​	练习题目：

1. [#130. 树状数组 1 ：单点修改，区间查询](https://loj.ac/p/130)
2. [#131. 树状数组 2 ：区间修改，单点查询](https://loj.ac/p/131)
3. [P1972 [SDOI2009] HH的项链](https://www.luogu.com.cn/problem/P1972)
4. [P3312 [SDOI2014] 数表](https://www.luogu.com.cn/problem/P3312)
5. [P3368 【模板】树状数组 2](https://www.luogu.com.cn/problem/P3368)
6. [P3374 【模板】树状数组 1](https://www.luogu.com.cn/problem/P3374)
7. [P4145 上帝造题的七分钟 2 / 花神游历各国](https://www.luogu.com.cn/problem/P4145)
8. [P4178 Tree](https://www.luogu.com.cn/problem/P4178)
9. [P5057 [CQOI2006] 简单题](https://www.luogu.com.cn/problem/P5057)
10. [P10814 【模板】离线二维数点](https://www.luogu.com.cn/problem/P10814)
11. [Point Add Range Sum](https://judge.yosupo.jp/problem/point_add_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/20)
12. [Rectangle Sum](https://judge.yosupo.jp/problem/rectangle_sum)(https://github.com/yosupo06/library-checker-problems/issues/118)
13. [Vertex Add Path Sum](https://judge.yosupo.jp/problem/vertex_add_path_sum)(https://github.com/yosupo06/library-checker-problems/issues/125)
14. [Vertex Add Subtree Sum](https://judge.yosupo.jp/problem/vertex_add_subtree_sum)(https://github.com/yosupo06/library-checker-problems/issues/167)
15. [Static Range Count Distinct](https://judge.yosupo.jp/problem/static_range_count_distinct)(https://github.com/yosupo06/library-checker-problems/issues/770)
16. [Rectangle Add Point Get](https://judge.yosupo.jp/problem/rectangle_add_point_get)(https://github.com/yosupo06/library-checker-problems/issues/994)



### 二、模板功能

#### 1.建立 W 树

1. 数据类型

   类型设定 `size_type = size_t` ，表示 `W` 树中下标编号的变量类型。

   模板参数 `typename Tp` ，表示元素类型。

   模板参数 `typename Operation` ，表示操作类型。

   构造参数 `size_type length` ，表示 `W` 树的覆盖范围为 `[0, length)`。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。

2. 时间复杂度

    $O(n)$ 。

3. 备注

    `W` 树处理的问题为单点增值修改，前缀和的查询。可以类比于 `RangeUpdate` 为 `false` 的树状数组，但是常数更小。

   本模板支持传递自定义类作为运算类型。

    `W` 树通过加和函数求解前缀和，再通过前缀和做差求解任意区间和。

   **注意：**

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `Tp` 对象。若不传递此参数，表示不进行初始化，比如要建立一颗空的求和 `W` 树，由于缓冲区初始值本身就是零，所以无需进行初始化。

#### 2.单点增值(add)

1. 数据类型

   输入参数 `size_type i​` ，表示单点增值的下标。

   输入参数 `const Tp &inc​` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 3.前缀和查询(presum)

1. 数据类型

   输入参数 `size_type i`，表示前缀和查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 4.区间查询(query)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。
   
3. 备注

   本方法只有在做差函数合理的时候有意义。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`，且 `left <= right + 1` ）


#### 5.查询全部和(query_all)

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。


### 三、模板示例

```c++
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
 * 求和功能 W 树
 */
void test_add() {
    cout << "test of add WTree:" << endl;
    // 当以长度进行构造的时候，树中元素均为默认初始值
    OY::WTree::Tree<int> T(10);
    cout << T << endl;
    // 单点修改
    for (int i = 0; i < 10; i++) T.add(i, i * 100);
    cout << T << endl;
    // 输出前缀和
    cout << "presum(5) = " << T.presum(5) << endl;
    cout << "presum(3) = " << T.presum(3) << endl;
    // 输出区间和
    cout << "sum(4 ~ 5) = " << T.query(4, 5) << endl;
    cout << endl;
}

/*
 * 求异或和功能 W 树
 */
void test_xor() {
    cout << "test of xor WTree:" << endl;
    // 当以长度进行构造的时候，树中元素均为默认初始值
    OY::WTree::Tree<int, OY::WTree::BitXor> T(10, [](int i) {
        return i * 100;
    });
    cout << T << endl;
    // 输出前缀和
    cout << "presum(3) = " << T.presum(3) << endl;
    cout << "presum(2) = " << T.presum(2) << endl;
    // 查询 3 处的值
    cout << "query(3) = " << T.query(3, 3) << endl;
    cout << endl;
}

int main() {
    test_add();
    test_xor();
}
```

```
#输出如下
test of add WTree:
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 100, 300, 600, 1000, 1500, 2100, 2800, 3600, 4500]
presum(5) = 1500
presum(3) = 600
sum(4 ~ 5) = 900

test of xor WTree:
[0, 100, 172, 384, 16, 484, 956, 256, 544, 420]
presum(3) = 384
presum(2) = 172
query(3) = 300


```

