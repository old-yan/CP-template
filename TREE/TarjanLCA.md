### 一、模板类别

​	数据结构： `Tarjan` 求 `LCA` 。

### 二、模板功能

#### 1.预处理

1. 数据类型

   模板参数 `typename _Tree` ，表示树的类型。

   模板参数 `uint32_t _MAXM` ，表示 `LCA` 的查询次数上限。

   构造参数 `_Tree &__tree`​ ，表示树。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   `Tarjan` 法是唯一的离线求 `LCA` 的方法，即必须预先知道所有的查询问题，再一次性求出所有答案。

#### 2.添加查询

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个结点。

   输入参数 `uint32_t __b` ，表示第二个结点。

2. 时间复杂度

   $O(1)$ ，取决于你选择的 `RMQ` 数据结构。

#### 3.统一求解

1. 数据类型

2. 时间复杂度

   $O(n+m)$ 。
   
3. 备注

   求解之后可以通过 `m_answer` 属性查看所有查询的答案。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/TarjanLCA.h"

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
    OY::TarjanLCA<OY::Tree<10>, 2> tl(T);
    //添加查询问题
    tl.addQuery(2, 4);
    tl.addQuery(1, 4);
    //求解
    tl.calc();
    //查询
    cout << "lca of 2 and 4: " << tl.m_answer[0] << endl;
    cout << "lca of 1 and 4: " << tl.m_answer[1] << endl;
}
```

```
#输出如下
lca of 2 and 4: 0
lca of 1 and 4: 3

```

