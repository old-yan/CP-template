### 一、模板类别

​	数据结构：标记永久化二维线段树。

​	练习题目：

1. [P3437 [POI2006] TET-Tetris 3D](https://www.luogu.com.cn/problem/P3437)

### 二、模板功能

​		显然， `SegTree2D` 并非 `SegTree` 在二维空间的延申，因为 `SegTree2D` 并未支持区间修改功能； `ZkwTree2D` 也非 `ZkwTree` 在二维空间的延申，因为 `ZkwTree2D` 也不支持区间修改功能。

​		有鉴于此，特设计本数据结构，以支持二维区间修改功能。本数据结构是 `SegTree` 在二维的延申，但是支持的功能较弱。

#### 1.建立二维线段树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中结点在内存池上的下标。

   模板参数 `typename CommutativeMonoid` ，表示交换幺半群类型。

   模板参数 `typename SizeType` ，表示表示区域大小时，所采用的数的类型。在区间长度为 `1e9` 级别时，采用 `uint32_t` 为佳；区域长度更长时，采用 `uint64_t` 。当然采用有符号数也无大碍。

   构造参数 `SizeType row` ，表示二维线段树的行范围为 `[0, row)`。默认值为 `0` 。

   构造参数 `SizeType column` ，表示二维线段树的列范围为 `[0, column)`。默认值为 `0` 。

   构造参数 `InitMapping init_mapping` ，表示在初始化时，从区域到区域聚合值的映射函数。。

2. 时间复杂度

   当 `Complete` 参数为 `true` 时，为 $O(m\cdot n)$ ，其中 `m` 和 `n` 分别表示行数和列数；否则为 $O(1)$ 。

3. 备注

   基础二维线段树处理的问题为单点修改，单点/区域和的查询。

   标记永久化的二维线段树处理的问题为单点/区间修改，单点/区域和的查询。
   
   目前标记永久化的二维线段树的大量细节都放到了模板参数 `CommutativeMonoid` 中，只需要设计好 `CommutativeMonoid` 即可让线段树工作。

   对于二维线段树来说，交换幺半群须满足以下要求：

   1. 声明 `value_type` 为值类型；
   
   2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；
   
   3. 定义静态函数 `identity` ，无输入参数，返回幺元。
   
   4. 定义静态函数 `pow` ，接受一个 `value_type x` 参数和一个 `SizeType n` 参数，返回 `n` 个 `x` 的聚合值。

   一般的，我们称 `op` 函数执行的是聚合操作，二维线段树要求区间操作函数的运算符满足**结合律**和**交换律**。


#### 2.重置(resize)

1. 数据类型

   输入参数 `SizeType row` ，表示二维线段树的行范围为 `[0, row)`。

   输入参数 `SizeType column` ，表示二维线段树的列范围为 `[0, column)`。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

   当需要有初始化操作时，为 $O(m\cdot n)$ ，其中 `m` 和 `n` 分别表示行数和列数；否则为 $O(1)$ 。

3. 备注

   使用映射函数进行初始化，可以将区域初状态直接赋到二维线段树里。

    `mapping` 要求传入一个行号，一个列号，返回一个 `value_type` 类型的值。在调用时，会按照行下标从 `0` 到 `row-1` 、列下标从 `0` 到 `column-1` 依次调用。


#### 3.单点增值(add)

1. 数据类型

   输入参数 `SizeType row` ，表示单点增值的行下标。

   输入参数 `SizeType column` ，表示单点增值的列下标。

   输入参数 `const value_type &modify​` ，表示增量大小。

2. 时间复杂度

   $O(\log m\cdot\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 4.区域增值(add)

1. 数据类型

   输入参数 `SizeType row1` ，表示区域增值的起始行下标。

   输入参数 `SizeType row2` ，表示区域增值的结束行下标。（闭区间）

   输入参数 `SizeType column1` ，表示区域增值的起始列下标。

   输入参数 `SizeType column2` ，表示区域增值的结束列下标。（闭区间）

   输入参数 `const value_type &modify​` ，表示增量大小。

2. 时间复杂度

   $O(\log m\cdot\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 5.单点查询(query)

1. 数据类型

   输入参数 `SizeType row` ，表示单点查询的行下标。

   输入参数 `SizeType column` ，表示单点查询的列下标。。

2. 时间复杂度

   $O(\log m + \log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）


#### 6.区域查询(query)

1. 数据类型

   输入参数 `SizeType row1` ，表示区域查询的起始行下标。

   输入参数 `SizeType row2` ，表示区域查询的结束行下标。（闭区间）

   输入参数 `SizeType column1​` ，表示区域查询的起始列下标。

   输入参数 `SizeType column2` ，表示区域查询的结束列下标。（闭区间）

2. 时间复杂度

   $O(\log m\cdot\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 7.查询全部(query_all)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。


### 三、模板示例

```c++
#include "DS/TagSegTree2D.h"
#include "IO/FastIO.h"

void test_sum_tree() {
    // 最可能用到的二维可区域加的求和树
    cout << "test sum tree:\n";

    using SumTree = OY::VectorTagSumSeg2D<int64_t>;
    SumTree S(10000000000, 10000000000);
    cout << S.query_all() << endl;

    S.add(0, 8000000000, 0, 8000000000, 1);
    cout << S.query(7500000000, 9000000000, 7500000000, 9000000000) << endl
         << endl;
}

void test_max_tree() {
    // 可能用到的二维可区域最大化的最值树，默认值设为 -1
    cout << "test max tree:\n";

    using MaxTree = OY::VectorTagMaxSeg2D<int64_t, -1>;
    MaxTree S(10000000000, 10000000000);
    cout << S.query_all() << endl;

    S.add(0, 8000000000, 0, 8000000000, 100);
    cout << S.query(7500000000, 9000000000, 7500000000, 9000000000) << endl
         << endl;
}

void test_prod_tree() {
    // 展示用法的二维可区域乘的求积树
    cout << "test prod tree:\n";

    struct MulMonoid {
        using value_type = double;
        static value_type op(value_type x, value_type y) { return x * y; }
        static value_type identity() { return 1; }
        static value_type pow(value_type x, uint64_t n) { return ::pow(x, n); }
    };
    using ProdTree = OY::TagSEG2D::Tree<MulMonoid>;
    ProdTree S(5, 7);
    cout.precision(1);
    cout << S << endl;
    cout << S.query_all() << endl;

    S.add(1, 3, 1, 4, 2);
    cout << S << endl;
    cout << S.query(0, 2, 0, 2) << endl;
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
250000001000000001

test max tree:
-1
100

test prod tree:
[[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]]
1.0
[[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]]
16.0

```

