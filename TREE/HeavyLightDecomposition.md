### 一、模板类别

​	数据结构：轻重链（树链剖分，重链剖分）。

​	练习题目：

1. [P2495 [SDOI2011] 消耗战](https://www.luogu.com.cn/problem/P2495)
2. [P3292 [SCOI2016] 幸运数字](https://www.luogu.com.cn/problem/P3292)
3. [P3379 【模板】最近公共祖先（LCA）](https://www.luogu.com.cn/problem/P3379)
4. [P3979 遥远的国度](https://www.luogu.com.cn/problem/P3979)
5. [P4069 [SDOI2016] 游戏](https://www.luogu.com.cn/problem/P4069)
6. [P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)
7. [P4103 [HEOI2014] 大工程](https://www.luogu.com.cn/problem/P4103)
8. [P4556 [Vani有约会] 雨天的尾巴 /【模板】线段树合并](https://www.luogu.com.cn/problem/P4556)
9. [P5903 【模板】树上 K 级祖先](https://www.luogu.com.cn/problem/P5903)
10. [记忆](https://ac.nowcoder.com/acm/problem/274793)
11. [Vertex Add Path Sum](https://judge.yosupo.jp/problem/vertex_add_path_sum)(https://github.com/yosupo06/library-checker-problems/issues/125)
12. [Vertex Set Path Composite](https://judge.yosupo.jp/problem/vertex_set_path_composite)(https://github.com/yosupo06/library-checker-problems/issues/190)
13. [Jump on Tree](https://judge.yosupo.jp/problem/jump_on_tree)(https://github.com/yosupo06/library-checker-problems/issues/809)


### 二、模板功能

#### 1.构造

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   模板参数 `typename Tree` ，表示树的类型。

   构造参数 `Tree *rooted_tree`​ ，表示树，需要指定根。默认为 `nullptr` 。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   树链剖分指的是，将整个树分割成若干条自上而下的链，且该链在树的遍历序列里连续。当要对树上的某个路径进行处理时，可以将路径分割成若干条链，也就是树的遍历序列上的若干个区间，而在一维序列上进行区间操作，难度就降低很多。

   显然，一个子树对应的序列元素也是连续的。

   `m_info[a].m_top_dfn` 属性表示结点 `a` 所在的链的最高位的结点的 `dfs` 序；

   `m_info[a].m_top_dep` 属性表示结点 `a` 所在的链的最高位的结点的深度；

   `m_info[a].m_parent` 属性表示结点 `a` 所在的链的父结点编号；

   `m_info[a].m_dfn` 属性表示结点 `a` 的 `dfs` 序；

   `m_info[a].m_dep` 属性表示结点 `a` 的深度，根的深度为 `0` ；

   `m_info[a].m_size` 属性表示结点 `a` 为根的子树的大小；

   `m_info[a].m_heavy` 属性表示结点 `a` 的重儿子的编号，叶子结点的重儿子为 `-1` 。

    `m_seq` 表示所有结点按照 `dfs` 序形成的序列。

#### 2.重置(reset)

1. 数据类型

   输入参数 `Tree *rooted_tree` ，表示树，需要指定根。

2. 时间复杂度

   $O(n)$ 。

#### 3.获取某结点的祖先(get_ancestor)

1. 数据类型

   输入参数 `size_type a` ，表示结点编号。

   输入参数 `size_type n` ，表示要查询 `n` 级祖先。
   
   返回类型 `size_type` ，表示祖先编号。当祖先不存在时返回 `-1` 。

2. 时间复杂度

   $O(\log n)$ 。

#### 4.查询某结点的父结点(find_parent)

1. 数据类型

   输入参数 `size_type a` ，表示结点编号。

   返回类型 `size_type` ，表示父结点编号。当父结点不存在时返回 `-1` 。

2. 时间复杂度

   $O(1)$ 。

#### 5.查询某结点在某子孙方向上的子结点(find_son)

1. 数据类型

   输入参数 `size_type a` ，表示结点编号。

   输入参数 `size_type b` ，表示结点 `a` 的某个子孙结点的编号。

   返回类型 `size_type` ，表示此方向的子结点编号。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法要求 `a` 结点非叶子结点， `b` 结点必须为 `a` 结点的子孙。所以答案必然存在。

#### 6.查询结点深度(get_depth)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   返回类型 `size_type` ，表示结点的深度。

2. 时间复杂度

   $O(1)$ 。

#### 7.对某结点调用回调(do_for_vertex)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `Callback &&call` ，表示要执行的回调。
   
   返回类型等同于回调函数的返回类型，即 `call` 的返回值。

2. 时间复杂度

   $O(1)$ 。


#### 8.对路径上的结点形成的序列区间依次调用回调(do_for_path)

1. 数据类型

   模板参数 `bool LCA` ，表示是否包含路径最高点。

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

   输入参数 `Callback &&call` ，表示要执行的回调。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本方法最多可能找到对数级别的区间。有人常常困惑树链剖分的时间复杂度到底是一个还是两个 `log` ，实际上，树链剖分会找到总量为 `log` 级别的区间们，复杂度为一个 `log` ；但是如果对找到的区间又进行了进一步的操作，那总的复杂度就另当别论。比如，每找出一个区间，就在线段树上查询这个区间的聚合值，那总复杂度就是两个 `log` 。每找出一个区间，就在 `ST` 表上查询这个区间的聚合值，那总复杂度就是一个 `log` 。所以，不说清楚对找到的区间要干啥，就稀里糊涂讨论复杂度，说明没理解这整个过程。如果硬要说复杂度，那就是一个 `log` ，这指的是单纯找出所有区间的时间复杂度，而不包括对找出的区间们进行进一步操作的时间复杂度。

   **注意：**

   在有些场景下，人们把结点 `a` 到其父结点 `p` 之间的边权赋到 `a` 身上，进而对各条边做后续修改/查询。在这种场景下，假如某路径含有 `5` 个点，里面只有 `4` 条边的信息，因为最高点的边权是不算在这个路径里的，在这种情况下模板参数 `LCA` 取 `false` 。

#### 9.对路径上的结点形成的有序区间调用回调(do_for_directed_path)

1. 数据类型

   输入参数 `size_type from` ，表示起点的编号。

   输入参数 `size_type to` ，表示终点的编号。

   输入参数 `Callback &&call` ，表示要执行的回调。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本方法最多可能找到对数级别的区间。将每个区间的对应结点依次链接在一起，恰好为从结点 `from` 到 `to` 的路径结点序列。
   
   在树链剖分里，同一条链上，结点从高到低，在区间内的编号依次增大。而显然，路径上的结点有时会从低到高爬升。此时，找到的区间左端点大于右端点。

#### 10.对子树序列区间调用回调(do_for_subtree)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `Callback &&call` ，表示要执行的回调。
   
   返回类型等同于回调函数的返回类型，即 `call` 的返回值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法只会找到一个区间，因为一个子树的 `dfs` 序必然是连续的。

#### 11.查询LCA(calc)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

   返回类型 `size_type` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(\log n)$ 。
   

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"

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

    OY::HLD::Table<decltype(T)> hld(&T);
    // 查看每个点的 dfs 序
    for (int i = 0; i < 9; i++) cout << hld.m_info[i].m_dfn << " \n"[i == 8];
    // 查看所有点按 dfs 序形成的序列
    for (int i = 0; i < 9; i++) cout << hld.m_seq[i] << " \n"[i == 8];

    // 查看 5 和 8 之间的路径所占的区间
    std::vector<std::pair<int, int>> ranges;
    hld.do_for_path<true>(5, 8, [&](int l, int r) {
        ranges.emplace_back(l, r);
    });
    cout << "from 5 to 8:\n";
    // 观察可以看到，找到的区间就是 5 到 8 的结点们
    for (auto &range : ranges) {
        for (int i = range.first; i <= range.second; i++) {
            cout << hld.m_seq[i] << " ";
        }
    }
    cout << endl;

    std::pair<int, int> subtree_range;
    hld.do_for_subtree(7, [&](int l, int r) { subtree_range = {l, r}; });
    cout << "subtree(7):\n";
    for (int i = subtree_range.first; i <= subtree_range.second; i++) {
        cout << hld.m_seq[i] << " ";
    }
}
```

```
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
4 1 2 0 5 7 8 6 3
3 1 2 8 0 4 7 5 6
from 5 to 8:
7 5 3 1 2 8 
subtree(7):
7 5 6 

```

