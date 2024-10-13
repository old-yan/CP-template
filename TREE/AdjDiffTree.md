### 一、模板类别

​	数据结构：树差分。

​	练习题目：

1. [P3128 [USACO15DEC] Max Flow P](https://www.luogu.com.cn/problem/P3128)
2. [P3258 [JLOI2014] 松鼠的新家](https://www.luogu.com.cn/problem/P3258)
3. [剖分](https://ac.nowcoder.com/acm/problem/244121)

### 二、模板功能

#### 1.初始化

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示矩阵大小的类型。

   模板参数 `typename CommutativeGroup` ，表示交换群类型。

   模板参数 `typename Tree` ，表示树的类型。

   模板参数 `bool AutoSwitch` ，表示是否自动切换状态。

   构造参数 `Tree *rooted_tree`  ，表示要维护的树，且必须已经指定根。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。

2. 时间复杂度

    $O(n)$ ，其中 `n` 表示树的大小。

3. 备注

   本模板通过模板参数 `typename CommutativeGroup` 确定交换群。交换群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；

3. 定义静态函数 `identity` ，无输入参数，返回幺元。

4. 定义静态函数 `inverse` ，输入参数一个 `value_type` ，返回其逆元。

    本模板要求区间操作函数的运算符满足**结合律**和**交换律**。常见的交换群为加法群和异或群。

   本数据结构，处于五种状态之一，且可以随时切换。

   当处于向上差分态 `TABLE_DIFFERENCE_UPWARD` 时，便于进行“将某个路径增加一定的值”操作；

   当处于向上前缀和态 `TABLE_PRESUM_UPWARD` 时，便于进行“查询某个路径的和”操作。

   当处于向下差分态 `TABLE_DIFFERENCE_DOWNWARD` 时，便于进行“将某个子树增加一定的值”操作；

   当处于向下前缀和态 `TABLE_PRESUM_DOWNWARD` 时，便于进行“查询某个子树的和”操作。

   当处于值态时 `TABLE_VALUE` ，便于进行“将某个点增加一定的值”或者“查询某点的值”操作；

   状态切换需要 $O(n)$ 的时间，所以使用时尽量避免频繁切换状态。

   如果担心忘记切换状态，可以将 `AutoSwitch` 设为 `true` ，则在调用函数时会自动切换到最合适的状态。
   
   **注意：**
   
   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。如要建立一颗空的差分表，此时的初状态为 `TABLE_ANY` 态，可以认为为任意状态。如果进行了有意义的初始化，则初状态为 `TABLE_VALUE` 态。

#### 2.重置(reset)

1. 数据类型

   输入参数 `Tree *rooted_tree`  ，表示要维护的树，且必须已经指定根。

   输入参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。

2. 时间复杂度

   $O(n)$ ，其中 `n` 表示树的大小。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到差分表里。

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。在调用时，会按照编号从 `0` 到 `n-1` 依次调用。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于 `[0，n)`）

#### 3.单点增值(add)

1. 数据类型

   输入参数 `size_type i` ，表示要增值的点的编号。

   输入参数 `value_type inc`​ ，表示要增加的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在值态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（编号位于 `[0，n)`）

#### 4.单点赋值(modify)

1. 数据类型

   输入参数 `size_type i` ，表示要赋值的点的编号。

   输入参数 `value_type inc`​ ，表示要赋的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在值态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（编号位于 `[0，n)`）

#### 5.子树增值(add_subtree)

1. 数据类型

   输入参数 `size_type root` ，表示要增值的子树的根结点的编号。

   输入参数 `value_type inc`​ ，表示要增加的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在向下差分态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（编号位于 `[0，n)`）

#### 6.路径增值(add_path)

1. 数据类型

   输入参数 `size_type i` ，表示要增值的路径的下方端点的编号。

   输入参数 `size_type g` ，表示要增值的路径的上方端点的编号。

   输入参数 `size_type gp` ，表示结点 `g` 的父结点的编号；若 `g` 没有父结点，则传 `-1` 。

   输入参数 `value_type inc`​ ，表示要增加的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在向上差分态下进行。

   本操作的**路径**特指一条从上到下的路径，即，路径中不存在两个相同深度的结点。

   本函数没有进行参数检查，所以请自己确保下标合法。（编号位于 `[0，n)`）

#### 7.路径增值(add_path)

1. 数据类型

   输入参数 `size_type i` ，表示要增值的路径的一段的编号。

   输入参数 `size_type j` ，表示要增值的路径的另一段的编号。

   输入参数 `size_type lca` ，表示结点 `i` 和结点 `j` 的最近公共祖先的编号。

   输入参数 `size_type lca` ，表示结点 `lca` 的父结点的编号；若 `lca` 没有父结点，则传 `-1` 。

   输入参数 `value_type inc`​ ，表示要增加的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在向上差分态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（编号位于 `[0，n)`）

#### 8.单点查询(query)

1. 数据类型

   输入参数 `size_type i` ，表示要增值的点的编号。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在值态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（编号位于 `[0，n)`）

#### 9.子树和查询(query_subtree)

1. 数据类型

   输入参数 `size_type root` ，表示要查询的子树的根结点。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在向上前缀和态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（编号位于 `[0，n)`）

#### 10.路径和查询(query_path)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的路径的下方端点的编号。

   输入参数 `size_type g` ，表示要查询的路径的上方端点的编号。

   输入参数 `size_type gp` ，表示结点 `g` 的父结点的编号；若 `g` 没有父结点，则传 `-1` 。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本操作须在向下前缀和态下进行。

   本操作的**路径**特指一条从上到下的路径，即，路径中不存在两个相同深度的结点。

   本函数没有进行参数检查，所以请自己确保下标合法。（编号位于 `[0，n)`）

#### 11.路径和查询(query_path)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的路径的一段的编号。

   输入参数 `size_type j` ，表示要查询的路径的另一段的编号。

   输入参数 `size_type lca` ，表示结点 `i` 和结点 `j` 的最近公共祖先的编号。

   输入参数 `size_type lcap` ，表示结点 `lca` 的父结点的编号；若 `lca` 没有父结点，则传 `-1` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在向下前缀和态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（编号位于 `[0，n)`）

#### 12.查询全部(query_all)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在向上前缀和态下进行。


#### 13.切换状态到向下差分态(switch_to_difference_downward)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   切换到向下差分态，以便进行子树增值。

#### 14.切换状态到向下前缀和态(switch_to_presum_downward)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   切换到向下前缀和态，以便进行路径查询。

#### 15.切换状态到向上差分态(switch_to_difference_upward)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   切换到向下差分态，以便进行路径和增值。

#### 16.切换状态到向上前缀和态(switch_to_presum_upward)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   切换到向下差分态，以便进行子树和查询、整树和查询。

#### 17.切换状态到值态(switch_to_value)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   切换到值态，以便进行单点修改和单点查询。

   **注意：**如果要进行全树输出，也请切换到值态。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/AdjDiffTree.h"
#include "TREE/FlatTree.h"

// 手动模式
void Ad_manual() {
    // 树可以是 FlatTree LinkTree 或者 VectorTree 均可
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> T(6);
    // 加边
    T.add_edge(0, 1);
    T.add_edge(0, 2);
    T.add_edge(0, 3);
    T.add_edge(3, 4);
    T.add_edge(3, 5);
    // 预备
    T.prepare();
    T.set_root(0);
    cout << T << endl;

    // 假定每个点的初值都是编号 * 1000
    OY::AdjSumTreeTable<int, decltype(T), false> Ad(&T, [&](int i) {
        return i * 1000;
    });
    cout << Ad << endl;

    // 子树增值
    Ad.switch_to_difference_downward();
    Ad.add_subtree(3, 1);
    Ad.switch_to_value();
    cout << Ad << endl;

    // 路径增值
    Ad.switch_to_difference_upward();
    Ad.add_path(1, 4, 0, -1, 10);
    Ad.switch_to_value();
    cout << Ad << endl;

    // 查询子树和
    Ad.switch_to_presum_upward();
    cout << "sum of subtree(3) = " << Ad.query_subtree(3) << endl;

    // 查询路径和
    Ad.switch_to_presum_downward();
    cout << "sum of path(1~4) = " << Ad.query_path(1, 4, 0, -1) << endl;
}

// 自动模式
void Ad_auto() {
    OY::FlatTree::Tree<bool, 1000> T(6);
    T.add_edge(0, 1);
    T.add_edge(0, 2);
    T.add_edge(0, 3);
    T.add_edge(3, 4);
    T.add_edge(3, 5);
    T.prepare();
    T.set_root(0);
    cout << T << endl;

    OY::AdjSumTreeTable<int, decltype(T), true> Ad(&T, [&](int i) {
        return i * 1000;
    });
    cout << Ad << endl;

    // 子树增值
    Ad.add_subtree(3, 1);
    cout << Ad << endl;

    // 路径增值
    Ad.add_path(1, 4, 0, -1, 10);
    cout << Ad << endl;

    // 查询子树和
    cout << "sum of subtree(3) = " << Ad.query_subtree(3) << endl;

    // 查询路径和
    cout << "sum of path(1~4) = " << Ad.query_path(1, 4, 0, -1) << endl;
}

int main() {
    Ad_manual();
    Ad_auto();
}
```

```
#输出如下
[0[1][2][3[4][5]]]
[0[1000][2000][3000[4000][5000]]]
[0[1000][2000][3001[4001][5001]]]
[10[1010][2000][3011[4011][5001]]]
sum of subtree(3) = 12023
sum of path(1~4) = 8042
[0[1][2][3[4][5]]]
[0[1000][2000][3000[4000][5000]]]
[0[1000][2000][3001[4001][5001]]]
[10[1010][2000][3011[4011][5001]]]
sum of subtree(3) = 12023
sum of path(1~4) = 8042

```

