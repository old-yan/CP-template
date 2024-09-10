### 一、模板类别

​	数据结构： `RMQLCA` 。

​	练习题目：

1. [P2495 [SDOI2011] 消耗战](https://www.luogu.com.cn/problem/P2495)
2. [P3128 [USACO15DEC] Max Flow P](https://www.luogu.com.cn/problem/P3128)
3. [P3258 [JLOI2014] 松鼠的新家](https://www.luogu.com.cn/problem/P3258)
4. [P3379 【模板】最近公共祖先（LCA）](https://www.luogu.com.cn/problem/P3379)
5. [P4103 [HEOI2014] 大工程](https://www.luogu.com.cn/problem/P4103)
6. [P4556 [Vani有约会] 雨天的尾巴 /【模板】线段树合并](https://www.luogu.com.cn/problem/P4556)
7. [Lowest Common Ancestor](https://judge.yosupo.jp/problem/lca)(https://github.com/yosupo06/library-checker-problems/issues/35)

### 二、模板功能

#### 1.预处理

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号类型。

   模板参数 `typename Tree` ，表示树的类型。

   模板参数 `typename RMQ` ，表示赖以查询 `RMQ` 的数据结构模板。

   构造参数 `Tree *rooted_tree`​ ，表示要处理的树，需要指定根，默认为 `nullptr`。

2. 时间复杂度

   $O(n\sim n\cdot \log n)$ ，取决于赖以查询 `RMQ` 的模板。若为 `ST` 表或者猫树，则为 $O(n\log n)$ ；若为线段树或者 `MaskRMQ` ，则为 $O(n)$ 。

#### 2.重置(reset)

1. 数据类型

   输入参数 `Tree *rooted_tree`​ ，表示要处理的树，需要指定根。

2. 时间复杂度

   $O(n\sim n\cdot \log n)$ ，取决于赖以查询 `RMQ` 的模板。若为 `ST` 表或者猫树，则为 $O(n\log n)$ ；若为线段树或者 `MaskRMQ` ，则为 $O(n)$ 。

#### 3.查询最近公共祖先(calc)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

   返回类型 `size_type` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(1\sim \log n)$ ，取决于赖以查询 `RMQ` 的模板。若为 `ST` 表、猫树或者 `MaskRMQ` ，则为 $O(1)$ ；若为线段树，则为 $O(\log n)$ 。

### 三、模板示例

```c++
#include "DS/MaskRMQ.h"
#include "DS/SparseTable.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/RMQLCA.h"

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
    OY::RMQLCA::Table<decltype(T), OY::STMinTable<uint32_t>> LCA(&T);
    // 查询 LCA
    cout << "lca of 2 and 4: " << LCA.calc(2, 4) << endl;
    cout << "lca of 1 and 4: " << LCA.calc(1, 4) << endl;

    // 换个 MaskRMQ
    OY::RMQLCA::Table<decltype(T), OY::MaskRMQMinValueTable<uint32_t>> LCA2(&T);
    // 查询 LCA
    cout << "lca of 2 and 4: " << LCA2.calc(2, 4) << endl;
    cout << "lca of 1 and 4: " << LCA2.calc(1, 4) << endl;
}
```

```
#输出如下
[3[1][0[2][4]]]
lca of 2 and 4: 0
lca of 1 and 4: 3
lca of 2 and 4: 0
lca of 1 and 4: 3

```

