### 一、模板类别

​	数据结构：普通 `RMQ` 求 `LCA` 。

### 二、模板功能

  本模板与 `RMQLCA` 模板完全相同。区别仅在于，本模板适用于 `VectorTree` 。

#### 1.预处理

1. 数据类型

   模板参数 `typename _Tree` ，表示树的类型。

   模板参数 `typename _Solver` ，表示 `RMQ` 的数据结构。可以采用 `ST` 表，猫树，线段树，根号树等等。默认为 `ST` 表。

   构造参数 `_Tree &__tree`​ ，表示树。

2. 时间复杂度

   $O(n)\sim O(n\cdot \log n)$ ，取决于你选择的 `RMQ` 数据结构。
   
3. 备注

   一般情况下，如果查询次数较少，可以选择线段树；如果查询次数较多，可以选择 `ST` 表。


#### 2.查询 LCA

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个结点。

   输入参数 `uint32_t __b` ，表示第二个结点。

   返回类型 `uint32_t` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(1)\sim O(\log n)$ ，取决于你选择的 `RMQ` 数据结构。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/RMQLCA_vector.h"

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
    OY::RMQLCA_vector rl(T);
    //查询
    cout << "lca of 2 and 4: " << rl.calc(2, 4) << endl;
}
```

```
#输出如下
lca of 2 and 4: 0

```

