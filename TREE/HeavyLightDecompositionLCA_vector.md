### 一、模板类别

​	数据结构：轻重链求 `LCA` 。

### 二、模板功能

  本模板与 `HeavyLightDecompositionLCA` 模板完全相同。区别仅在于，本模板适用于 `VectorTree` 。

#### 1.预处理

1. 数据类型

   模板参数 `typename _Tree` ，表示树的类型。

   构造参数 `_Tree &__tree`​ ，表示树。

2. 时间复杂度

   $O(n)$ 。


#### 2.查询 LCA

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个结点。

   输入参数 `uint32_t __b` ，表示第二个结点。

   返回类型 `uint32_t` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(\log n)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/HeavyLightDecompositionLCA_vector.h"

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
    // LCA 预处理
    OY::HeavyLightDecompositionLCA_vector hld(T);
    //查询
    cout << "lca of 2 and 4: " << hld.calc(2, 4) << endl;
}
```

```
#输出如下
lca of 2 and 4: 0

```

