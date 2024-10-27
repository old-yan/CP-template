### 一、模板类别

​	数据结构： `bfn` 序控制器。

​	练习题目：

1. [P3379 【模板】最近公共祖先（LCA）](https://www.luogu.com.cn/problem/P3379)
2. [P5903 【模板】树上 K 级祖先](https://www.luogu.com.cn/problem/P5903)
3. [小H和游戏](https://ac.nowcoder.com/acm/problem/15158)



### 二、模板功能

#### 1.构造

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   模板参数 `typename Tree` ，表示树的类型。

   构造参数 `Tree *rooted_tree`​ ，表示树，需要指定根。默认为 `nullptr` 。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   本模板按照 `bfs` 顺序将树中的每个点铺放到一个一维的序列里，从而实现把对某一层的连续结点的查询和修改转化为一维序列的区间查询和区间修改。

   显然，一个结点的所有孩子结点对应的序列元素是连续的。

   `m_info[a].m_bfn` 属性表示结点 `a` 的 `bfs` 序；
   
   `m_info[a].m_dfn` 属性表示结点 `a` 的 `dfs` 序（后序遍历）；

   `m_info[a].m_dep` 属性表示结点 `a` 的深度；

    `m_bfs_seq` 表示所有结点按照 `bfs` 顺序形成的结点编号序列；
    
    `m_bfs_dfn_seq` 表示所有结点的 `dfn` 按照 `bfs` 顺序形成的 `dfn` 序列；

#### 2.重置(reset)

1. 数据类型

   输入参数 `Tree *rooted_tree` ，表示树，需要指定根。

2. 时间复杂度

   $O(n)$ 。

#### 3.对某结点调用回调(do_for_vertex)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `BfnCallback &&call` ，表示要执行的回调。
   
   返回类型等同于回调函数的返回类型，即 `call` 的返回值。

2. 时间复杂度

   $O(1)$ 。


#### 4.对后代序列区间调用回调(do_for_descendants)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。
   
   输入参数 `size_type n` ，表示后代层数。

   输入参数 `BfnCallback &&call` ，表示要执行的回调。
   
   返回类型等同于回调函数的返回类型，即 `call` 的返回值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法只会找到一个区间，因为某一层后代的 `bfs` 序必然是连续的。
   
   本方法要求 `n` 非负， `n` 为零时，会找到结点 `a` 本身； `n` 为 `1` 时，会找到结点 `a` 的所有直接孩子； `n` 为 `2` 时，会找到结点 `a` 的所有二代孙子；依此类推。
   
   当 `n` 过大时，没找到相应的子孙， `call(l, r)` 的 `l` 和 `r` 满足 `l==r+1` ，表示空区间。
   
#### 5.查询结点深度(get_depth)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   返回类型 `size_type` ，表示结点的深度。

2. 时间复杂度

   $O(1)$ 。
   
#### 6.获取某结点的祖先(get_ancestor)

1. 数据类型

   输入参数 `size_type a` ，表示结点编号。

   输入参数 `size_type n` ，表示要查询 `n` 级祖先。
   
   返回类型 `size_type` ，表示祖先编号。当祖先不存在时返回 `-1` 。

2. 时间复杂度

   $O(\log n)$ 。
   
#### 7.查询LCA(calc)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

   返回类型 `size_type` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(\log^2 n)$ 。   
   
   
### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/BfnController.h"
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

    OY::BFN::Table<decltype(T)> bfn(&T);
    // 查看每个点的 bfs 序
    for (int i = 0; i < 9; i++) cout << bfn.m_info[i].m_bfn << " \n"[i == 8];
    // 查看所有点按 bfs 序形成的序列
    for (int i = 0; i < 9; i++) cout << bfn.m_seq[i] << " \n"[i == 8];

    // 查询  1 的 二代孩子
    bfn.do_for_descendants(1, 2, [&](int l, int r) {
        for (int i = l; i <= r; i++) cout << bfn.m_seq[i] << ",\n"[i == r];
    });
    // 查询  7 的 一代孩子
    bfn.do_for_descendants(7, 1, [&](int l, int r) {
        for (int i = l; i <= r; i++) cout << bfn.m_seq[i] << ",\n"[i == r];
    });
    // 查询  3 的 一代孩子
    bfn.do_for_descendants(3, 1, [&](int l, int r) {
        for (int i = l; i <= r; i++) cout << bfn.m_seq[i] << ",\n"[i == r];
    });
    // 查询  3 的 二代孩子
    bfn.do_for_descendants(3, 2, [&](int l, int r) {
        for (int i = l; i <= r; i++) cout << bfn.m_seq[i] << ",\n"[i == r];
    });
}
```

```
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
4 1 5 0 2 6 7 3 8
3 1 4 7 0 2 5 6 8
8
5,6
1,4,7
0,2,5,6

```
