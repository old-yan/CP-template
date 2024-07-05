### 一、模板类别

​	数据结构：`Tarjan` 算法。

​	练习题目：

1. [P3388 【模板】割点（割顶）](https://www.luogu.com.cn/problem/P3388)
2. [P8435 【模板】点双连通分量](https://www.luogu.com.cn/problem/P8435)
3. [T225524 【模板】点双连通分量（v-dcc）](https://www.luogu.com.cn/problem/T225524)
4. [Biconnected Components](https://judge.yosupo.jp/problem/biconnected_components)(https://github.com/yosupo06/library-checker-problems/issues/731)

### 二、模板功能

​	图论模板往往包含一个 `Solver` 和一个 `Graph` 。前者仅仅进行逻辑运算，而不包含图本身的数据；后者保存了图的点、边数据，并提供方便的接口。

​	简单起见，使用者可以只使用 `Graph` 及其接口。

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   构造参数 `size_type edge_cnt` ，表示边数。无向边按一条边计。默认为 `0` 。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Tarjan` 算法处理的问题为无向图找割点问题，以及找点双连通分量问题。

   本数据结构可以接受重边和自环。
   

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

   输入参数 `size_type edge_cnt` ，表示边数。无向边按一条边计。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示边的起点编号。

   输入参数 `size_type b` ，表示边的终点编号。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在无向图中，只需要按一侧方向加边。

#### 4.获取查询器(calc)

1. 数据类型

   模板参数 `bool GetCut` ，表示是否要进行割点相关计算。

   模板参数 `bool GetVBCC` ，表示是否要进行点双连通分量相关计算。

   返回类型 `Solver<GetCut, GetVBCC>` ，表示用来计算和保存割点、点双连通分量的对象。

2. 时间复杂度

   $O(m+n)$ 。

3. 备注

   可以通过返回的对象查询割点、查询点双连通分量。

#### 5.获取点双连通分量(get_vbccs)

1. 数据类型

   返回类型 `std::vector<std::vector<size_type>>` ，表示图中的每个点双连通分量。

2. 时间复杂度

   $O(m+n)$ 。

#### 6.获取割点(get_cuts)

1. 数据类型

   返回类型 `std::vector<size_type>` ，表示图中的每个割点的编号。

2. 时间复杂度

   $O(m+n)$ 。

### 三、模板示例

```c++
#include "GRAPH/Tarjan_cut.h"
#include "IO/FastIO.h"

int main() {
    OY::VBCC::Graph G(6, 5);
    // 加五条边
    G.add_edge(0, 3);
    G.add_edge(0, 5);
    G.add_edge(1, 5);
    G.add_edge(3, 2);
    G.add_edge(5, 3);

    // 单独获取割点
    cout << "get cuts:\n";
    auto cuts = G.get_cuts();
    for (uint32_t i = 0; i < cuts.size(); i++) {
        cout << "No." << i << " cut vertex: index = " << cuts[i] << '\n';
    }
    cout << '\n';

    // 单独获取点双连通分量
    cout << "get vbccs:\n";
    auto vbccs = G.get_vbccs();
    for (uint32_t i = 0; i < vbccs.size(); i++) {
        cout << "No." << i << " group:\n";
        for (auto a : vbccs[i]) cout << a << ' ';
        cout << endl;
    }
    cout << '\n';

    // 如果既要获取割点，也要获取点双
    auto sol = G.calc<true, true>();
    sol.do_for_each_cut([&](uint32_t i) {
        cout << "cut vertex: index = " << i << '\n';
    });
    auto print_vbcc = [&](uint32_t *first, uint32_t *last) {
        cout << "vbcc:";
        for (auto it = first; it != last; ++it) cout << ' ' << *it;
        cout << endl;
    };
    sol.do_for_each_vbcc(print_vbcc);
    cout << '\n';

    /**
     * 一个特别的功能，就是把每个 vbcc 里的边的编号们找出来
     * 每个边一定会被分到且仅被分到一个 vbcc 内部
     * 需要传递两个回调
     * 第一个回调负责处理有边的 vbcc 里的所有的边
     * 第二个回调负责处理没边的 vbcc 里的单独的点（显然没边的情况下 vbcc 里只可能有一个孤立点）
     **/
    uint32_t group_id = 0;
    auto edge_call = [&](uint32_t *first, uint32_t *last) {
        cout << "vbcc group id = " << group_id++ << endl;
        for (auto it = first; it != last; ++it) {
            uint32_t index = *it;
            uint32_t from = G.m_raw_edges[index].m_from, to = G.m_raw_edges[index].m_to;
            cout << "edge index = " << index << ", from " << from << " to " << to << endl;
        }
    };
    auto single_call = [&](uint32_t i) {
        cout << "vbcc group id = " << group_id++ << endl;
        cout << "no edge, single vertex = " << i << endl;
    };
    sol.find_edges(edge_call, single_call, G);
}
```

```
#输出如下
get cuts:
No.0 cut vertex: index = 3
No.1 cut vertex: index = 5

get vbccs:
No.0 group:
3 2 
No.1 group:
5 1 
No.2 group:
0 3 5 
No.3 group:
4 

cut vertex: index = 3
cut vertex: index = 5
vbcc: 3 2
vbcc: 5 1
vbcc: 0 3 5
vbcc: 4

vbcc group id = 0
edge index = 3, from 3 to 2
vbcc group id = 1
edge index = 2, from 1 to 5
vbcc group id = 2
edge index = 0, from 0 to 3
edge index = 4, from 5 to 3
edge index = 1, from 0 to 5
vbcc group id = 3
no edge, single vertex = 4

```

