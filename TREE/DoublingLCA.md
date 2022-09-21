### 一、模板类别

​	数据结构：倍增法求 `LCA` 。

### 二、模板功能

#### 1.预处理

1. 数据类型

   模板参数 `typename _Tree` ，表示树的类型。

   构造参数 `_Tree &__tree`​ ，表示树。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

#### 2.获取某结点的祖先

1. 数据类型

   输入参数 `uint32_t __a` ，表示结点编号。

   输入参数 `uint32_t __n` ，表示要查询 `__n` 级祖先。
   
   返回类型 `uint32_t` ，表示祖先编号。当祖先不存在时返回 `-1` 。

2. 时间复杂度

   $O(\log n)$ 。

#### 3.查询 LCA

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个结点。

   输入参数 `uint32_t __b` ，表示第二个结点。

   返回类型 `uint32_t` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(\log n)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/DoublingLCA.h"

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
    // LCA 预处理
    OY::DoublingLCA dl(T);
    //查询
    cout << "lca of 2 and 4: " << dl.calc(2, 4) << endl;
}
```

```
#输出如下
lca of 2 and 4: 0

```

