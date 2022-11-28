### 一、模板类别

​	数据结构：长短链求 `LCA` 。

### 二、模板功能

  本模板与 `LongShortDecompositionLCA` 模板完全相同。区别仅在于，本模板适用于 `VectorTree` 。

#### 1.预处理

1. 数据类型

   模板参数 `typename _Tree` ，表示树的类型。

   构造参数 `_Tree &__tree`​ ，表示树。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

#### 2.查询祖先

1. 数据类型

   输入参数 `uint32_t __a` ，表示结点编号。

   输入参数 `uint32_t __n` ，表示要查询的祖先代数。

   返回类型 `uint32_t` ，表示 `__a` 的 `__n` 级祖先结点的编号。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法不会对参数进行检查，所以请自行确保参数合法。（`__n` 不要超过 `__a` 的深度）。

#### 3.查询 LCA

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个结点。

   输入参数 `uint32_t __b` ，表示第二个结点。

   返回类型 `uint32_t` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法在查询祖先方法的基础上，利用二分确定祖先代数，进而确定最近公共祖先。

   实践中本方法常数极大，如果没有特殊需求不推荐使用本模板进行 `LCA` 的计算。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/LongShortDecompositionLCA_vector.h"

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
    OY::LongShortDecompositionLCA_vector lsd(T);
    //查询
    cout << "lca of 2 and 4: " << lsd.calc(2, 4) << endl;
}
```

```
#输出如下
lca of 2 and 4: 0

```

