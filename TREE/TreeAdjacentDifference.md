### 一、模板类别

​	数据结构： 树上差分。

### 二、模板功能

#### 1.预处理

1. 数据类型

   模板参数 `typename _Tree` ，表示树的类型。

   模板参数 `typename _Tp` ，表示树上结点的值的类型。

   模板参数 `typename _Solver` ，表示求最近公共祖先的数据结构。默认为 `heavyLightDecompositionLCA` 。

   构造参数 `_Tree &__tree`​ ，表示树。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   树上差分是离线方法，即只能将所有的路径修改一次性完成后，才能看到每个结点的当前值。
   
   在修改时，保持差分态；修改后调用求和方法，呈现和状态，只有和状态下可以直接读出每个点的值。差分态和和状态之间的切换时间为线性。

#### 2.在某点上增加值

1. 数据类型

   输入参数 `uint32_t __a` ，表示结点编号。

   输入参数 `_Tp __inc` ，表示在点上增加的值。

2. 时间复杂度

   $O(1)$ 。

#### 3.在某条路径上增加值

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个结点。

   输入参数 `uint32_t __b` ，表示第二个结点。

   输入参数 `_Tp __inc` ，表示在两个点的路径上增加的值。

2. 时间复杂度

   $O(1)$ 。

#### 4.切换和状态

1. 数据类型

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   切换为和状态之后可以直接读出每个点的值。

#### 5.切换差分态

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   切换为差分状态之后可以进行路径修改操作，但不能实时读出结点的值。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/TreeAdjacentDifference.h"

int main() {
    //一个无权树
    OY::Tree<10> T(5);
    //加边
    T.addEdge(2, 0);
    T.addEdge(1, 3);
    T.addEdge(4, 0);
    T.addEdge(0, 3);
    T.prepare();
    //定根
    T.setRoot(3);
    //预处理
    OY::TreeAdjacentDifference<OY::Tree<10>, int> tad(T);
    //进行路径修改
    tad.addPathValue(1, 4, 10);
    tad.addPathValue(2, 4, -3);
    //切换和状态
    tad.partialSum();
    //读出结点值
    cout << "value of 0 = " << tad.m_dif[0] << endl;
    cout << "value of 3 = " << tad.m_dif[3] << endl;
    cout << "value of 2 = " << tad.m_dif[2] << endl;
}
```

```
#输出如下
value of 0 = 7
value of 3 = 10
value of 2 = -3

```

