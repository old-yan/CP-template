### 一、模板类别

​	数据结构：最小直径生成树。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

   构造参数 `_Tp __infiniteDistance` ，表示无穷大边权值。默认为 `_Tp` 类最大值的一半。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本模板处理的问题为无向图的最小直径生成树问题。

   本数据结构可以接受重边和自环。
   
   **注意：**

   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示无向边的起点编号。

   输入参数 `uint32_t __b` ，表示无向边的终点编号。

   输入参数 `_Tp __distance` ，表示无向边的边权。

2. 时间复杂度

   $O(1)$ 。


#### 3.计算最短路

1. 数据类型

2. 时间复杂度

   $O(n^3)$ 。

3. 备注

   本方法按照 `Floyd` 算法计算全源最短路。

#### 4.获取以顶点为中心的最小化最远点

1. 数据类型

   返回类型为自定义结构体，其 `id` 属性表示中心编号， `dis` 属性表示中心到其他点的最短路的最大值。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

   在调用本方法之前请先调用计算最短路方法，并确定全图连通。
   

#### 5.获取以边上某点为中心的最小化最远点

1. 数据类型

   返回类型为自定义结构体，其 `id` 属性表示边的编号，`double_bias` 属性表示中心到该边左端的**距离的二倍**， `double_dis` 属性表示中心到所有点的最短路的**最大值的二倍**。

2. 时间复杂度

   $O(n^2\cdot\log n+ m\cdot n)$ 。

3. 备注

   在调用本方法之前请先调用计算最短路方法，并确定全图连通。

   为什么返回值要返回答案的二倍，而不直接返回答案本身呢？因为中心的取点有可能使得这两个值不是整数，而带着 `1/2` 。那么只要乘二就可以维持原来的整数类型，避免处理麻烦。

   ”边的左端点“取决于本数据结构中存储这条边时的加点顺序，先来的是左端点。

#### 6.获取以某顶点为中心的最短路树

1. 数据类型

   输入参数 `uint32_t __root` ，表示作为中心的顶点编号。

   返回类型 `std::vector<uint32_t>`  ，表示选取的边。

2. 时间复杂度

   $O(m\cdot \log n)$ 。

3. 备注

   本方法返回 `n-1` 条边的编号。且满足，这些边和点组成的图为无环连通图，且从 `__root` 到每个点的距离切好等于原图中它俩的最短路距离。

#### 7.获取以某边上的点为中心的最短路树

1. 数据类型

   输入参数 `uint32_t __edgeId` ，表示边的编号。

   输入参数 `_Tp __doubleBias` ，表示中心距离这条边左端点的距离。

   返回类型 `std::vector<uint32_t>`  ，表示选取的边。

2. 时间复杂度

   $O(m\cdot \log n)$ 。

3. 备注

   本方法返回 `n-1` 条边的编号。且满足，原图中心所在边包含于其中，且这些边和点组成的图为无环连通图。且从中心到每个点的距离切好等于原图中中心到该点的最短路距离。

   ”边的左端点“取决于本数据结构中存储这条边时的加点顺序，先来的是左端点。

### 三、模板示例

```c++
#include "GRAPH/MinimumDiameterSpanningTree.h"
#include "IO/FastIO.h"

int main() {
    OY::MinimumDiameterSpanningTree<int> G(6, 7);
    // 加五条边
    G.addEdge(0, 1, 10);
    G.addEdge(1, 2, 20);
    G.addEdge(0, 2, 30);
    G.addEdge(2, 3, 1000);
    G.addEdge(3, 4, 30);
    G.addEdge(4, 5, 20);
    G.addEdge(3, 5, 10);

    // 计算全源最短路
    G.calc();

    // 先找以顶点为中心
    auto [id1, dis1] = G.getCenterOfVertex();

    // 再找以边上某点为中心
    auto [id2, double_bias, double_dis2] = G.getCenterOfEdge();

    // 如果以顶点为中心可以取得最小值，最好让顶点为中心，比较方便
    if (dis1 * 2 <= double_dis2) {
        cout << "minimum diameter of spanning tree: " << dis1 * 2 << '\n';
        cout << "edges of the tree:\n";
        auto edges = G.getTreeEdgesOfVertexCenter(id1);
        for (uint32_t index : edges) {
            auto &[from, to, dis] = G.m_edges[index];
            cout << "from " << from << " to " << to << '\n';
        }
    } else {
        cout << "minimum diameter of spanning tree: " << double_dis2 << '\n';
        cout << "edges of the tree:\n";
        auto edges = G.getTreeEdgesOfEdgeCenter(id2, double_dis2);
        for (uint32_t index : edges) {
            auto &[from, to, dis] = G.m_edges[index];
            cout << "from " << from << " to " << to << '\n';
        }
    }
}
```

```
#输出如下
minimum diameter of spanning tree: 1060
edges of the tree:
from 0 to 2
from 1 to 2
from 3 to 4
from 3 to 5
from 2 to 3

```

