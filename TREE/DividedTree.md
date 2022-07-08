### 一、模板类别

​	数据结构：分治重构树。

### 二、模板功能

#### 1.获取分治重构树

1. 数据类型

   输入参数 `_Tree &__tree`​ ，表示树。

   返回类型 `_Tree<_MAXN>` ，表示重构后的树。

2. 时间复杂度

   $O(n\cdot \log n)$ 。
   
3. 备注

   一棵任意树的高度可能是 $O(n)$ 的，而如果树高为 $O(\log n)$ ，有些暴力做法就可以说得通。通过不断找重心的方法，将原树结构替换为更加匀称的新树，从而实现了树高 $O(\log n)$ 。

   **注意：**生成的新树的边，仅表示分治委派关系，而不代表实际有边相连。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/DividedTree.h"

int main() {
    //一个无权树
    OY::Tree<10> T1(5);
    //加边
    T1.addEdge(0, 1);
    T1.addEdge(1, 2);
    T1.addEdge(2, 3);
    T1.addEdge(3, 4);
    T1.prepare();

    auto T2 = OY::getDivideTree(T1);
    cout << T2.m_root << endl;
    for (uint32_t cur = T2.m_starts[T2.m_root], end = T2.m_starts[T2.m_root + 1]; cur != end; cur++)
        cout << "son :" << T2.m_to[cur] << endl;
}
```

```
#输出如下
2
son :1
son :3

```

