### 一、模板类别

​	数据结构：二维 `Zkw`线段树（ `Zkw` 线段树套 `Zkw` 线段树）

​	练习题目：

1. [Luck and Love](https://acm.hdu.edu.cn/showproblem.php?pid=1823)
2. [Check Corners](http://acm.hdu.edu.cn/showproblem.php?pid=2888)
3. [Mosaic](https://acm.hdu.edu.cn/showproblem.php?pid=4819)
4. [P2216 [HAOI2007] 理想的正方形](https://www.luogu.com.cn/problem/P2216)



### 二、模板功能

#### 1.建立二维线段树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中结点在内存池上的下标。

   模板参数 `typename Node` ，表示结点类型。

   构造参数 `size_type row` ，表示线段树的行范围为 `[0, row)`。默认值为 `0` 。

   构造参数 `size_type column` ，表示线段树的列范围为 `[0, column)`。默认值为 `0` 。

   构造参数 `InitMapping init_mapping` ，表示在初始化时，从下标到值的映射函数。默认为 `Zkw2D::Ignore` 。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等

2. 时间复杂度

    $O(m\cdot n)$ ，其中 `m` 和 `n` 分别表示行数和列数。

3. 备注

   二维 `Zkw` 线段树处理的问题为单点修改，单点/区域和的查询。

   二维 `Zkw` 线段树可以实现二维树状数组的绝大部分功能，而且可以实现更强的功能，比如区域最值查询、区域按位与、区域按位或。但是不能实现二维树状数组加强版的区域修改功能。这是因为二维树状数组本身也不能实现区域修改功能，加强版的区域修改功能只不过是在加法运算符、差分性质下做到的特例。

   不同于二维树状数组，二维 `Zkw` 线段树对任意区域的查询，都是通过自下而上的累积而来，而非通过差分求得。

   本二维 `Zkw` 线段树模板与之前版本的重大区别为，不再支持乘法/除法或者其他类型的模板函数传递，目前二维 `Zkw` 线段树的大量细节都放到了模板参数 `Node` 中，只需要设计好 `Node` 即可让线段树工作。

   对于二维 `Zkw` 线段树来说，结点须满足以下要求：

   1. 声明 `value_type` 为值类型；
   2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；
   3. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   4. 实现成员函数 `get` ，返回本结点的值。

   至此，二维线段树所需的结点功能已经足够。此外，还可以添加如下功能：

   1. 声明 `modify_type` 为修改类型；
   2. 定义静态函数 `map` ，接受一个修改值和一个结点指针，对该结点进行相应修改。如果没有定义本函数，则会把修改值和结点的旧值通过 `op` 函数聚合之后，赋给结点；

   一般的，我们称 `op` 函数执行的是聚合操作，二维 `Zkw` 线段树要求区间操作函数的运算符满足**结合律**和**交换律**。

   **注意：**

   构造参数中的 `mapping` 参数，入参为行下标、列下标，返回值为一个 `value_type` 对象。默认情况下， `mapping` 为 `Zkw2D::Ignore` 类，表示不进行初始化，比如要建立一颗空的求和二维 `Zkw` 线段树。



####  2.重置(resize)

1. 数据类型

   输入参数 `size_type row` ，表示二维线段树的行范围为 `[0, row)`。

   输入参数 `size_type column` ，表示二维线段树的列范围为 `[0, column)`。

   构造参数 `InitMapping init_mapping` ，表示在初始化时，从行列下标到值的映射函数。

2. 时间复杂度

   $O(m\cdot n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到线段树里。

   参数中的 `mapping` 参数，入参为行下标、列下标，返回值须为一个 `value_type` 对象。在调用时，会按照行下标从 `0` 到 `row-1` 、列下标从 `0` 到 `column-1` 依次调用。

#### 3.单点增值(add)

1. 数据类型

   输入参数 `size_type i` ，表示单点增值的行下标。

   输入参数 `size_type j` ，表示单点增值的列下标。

   输入参数 `const modify_type &modify​` ，表示增量大小。

2. 时间复杂度

   $O(\log m\cdot\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 4.单点赋值(modify)

1. 数据类型

   输入参数 `size_type i` ，表示单点赋值的行下标。

   输入参数 `size_type j` ，表示单点赋值的列下标。

   输入参数 `const value_type &val​` ，表示赋的值。

2. 时间复杂度

   $O(\log m\cdot\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 5.单点查询(query)

1. 数据类型

   输入参数 `size_type row` ，表示单点查询的行下标。

   输入参数 `size_type column` ，表示单点查询的列下标。

2. 时间复杂度

   $O(1)$ 。

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

#### 8.从某些行中查询第k个元素(kth)

1. 数据类型

   输入参数 `size_type row1` ，表示区域查询的起始行下标。

   输入参数 `size_type row2` ，表示区域查询的结束行下标。（闭区间）

   输入参数 `value_type k​` ，表示查询的名次。

2. 时间复杂度

    $O(\log m\cdot\log n)$ 。

3. 备注

   本函数可以这样表述：找到最小的列号 `c` ，使得 `query(row1, row2, 0, c) >= k + 1` 成立。

   本函数常用于维护带修区间第 `k` 大元素。


### 三、模板示例

```c++
#include "DS/ZkwTree2D.h"
#include "IO/FastIO.h"

int main() {
    // 二维线段树与一维线段树的用法基本类似，所以只展示一下特殊方法
    int matrix[4][5] = {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
    };
    // 除了行数、列数，还需要传递一个寻址函数
    OY::ZkwSumTree2D<int> T(4, 5, [&](int i, int j) { return matrix[i][j]; });
    cout << T << endl;
    // 输出[0,2]行，[1,4]列的和
    cout << "sum(matrix[0~2][1~4])=" << T.query(0, 2, 1, 4) << endl;

    // 来完成一个二维树状数组做不到的：区域最值
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    // 建立一个区间最大值累加器
    auto getmax = [](int x, int y) {
        return x > y ? x : y;
    };
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } getmax;
#endif
    auto T_max = OY::make_ZkwTree2D<int>(4, 5, getmax, [&](int i, int j) { return matrix[i][j]; });
    cout << T_max << endl;
    // 输出[0,2]行，[1,4]列的最大值
    cout << "max(matrix[0~2][1~4])=" << T_max.query(0, 2, 1, 4) << endl;
}
```

```
#输出如下
[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7]]
sum(matrix[0~2][1~4])=27
[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7]]
max(matrix[0~2][1~4])=6

```
