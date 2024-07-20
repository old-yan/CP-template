### 一、模板类别

​	数据结构：`Tarjan` 算法。

​	练习题目：

1. [P8436 【模板】边双连通分量](https://www.luogu.com.cn/problem/P8436)
2. [T103481 【模板】割边](https://www.luogu.com.cn/problem/T103481)
3. [T103489 【模板】边双连通分量](https://www.luogu.com.cn/problem/T103489)
4. [U119054 【模板】割边/边双连通分量](https://www.luogu.com.cn/problem/U119054)
5. [U134060 【模板】割边（桥）](https://www.luogu.com.cn/problem/U134060)
6. [U143936 割边/桥（模板）](https://www.luogu.com.cn/problem/U143936)
7. [U238155 【模板】割边](https://www.luogu.com.cn/problem/U238155)
8. [图上计数(Hard)](https://ac.nowcoder.com/acm/problem/276004)
9. [Two-Edge-Connected Components](https://judge.yosupo.jp/problem/two_edge_connected_components)(https://github.com/yosupo06/library-checker-problems/issues/366)

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

   `Tarjan` 算法处理的问题为无向图找桥问题，以及找边双连通分量问题。

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

   模板参数 `bool GetBridge` ，表示是否要进行桥边相关计算。

   模板参数 `bool GetEBCC` ，表示是否要进行边双连通分量相关计算。

   返回类型 `Solver<GetBridge, GetEBCC>` ，表示用来计算和保存桥边、边双连通分量的对象。

2. 时间复杂度

   $O(m+n)$ 。

3. 备注

   可以通过返回的对象查询桥边、查询边双连通分量。

#### 5.获取边双连通分量(get_ebccs)

1. 数据类型

   返回类型 `std::vector<std::vector<size_type>>` ，表示图中的每个边双连通分量。

2. 时间复杂度

   $O(m+n)$ 。

#### 6.获取桥边(get_bridges)

1. 数据类型

   返回类型 `std::vector<size_type>` ，表示图中的每个桥边的编号。

2. 时间复杂度

   $O(m+n)$ 。

### 三、模板示例

```c++
#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

int main() {
    OY::EBCC::Graph G(5, 5);
    // 加五条边
    G.add_edge(0, 3);
    G.add_edge(0, 4);
    G.add_edge(1, 4);
    G.add_edge(3, 2);
    G.add_edge(4, 3);

    // 单独获取桥边
    cout << "get bridges:\n";
    auto bridges = G.get_bridges();
    for (uint32_t i = 0; i < bridges.size(); i++) {
        uint32_t from = G.m_raw_edges[bridges[i]].m_from;
        uint32_t to = G.m_raw_edges[bridges[i]].m_to;
        cout << "No." << i << " bridge edge: index = " << bridges[i] << ", from " << from << " to " << to << '\n';
    }
    cout << '\n';

    // 单独获取边双连通分量
    cout << "get ebccs:\n";
    auto ebccs = G.get_ebccs();
    for (uint32_t i = 0; i < ebccs.size(); i++) {
        cout << "No." << i << " group:\n";
        for (auto a : ebccs[i]) cout << a << ' ';
        cout << endl;
    }
    cout << '\n';

    // 如果既要获取桥边，也要获取边双
    auto sol = G.calc<true, true>();
    sol.do_for_each_bridge([&](uint32_t index) {
        uint32_t from = G.m_raw_edges[index].m_from;
        uint32_t to = G.m_raw_edges[index].m_to;
        cout << "bridge edge: index = " << index << ", from " << from << " to " << to << '\n';
    });
    auto print_ebcc = [&](uint32_t *first, uint32_t *last) {
        cout << "ebcc:";
        for (auto it = first; it != last; ++it) cout << ' ' << *it;
        cout << endl;
    };
    sol.do_for_each_ebcc(print_ebcc);
}
```

```
#输出如下
get bridges:
No.0 bridge edge: index = 2, from 1 to 4
No.1 bridge edge: index = 3, from 3 to 2

get ebccs:
No.0 group:
2 
No.1 group:
1 
No.2 group:
0 3 4 

bridge edge: index = 2, from 1 to 4
bridge edge: index = 3, from 3 to 2
ebcc: 2
ebcc: 1
ebcc: 0 3 4

```

