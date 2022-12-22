### 一、模板类别

​	数据结构：`Tarjan` 算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp`​ ，表示图中的边权。默认为 `bool` 类型。
   
   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Tarjan` 算法处理的问题为无向图找割点问题，以及找点双连通分量问题。

   本数据结构可以接受重边和自环。
   
   **注意：**

   模板参数的 `_Tp` 类型参数，在找割点、找点双连通分量的问题中并不重要。
   
   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示无向边的起点编号。

   输入参数 `uint32_t __b` ，表示无向边的终点编号。

2. 时间复杂度

   $O(1)$ 。


#### 3.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示无向边的起点编号。

   输入参数 `uint32_t __b` ，表示无向边的终点编号。

   输入参数 `_Tp __distance` ，表示无向边的边权。

2. 时间复杂度

   $O(1)$ 。


#### 4.运行主算法

1. 数据类型

   模板参数 `bool _GetVbcc` ，表示是否求出点双连通分量。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   在调用本方法前，请先预备。

   本方法对所有的点进行标注其相邻的组分数，如果一个点相邻的组分数大于 `1` ，那么将其去掉后，整个图的组分数增加，其为割点。

   是否求点双连通分量不会改变时间复杂度，只会影响常数，求出的点双连通分量保存到 `m_vbcc` 属性里。

   **注意：**

   一个孤立点是一个点双连通分量。

   一个点可以同时包含在多个点双连通分量里。

#### 5.获取所有点双连通分量里的边

1. 数据类型

   返回类型 `std::vector<std::basic_string_view<uint32_t>>` ，表示每个点双连通分量里的边的初始编号。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   在调用本方法前，请先运行主算法。当然，运行主算法时并不需要求出点双连通分量。

   本方法获取每个点双连通分量里的边的编号。

   **注意：**

   一个边，必然在且仅在一个点双连通分量里。

   有的点双连通分量里，没有边。这是因为这个分量只包含一个孤立点。

#### 6.获取仙人掌图的每个圈的环长，以及每个点的去重 dfs 距离

1. 数据类型

   返回类型 `std::pair<std::vector<distance_type>, std::vector<distance_type>>` ，前者表示每个点双连通子图的环长，后者表示 `dfs` 只走树边的情况下每个点的距离。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   在调用本方法前，请先运行主算法。当然，运行主算法时并不需要求出点双连通分量。
   
   本方法仅适用于仙人掌图。

   仙人掌图为任意一条边至多只出现在一条简单回路的无向连通图。

   在仙人掌图里，每个点双连通分量，要么为一个简单环，要么为一条边连接的两个点，要么为一个孤立点。

   返回值的 `first` 长度等于点双连通子图的数量，分别表示每个子图的环长（若子图不成环则为零）。

   返回值的 `second` 长度等于点的数量，分别表示每个点在去重 `dfs` 过程中的距离。
   
   显然，结合环长和距离差，可以方便地求出同一个点双连通分量里，两个结点之间的最短路长：要么为距离差，要么为环长减距离差。

#### 7.判断是否是割点

1. 数据类型

   输入参数 `uint32_t __i` ，表示图中某点的编号。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在调用本方法之前请先运行主算法。

#### 8.获取所有割点

1. 数据类型

   返回类型 `std::vector<uint32_t>` ，表示所有割点的编号。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前请先运行主算法。

### 三、模板示例

```c++
#include "GRAPH/Tarjan_cut.h"
#include "IO/FastIO.h"

int main() {
    OY::Tarjan_cut G(6, 5);
    // 加五条边
    G.addEdge(0, 3);
    G.addEdge(0, 5);
    G.addEdge(1, 5);
    G.addEdge(3, 2);
    G.addEdge(5, 3);
    // 运行主算法
    G.calc<true>();
    for (uint32_t i = 0; i < 6; i++) {
        cout << "No." << i << " vertex " << (G.isCut(i) ? "is" : "isn't") << " cut\n";
    }
    // 主算法求出了每个 vbcc 里的点
    auto &&groups = G.m_vbcc;
    for (uint32_t i = 0; i < groups.size(); i++) {
        cout << "No." << i << " group's vertexes:\n";
        for (auto a : groups[i]) cout << a << ' ';
        cout << endl;
    }
    // 还可以求每个 vbcc 里的边
    auto edge_group = G.getEdgeVBCC();
    for (uint32_t i = 0; i < edge_group.size(); i++) {
        cout << "No." << i << " group's edges:\n";
        if (edge_group[i].empty())
            cout << "single vertex, no edge\n";
        else
            for (auto index : edge_group[i]) {
                auto &[from, to] = G.m_rawEdges[index];
                cout << from << " -> " << to << '\n';
            }
    }
}
```

```
#输出如下
No.0 vertex isn't cut
No.1 vertex isn't cut
No.2 vertex isn't cut
No.3 vertex is cut
No.4 vertex isn't cut
No.5 vertex is cut
No.0 group's vertexes:
3 2 
No.1 group's vertexes:
5 1 
No.2 group's vertexes:
0 3 5 
No.3 group's vertexes:
4 
No.0 group's edges:
3 -> 2
No.1 group's edges:
1 -> 5
No.2 group's edges:
0 -> 3
5 -> 3
0 -> 5
No.3 group's edges:
single vertex, no edge

```

