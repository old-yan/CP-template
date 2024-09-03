### 一、模板类别

​	数据结构： `dfn` 序控制器。

​	练习题目：

1. [2003. 每棵子树内缺失的最小基因值](https://leetcode.cn/problems/smallest-missing-genetic-value-in-each-subtree/)
2. [Vertex Add Subtree Sum](https://judge.yosupo.jp/problem/vertex_add_subtree_sum)(https://github.com/yosupo06/library-checker-problems/issues/167)


### 二、模板功能

#### 1.构造

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   模板参数 `typename Tree` ，表示树的类型。

   构造参数 `Tree *rooted_tree`​ ，表示树，需要指定根。默认为 `nullptr` 。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   本模板按照 `dfs` 顺序将树中的每个点铺放到一个一维的序列里，从而实现把对子树的查询和修改转化为一维序列的区间查询和区间修改。

   显然，一个子树对应的序列元素也是连续的。

   `m_info[a].m_dfn` 属性表示结点 `a` 的 `dfs` 序；

   `m_info[a].m_size` 属性表示结点 `a` 为根的子树的大小；

    `m_seq` 表示所有结点按照 `dfs` 顺序形成的结点编号序列。

#### 2.重置(reset)

1. 数据类型

   输入参数 `Tree *rooted_tree` ，表示树，需要指定根。

2. 时间复杂度

   $O(n)$ 。

#### 3.对某结点调用回调(do_for_vertex)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `Callback &&call` ，表示要执行的回调。
   
   返回类型等同于回调函数的返回类型，即 `call` 的返回值。

2. 时间复杂度

   $O(1)$ 。


#### 4.对子树序列区间调用回调(do_for_subtree)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `Callback &&call` ，表示要执行的回调。
   
   返回类型等同于回调函数的返回类型，即 `call` 的返回值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法只会找到一个区间，因为一个子树的 `dfs` 序必然是连续的。
   

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/DfnController.h"
#include "TREE/FlatTree.h"

int main() {
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> T(9);
    // 加边
    T.add_edge(3, 1);
    T.add_edge(1, 0);
    T.add_edge(1, 2);
    T.add_edge(2, 8);
    T.add_edge(3, 4);
    T.add_edge(3, 7);
    T.add_edge(7, 5);
    T.add_edge(7, 6);
    T.prepare();
    T.set_root(3);
    cout << T << endl;

    OY::DFN::Table<decltype(T)> dfn(&T);
    // 查看每个点的欧拉序
    for (int i = 0; i < 9; i++) cout << dfn.m_info[i].m_dfn << " \n"[i == 8];
    // 查看所有点按欧拉序形成的序列
    for (int i = 0; i < 9; i++) cout << dfn.m_seq[i] << " \n"[i == 8];

    std::pair<int, int> subtree_range;
    dfn.do_for_subtree(7, [&](int l, int r) { subtree_range = {l, r}; });
    cout << "subtree(7):\n";
    for (int i = subtree_range.first; i <= subtree_range.second; i++) {
        cout << dfn.m_seq[i] << " ";
    }
}
```

```
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
2 1 3 0 5 7 8 6 4
3 1 0 2 8 4 7 5 6
subtree(7):
7 5 6 

```
