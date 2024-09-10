### 一、模板类别

​	数据结构：虚树。

​	练习题目：

1. [P2495 [SDOI2011] 消耗战](https://www.luogu.com.cn/problem/P2495)
2. [P4103 [HEOI2014] 大工程](https://www.luogu.com.cn/problem/P4103)

### 二、模板功能

​	本模板实际上并非是一个数据结构，而是一个函数。

#### 1.建立虚树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   模板参数 `size_type MAX_BUFFER` ，表示缓冲池的最大长度。一般是对每棵树的结点数取最大值即可。

   输入参数 `Iterator first` 及 `Iterator last` ，表示建立虚树必须包含的关键结点。

   输入参数 `DFNGetter &&dfn_getter` ，表示获取树上结点 `dfs` 序的函数。

   输入参数 `LCAGetter &&lca_getter` ，表示获取两结点的最近公共祖先的函数。

   输入参数 `Callback &&call` ，表示找到两个虚树中相连的结点时，调用的回调函数。

2. 时间复杂度

   $O(m\sim m\log n)$ ，此处 `m` 指关键结点的数量， `n` 指树的大小。具体复杂度主要取决于最近公共祖先的求法。
   
3. 备注

   虚树，是指指定一些关键结点，再在原树的基础上，取一些”关节部位“的结点，不改变结点之间的祖孙关系，将关键结点串烧起来形成的新树。

   建立虚树与建立点分树具有相似性，往往只是需要在虚树/点分树的结构上进行一个 `dp` ，在构建好虚树/点分树的同时就可以完成 `dp` ，而不需要真正地把虚树/点分树建立出来。所以本模板提供了回调函数以便在构建虚树树的同时进行 `dp` 转移。

   当然，如果确有真正虚树树的需求，完全可以在回调中获取到所有的边，进而构建出一棵虚树。
   
   本模板的常用用法为，配合重链剖分或者 `RMQLCA` 实现 `dfs` 序和最近公共祖先的查询，在回调中进行 `dp` 转移。回调函数的入参有两个参数，分别为虚树某条边的下位结点 `a` 和其父结点 `p` ，显然在原树中， `p` 并不一定为 `a` 的父结点，但一定为 `a` 的祖先结点。本模板保证在调用回调时，虚树中 `a` 的子树已经构建完毕。

### 三、模板示例

```c++
#include "DS/SparseTable.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VirtualTree.h"

int main() {
    // 虚树主要用法技巧在 oj 测试中体现，此处只找出所有虚树的边
    OY::FlatTree::Tree<bool, 1000> T(7);
    T.add_edge(3, 1);
    T.add_edge(1, 0);
    T.add_edge(1, 2);
    T.add_edge(3, 5);
    T.add_edge(5, 4);
    T.add_edge(5, 6);
    T.prepare();
    T.set_root(3);
    cout << T << endl;

    // 假如我们希望虚树中包含 0 2 4 这三个结点
    int keys[] = {0, 2, 4};

    // 借助 RMQLCA 查询 dfs 序和 lca
    OY::RMQLCA::Table<decltype(T), OY::STMinTable<uint32_t>> rmq_lca(&T);
    std::vector<std::pair<int, int>> edges;
    OY::VTREE::VirtualTree<1000>::solve(
        keys, keys + 3, [&](int a) { return rmq_lca.m_dfn[a]; }, [&](int a, int b) { return rmq_lca.calc(a, b); }, [&](int a, int b) { edges.emplace_back(a, b); });
    cout << "in virtual tree:\n";
    for (auto &e : edges) {
        cout << e.first << " is connected with " << e.second << endl;
    }
}
```

```
#输出如下
[3[1[0][2]][5[4][6]]]
in virtual tree:
0 is connected with 1
2 is connected with 1
1 is connected with 3
4 is connected with 3

```