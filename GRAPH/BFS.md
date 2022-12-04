### 一、模板类别

​	数据结构：`BFS` 算法的实现。

### 二、模板功能

#### 1.构造图

1. 数据类型

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

   构造参数 `uint32_t __infiniteDistance` ，表示无穷大边权值。默认为 `uint32_t` 类的最大值的一半。
   
2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `BFS` 算法处理的问题为有向图的最短路问题。

   如果图为无向图，需要将原图的边视为正反两个方向的有向边来构建。

   本数据结构可以接受重边和自环。
   
   **注意：**
   
   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。对于无向图，本参数须为图的边数的二倍。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示有向边的起点编号。

   输入参数 `uint32_t __b` ，表示有向边的终点编号。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：**在无向图中，需要按正反两个反向进行加边。

#### 3.预备

1. 数据类型

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法用于在加完所有边后，进行一些预处理。

#### 4.设置初始距离

1. 数据类型

   输入参数 `uint32_t __i` ，表示要设置初始距离的点编号。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   本方法须在计算最短路方法之前调用，一般将源点的距离设为 `0` 。

#### 5.计算最短路

1. 数据类型

   模板参数 `bool _GetPath` ，表示是否记录最短路的具体路径。

2. 时间复杂度

   $O(m)$ 。

3. 备注

   在调用本方法之前请先设置好源点的初始距离。

   在调用本方法之前请先预备。


#### 6.获取最短路路径（按边获取）

1. 数据类型

   输入参数 `uint32_t __target` ，表示终点。

   返回类型 `std::vector<uint32_t>` ，表示从源点到汇点的最短路上每条边的编号。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前请先调用计算最短路方法，且模板参数设为 `true` 。
   
   输出的边的编号是按加边顺序确定的。

#### 7.获取最短路路径（按点获取）

1. 数据类型

   输入参数 `uint32_t __target` ，表示终点。

   返回类型 `std::vector<uint32_t>` ，表示从源点到汇点的最短路上每个点的编号。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前请先调用计算最短路方法，且模板参数设为 `true` 。

### 三、模板示例

```c++
#include "GRAPH/BFS.h"
#include "IO/FastIO.h"

int main() {
    //有向图
    OY::BFS G1(4, 3);
    //加三条边
    G1.addEdge(0, 1);
    G1.addEdge(1, 2);
    G1.addEdge(2, 3);
    G1.prepare();
    //设置好源点距离
    G1.setDistance(0);
    //求最短路
    G1.calc<true>();
    cout << "min distance from 0 to 3 is: " << G1.m_distances[3] << endl;
    auto vertex_path = G1.getPath_vertex(3);
    cout << "min_distance_path from 0 to 3 is:\n";
    for (auto i : vertex_path)
        cout << i << endl;

    //无向图
    OY::BFS G2(4, 8);
    //加四对边
    G2.addEdge(0, 1);
    G2.addEdge(1, 0);
    G2.addEdge(0, 2);
    G2.addEdge(2, 0);
    G2.addEdge(2, 1);
    G2.addEdge(1, 2);
    G2.addEdge(1, 3);
    G2.addEdge(3, 1);
    G2.prepare();
    //设置好源点距离
    G2.setDistance(0);
    //求最短路
    G2.calc<true>();
    cout << "min distance from 0 to 3 is: " << G2.m_distances[3] << endl;
    auto edge_path = G2.getPath_edge(3);
    cout << "min_distance_path from 0 to 3 is:\n";
    for (auto i : edge_path) {
        auto [from, to] = G2.m_rawEdges[i];
        cout << from << "->" << to << endl;
    }
}
```

```
#输出如下
min distance from 0 to 3 is: 3
min_distance_path from 0 to 3 is:
0
1
2
3
min distance from 0 to 3 is: 2
min_distance_path from 0 to 3 is:
0->1
1->3
```

