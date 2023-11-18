### 一、模板类别

​	数据结构：标记永久化二维 `zkw` 树。

​	练习题目：

1. [P3437 [POI2006] TET-Tetris 3D](https://www.luogu.com.cn/problem/P3437)

### 二、模板功能

​		显然， `SegTree2D` 并非 `SegTree` 在二维空间的延申，因为 `SegTree2D` 并未支持区间修改功能； `ZkwTree2D` 也非 `ZkwTree` 在二维空间的延申，因为 `ZkwTree2D` 也不支持区间修改功能。

​		有鉴于此，特设计本数据结构，以支持二维区间修改功能。本数据结构是 `ZkwTree` 在二维的延申，但是支持的功能较弱。

#### 1.建立二维Zkw树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中结点在内存池上的下标。

   模板参数 `typename Node` ，表示结点类型。
   
   模板参数 `size_type MAX_NODE` ，表示最大结点数，默认为 `1<<22` 。

   构造参数 `size_type row` ，表示二维 `zkw` 树的行范围为 `[0, row)`。默认值为 `0` 。

   构造参数 `size_type column` ，表示二维 `zkw` 树的列范围为 `[0, column)`。默认值为 `0` 。

   构造参数 `InitMapping init_mapping` ，表示在初始化时，从区域到区域聚合值的映射函数。默认为 `TagZkw2D::Ignore` 。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

2. 时间复杂度

   $O(m\cdot n)$ 。

3. 备注

   基础二维 `zkw` 树处理的问题为单点修改，单点/区域和的查询。

   标记永久化的二维 `zkw` 树处理的问题为单点/区间修改，单点/区域和的查询。

   目前的标记永久化的二维 `zkw` 树的大量细节都放到了模板参数 `Node` 中，只需要设计好 `Node` 即可让线段树工作。

   对于标记永久化的二维 `zkw` 树来说，结点须满足以下要求：

   1. 声明 `value_type` 为值类型，标记类型；
   2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；
   3. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   4. 实现成员函数 `get` ，返回本结点的值；
   5. 实现成员函数 `set_tag` ，接受一个 `value_type` 参数，将此标记赋给本结点；
   6. 实现成员函数 `get_tag` ，返回本结点的标记值。

   至此，标记永久化的二维 `zkw` 树所需的结点功能已经足够。此外，还可以添加如下功能：

   1. 定义静态函数 `applied` ，接受一个标记值，返回这个标记值所对应的对聚合值的贡献。如果没有定义本函数，则会把标记值本身作为最终的贡献；
   2. 定义静态函数 `applied` ，接受一个标记值，一个行宽度，和一个列宽度，返回这个标记值在对应子区域所产生的对聚合值的贡献。如果没有定义本函数，则会把标记值本身作为最终的贡献；
   3. 定义静态函数 `default_tag` ，返回默认标记值。如果没有定义本函数，则会把 `value_type` 的默认构造对象作为默认标记值；
   4. 定义成员函数 `clear_tag` ，将结点内的标记值清空。

   标记永久化的二维 `zkw` 树要求区间操作函数的运算符满足**结合律**和**交换律**。为简单起见，标记之间的操作函数要求和值之间的操作函数相同。此处与一维 `zkw` 树的区别就是，因为无法控制区间操作是先上下聚合再左右聚合，所以必须满足交换律。


#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type row` ，表示二维 `zkw` 树的行范围为 `[0, row)`。

   输入参数 `size_type column` ，表示二维 `zkw` 树的列范围为 `[0, column)`。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

   $O(m\cdot n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区域初状态直接赋到二维 `zkw` 树里。

    `mapping` 要求传入一个行号，一个列号，返回一个 `value_type` 类型的值。在调用时，会按照行下标从 `0` 到 `row-1` 、列下标从 `0` 到 `column-1` 依次调用。


#### 3.单点增值(add)

1. 数据类型

   输入参数 `size_type row` ，表示单点增值的行下标。

   输入参数 `size_type column` ，表示单点增值的列下标。

   输入参数 `const value_type &modify​` ，表示增量大小。

2. 时间复杂度

   $O(\log m\cdot\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 4.区域增值(add)

1. 数据类型

   输入参数 `size_type row1` ，表示区域增值的起始行下标。

   输入参数 `size_type row2` ，表示区域增值的结束行下标。（闭区间）

   输入参数 `size_type column1` ，表示区域增值的起始列下标。

   输入参数 `size_type column2` ，表示区域增值的结束列下标。（闭区间）

   输入参数 `const value_type &modify​` ，表示增量大小。

2. 时间复杂度

   $O(\log m\cdot\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 5.单点查询(query)

1. 数据类型

   输入参数 `size_type row` ，表示单点查询的行下标。

   输入参数 `size_type column` ，表示单点查询的列下标。。

2. 时间复杂度

   $O(\log m\cdot\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）


#### 6.区域查询(query)

1. 数据类型

   输入参数 `size_type row1` ，表示区域查询的起始行下标。

   输入参数 `size_type row2` ，表示区域查询的结束行下标。（闭区间）

   输入参数 `size_type column1​` ，表示区域查询的起始列下标。

   输入参数 `size_type column2` ，表示区域查询的结束列下标。（闭区间）

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
#include "DS/TagZkwTree2D.h"
#include "IO/FastIO.h"

template <typename Tp>
struct Zero {
    template <typename... Args>
    Tp operator()(Args...) const { return 0; }
};
void test_sum_tree() {
    // 最可能用到的二维可区域加的求和树
    cout << "test sum tree:\n";

    using SumTree = OY::TagZkw2D::Tree<OY::TagZkw2D::BaseNode<int64_t>, 1000>;
    SumTree S(4, 5);
    cout << S << endl;
    cout << S.query_all() << endl;

    S.add(0, 2, 0, 3, 10);
    cout << S << endl;
    cout << S.query(1, 3, 1, 3) << endl
         << endl;
    ;
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
void test_max_tree() {
    // 可能用到的二维可区域最大化的最值树，默认值设为 -1
    cout << "test max tree:\n";

    using MaxTree = OY::TagZkw2D::Tree<MaxNode<int64_t, -1>, 1000>;
    MaxTree S(4, 5);
    cout << S << endl;
    cout << S.query_all() << endl;

    S.add(0, 2, 0, 3, 100);
    cout << S << endl;
    cout << S.query(1, 3, 1, 3) << endl
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

    using ProdTree = OY::TagZkw2D::Tree<ProdNode<double>, 1000>;
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
[[0, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0]]
0
[[10, 10, 10, 10, 0], [10, 10, 10, 10, 0], [10, 10, 10, 10, 0], [0, 0, 0, 0, 0]]
60

test max tree:
[[-1, -1, -1, -1, -1], [-1, -1, -1, -1, -1], [-1, -1, -1, -1, -1], [-1, -1, -1, -1, -1]]
-1
[[100, 100, 100, 100, -1], [100, 100, 100, 100, -1], [100, 100, 100, 100, -1], [-1, -1, -1, -1, -1]]
100

test prod tree:
[[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]]
1.0
[[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]]
16.0

```

