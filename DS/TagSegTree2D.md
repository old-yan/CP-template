### 一、模板类别

​	数据结构：标记永久化二维线段树。

​	练习题目：

1. [P3437 [POI2006] TET-Tetris 3D](https://www.luogu.com.cn/problem/P3437)

### 二、模板功能

​		显然， `SegTree2D` 并非 `SegTree` 在二维空间的延申，因为 `SegTree2D` 并未支持区间修改功能； `ZkwTree2D` 也非 `ZkwTree` 在二维空间的延申，因为 `ZkwTree2D` 也不支持区间修改功能。

​		有鉴于此，特设计本数据结构，以支持二维区间修改功能。本数据结构是 `SegTree` 在二维的延申，但是支持的功能较弱。

#### 1.建立二维线段树

1. 数据类型

   类型设定 `index_type = uint32_t` ，表示树中结点在内存池上的下标。

   模板参数 `typename Node` ，表示结点类型。

   模板参数 `typename RangeMapping` ，表示从区域映射到区域聚合值的映射函数所属的类。默认为 `TagSeg2D::Ignore` 。

   模板参数 `bool Complete` ，表示是否在构造时将整个区域初始化。默认为 `false` 。

   模板参数 `typename SizeType` ，表示表示区域大小时，所采用的数的类型。在区间长度为 `1e9` 级别时，采用 `uint32_t` 为佳；区域长度更长时，采用 `uint64_t` 。当然采用有符号数也无大碍。

   模板参数 `SizeType MAX_TREENODE` ，表示最大树结点数，默认为 `1<<20` 。
   
   模板参数 `SizeType MAX_NODE` ，表示最大结点数，默认为 `1<<22` 。

   构造参数 `SizeType row` ，表示二维线段树的行范围为 `[0, row)`。默认值为 `0` 。

   构造参数 `SizeType column` ，表示二维线段树的列范围为 `[0, column)`。默认值为 `0` 。

   构造参数 `InitMapping init_mapping` ，表示在初始化时，从区域到区域聚合值的映射函数。默认为 `TagSeg2D::Ignore` 。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

2. 时间复杂度

   当 `Complete` 参数为 `true` 时，为 $O(m\cdot n)$ ，其中 `m` 和 `n` 分别表示行数和列数；否则为 $O(1)$ 。

3. 备注

   基础二维线段树处理的问题为单点修改，单点/区域和的查询。

   标记永久化的二维线段树处理的问题为单点/区间修改，单点/区域和的查询。

   目前的标记永久化的二维线段树的大量细节都放到了模板参数 `Node` 中，只需要设计好 `Node` 即可让线段树工作。

   对于标记永久化的二维线段树来说，结点须满足以下要求：

   1. 声明 `value_type` 为值类型，标记类型；
   2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；
   3. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   4. 实现成员函数 `get` ，返回本结点的值；
   5. 实现成员函数 `set_tag` ，接受一个 `value_type` 参数，将此标记赋给本结点；
   6. 实现成员函数 `get_tag` ，返回本结点的标记值。

   至此，标记永久化的二维线段树所需的结点功能已经足够。此外，还可以添加如下功能：

   1. 定义静态函数 `applied` ，接受一个标记值，返回这个标记值所对应的对聚合值的贡献。如果没有定义本函数，则会把标记值本身作为最终的贡献；
   2. 定义静态函数 `applied` ，接受一个标记值，一个行宽度，和一个列宽度，返回这个标记值在对应子区域所产生的对聚合值的贡献。如果没有定义本函数，则会把标记值本身作为最终的贡献；
   3. 定义静态函数 `default_tag` ，返回默认标记值。如果没有定义本函数，则会把 `value_type` 的默认构造对象作为默认标记值；
   4. 定义成员函数 `clear_tag` ，将结点内的标记值清空。

   标记永久化的二维线段树要求区间操作函数的运算符满足**结合律**和**交换律**。为简单起见，标记之间的操作函数要求和值之间的操作函数相同。此处与一维线段树的区别就是，因为无法控制区间操作是先上下聚合再左右聚合，所以必须满足交换律。

   **注意：**

   模板参数中的 `RangeMapping` l类型，用于生成仿函数，入参为区域的首行下标、末行下标、首列下标、末列下标，返回值须为一个 `value_type` 对象，表示这个区域的初始聚合值。默认情况下， 为 `TagSeg2D::Ignore` 类，表示不进行初始化，比如要建立一颗空的二维求和线段树，由于全局变量值本身就是零，所以无需进行初始化。

   **注意：**

   正是由于 `TagSegTree2D` 不需要在一开始就初始化全部的结点，但有时 `TagSegTree2D` 需要一开始就全局初始化，所以产生了 `Complete` 参数和 `RangeMapping` 参数。

   显然， `InitMapping` 函数和 `RangeMapping` 函数互斥，当开局就全部初始化时，可以传递 `InitMapping` ，但是肯定不需要 `RangeMapping` 参数，因为区域聚合值都可以通过子结点聚合得到；如果开局不全部初始化，可以设置 `RangeMapping` 参数，但是不需要传递 `InitMapping` 参数。


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

   $O(\log m\cdot\log n)$ 。

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

template <typename Tp>
struct Zero {
    template <typename... Args>
    Tp operator()(Args...) const { return 0; }
};
void test_sum_tree() {
    // 最可能用到的二维可区域加的求和树
    cout << "test sum tree:\n";

    using SumTree = OY::TagSeg2D::Tree<OY::TagSeg2D::BaseNode<int64_t>, Zero<int64_t>, false, uint64_t, 1000, 10000>;
    SumTree S(10000000000, 10000000000);
    cout << S.query_all() << endl;

    S.add(0, 8000000000, 0, 8000000000, 1);
    cout << S.query(7500000000, 9000000000, 7500000000, 9000000000) << endl
         << endl;
}

template <typename ValueType, ValueType Min>
struct MaxNode {
    using value_type = ValueType;
    static value_type op(const value_type &x, const value_type &y) { return std::max(x, y); }
    static value_type applied(const value_type &x) { return x; }
    static value_type default_tag() { return Min; }
    value_type m_val, m_tag;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    const value_type &get_tag() const { return m_tag; }
    void set_tag(const value_type &tag) { m_tag = tag; }
    void clear_tag() { m_val = m_tag = Min; }
};
template <typename Tp>
struct Min {
    template <typename... Args>
    Tp operator()(Args...) const { return -1; }
};
void test_max_tree() {
    // 可能用到的二维可区域最大化的最值树，默认值设为 -1
    cout << "test max tree:\n";

    using MaxTree = OY::TagSeg2D::Tree<MaxNode<int64_t, -1>, Min<int64_t>, false, uint64_t, 1000, 10000>;
    MaxTree S(10000000000, 10000000000);
    cout << S.query_all() << endl;

    S.add(0, 8000000000, 0, 8000000000, 100);
    cout << S.query(7500000000, 9000000000, 7500000000, 9000000000) << endl
         << endl;
}

template <typename ValueType>
struct ProdNode {
    using value_type = ValueType;
    static value_type op(const value_type &x, const value_type &y) { return x * y; }
    static value_type applied(const value_type &x) { return x; }
    static value_type applied(const value_type &x, int row_width, int col_width) { return pow(x, row_width * col_width); }
    static value_type default_tag() { return 1; }
    value_type m_val, m_tag;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    const value_type &get_tag() const { return m_tag; }
    void set_tag(const value_type &tag) { m_tag = tag; }
    void clear_tag() { m_val = m_tag = 1; }
};
void test_prod_tree() {
    // 展示用法的二维可区域乘的求积树
    cout << "test prod tree:\n";

    using ProdTree = OY::TagSeg2D::Tree<ProdNode<double>, OY::TagSeg2D::Ignore, true, uint32_t, 1000, 10000>;
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

