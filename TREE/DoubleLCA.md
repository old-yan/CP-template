### 一、模板类别

​	数据结构：倍增法求 `LCA` 。

### 二、模板功能

#### 1.预处理

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号类型。

   模板参数 `typename Tree` ，表示树的类型。

   模板参数 `size_type MAX_VERTEX` ，表示树中最大结点数。

   模板参数 `size_type MAX_NODE` ，表示倍增模板最多使用的结点数。

   构造参数 `Tree &rooted_tree`​ ，表示要处理的树，需要指定根，默认为 `nullptr`。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

#### 2.重置(reset)

1. 数据类型

   输入参数 `Tree &rooted_tree`​ ，表示要处理的树，需要指定根。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

#### 3.查询某结点的祖先(get_ancestor)

1. 数据类型

   输入参数 `size_type a` ，表示结点编号。

   输入参数 `size_type n` ，表示要查询 `n` 级祖先。
   
   返回类型 `size_type` ，表示祖先编号。当祖先不存在时返回 `-1` 。

2. 时间复杂度

   $O(\log n)$ 。

#### 4.查询最近公共祖先(calc)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

   返回类型 `size_type` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(\log n)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/DoubleLCA.h"
#include "TREE/FlatTree.h"

int main() {
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> T(5);
    // 加边
    T.add_edge(2, 0);
    T.add_edge(1, 3);
    T.add_edge(4, 0);
    T.add_edge(0, 3);
    T.prepare();
    T.set_root(3);
    cout << T << endl;
    // LCA 预处理
    OY::DoubleLCA::Table<decltype(T), 1000, 1000 << 10> LCA(&T);
    // 查询 祖先
    cout << "father of father of 4: " << LCA.get_ancestor(4, 2) << endl;
    // 查询 LCA
    cout << "lca of 2 and 4: " << LCA.calc(2, 4) << endl;
}
```

```
#输出如下
[3[1][0[2][4]]]
father of father of 4: 3
lca of 2 and 4: 0

```
