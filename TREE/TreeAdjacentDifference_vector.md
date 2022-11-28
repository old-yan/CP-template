### 一、模板类别

​	数据结构： 树上差分。

### 二、模板功能

  本模板与 `TreeAdjacentDiffrence` 模板完全相同。区别仅在于，本模板适用于 `VectorTree` 。

#### 1.预处理

1. 数据类型

   模板参数 `typename _Tree` ，表示树的类型。

   模板参数 `typename _Tp` ，表示树上结点的值的类型，默认为 `int64_t` 。

   模板参数 `typename _Operation` ，表示树上修改的操作类型，默认为 `std::plus<_Tp>` ，表示要对树上结点的值进行增加。

   模板参数 `typename _Inverse` ，表示在上述操作中， `_Tp` 类型元素的取逆元函数。对于加法操作来说，元素的逆元就是倒数，也就是加上一个元素，再加上该元素的倒数，就可以抵消掉。默认为 `std::negate<_Tp>` 表示倒数。

   构造参数 `_Tree &__tree`​ ，表示树。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   树上差分是离线方法，即只能将所有的修改全部完成后，调用求和方法，才能看到每个结点的当前值。
   

#### 2.在某条路径上增加值

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个结点。

   输入参数 `uint32_t __b` ，表示第二个结点。

   输入参数 `uint32_t __lca` ，表示两个结点的最近公共祖先。

   输入参数 `_Tp __inc` ，表示在两个点的路径上增加的值。

2. 时间复杂度

   $O(1)$ 。

#### 3.在某个子树上增加值

1. 数据类型

   输入参数 `uint32_t __a` ，表示子树根结点。

   输入参数 `_Tp __inc` ，表示在子树上增加的值。

2. 时间复杂度

   $O(1)$ 。

#### 4.在某点上增加值

1. 数据类型

   输入参数 `uint32_t __a` ，表示结点编号。

   输入参数 `_Tp __inc` ，表示在点上增加的值。

2. 时间复杂度

   $O(1)$ 。

#### 5.切换和状态

1. 数据类型

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   切换为和状态之后可以直接读出每个点的值。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/HeavyLightDecompositionLCA_vector.h"
#include "TREE/TreeAdjacentDifference_vector.h"

int main() {
    //一个无权树
    OY::VectorTree T(5);
    //加边
    T.addEdge(2, 0);
    T.addEdge(1, 3);
    T.addEdge(4, 0);
    T.addEdge(0, 3);
    //定根
    T.setRoot(3);
    //预处理
    OY::TreeAdjacentDifference_vector<OY::VectorTree<bool>, int> tad(T);
    //再来一个专门辅助计算 lca 的助手
    OY::HeavyLightDecompositionLCA_vector hld(T);
    //进行路径修改
    tad.addPathValue(1, 4, hld.calc(1, 4), 10);
    tad.addPathValue(2, 4, hld.calc(2, 4), -3);
    //也可以进行子树修改
    tad.addSubtreeValue(1, 2);
    //切换和状态
    tad.partialSum();
    //读出结点值
    cout << "value of 0 = " << tad.m_values[0] << endl;
    cout << "value of 1 = " << tad.m_values[1] << endl;
    cout << "value of 2 = " << tad.m_values[2] << endl;
    cout << "value of 3 = " << tad.m_values[3] << endl;
    cout << "value of 4 = " << tad.m_values[4] << endl;
}
```

```
#输出如下
value of 0 = 7
value of 1 = 12
value of 2 = -3
value of 3 = 10
value of 4 = 7

```

