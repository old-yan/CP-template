### 一、模板类别

​	数据结构：`Steiner` 算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

   构造参数 `_Tp __infiniteCost` ，表示无穷大边权值。默认为 `_Tp` 类的最大值的一半。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Steiner` 算法处理的问题，与无向图的最小生成树问题很接近。区别就是，`Steiner` 算法允许在要连通的点集之外，选取一些额外的点，也就是这些第三方的点并不要求被连通，但是可以提供连通协助。

   最小斯坦纳树有解的前提是全图连通，本数据结构不考虑不连通的情况。
   
   本数据结构可以接受重边和自环。

   **注意：**
   
   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示无向边的起点编号。

   输入参数 `uint32_t __b` ，表示无向边的终点编号。

   输入参数 `_Tp __cost` ，表示无向边的边权。

2. 时间复杂度

   $O(1)$。

#### 3.预备

1. 数据类型

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法用于在加完所有边后，进行一些预处理。


#### 4.计算最小斯坦纳树（Dijkstra算法）

1. 数据类型

   模板参数 `bool _GetPath` ，表示是否要记录方案。

   输入参数 `const std::vector<uint32_t>&__keys` ，表示必须要连通的点。

2. 时间复杂度

   $O(n\cdot 3^k+m\cdot \log n\cdot 2^k)$ 。

3. 备注

   在调用本方法之前，请先预备。

   建立最小斯坦纳树的总代价保存为 `m_minCost` 。

#### 5.计算最小斯坦纳树（SPFA算法）

1. 数据类型

   模板参数 `bool _GetPath` ，表示是否要记录方案。

   输入参数 `const std::vector<uint32_t>&__keys` ，表示必须要连通的点。

2. 时间复杂度

   $O(n\cdot 3^k+m\cdot n\cdot 2^k)$ 。

3. 备注

   在调用本方法之前，请先预备。

   建立最小斯坦纳树的总代价保存为 `m_minCost` 。

   虽然时间复杂度比 `Dijkstra` 版的要高，但实际跑起来有可能比 `Dijkstra` 版更快。当然一般情况下 `Dijkstra` 的速度已经够了。

#### 6.获取建立最小斯坦纳树的方案

1. 数据类型

   返回类型 `std::vector<uint32_t>` ，表示建立最小斯坦纳树用到的所有边。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   在调用本方法之前请先调用建立最小斯坦纳树方法。
   

### 三、模板示例

```c++
#include "GRAPH/Steiner.h"
#include "IO/FastIO.h"

int main() {
    //无向图
    OY::Steiner<int> G(6, 8);
    //加八条边
    G.addEdge(0, 1, 100);
    G.addEdge(1, 2, 120);
    G.addEdge(2, 0, 110);
    G.addEdge(0, 3, 20);
    G.addEdge(4, 3, 10);
    G.addEdge(4, 2, 50);
    G.addEdge(4, 5, 10);
    G.addEdge(5, 1, 30);
    G.prepare();
    //计算可能性
    if (!G.calc_dijk<true>({0, 1, 2})) {
        cout << "Graph is not connected\n";
    } else {
        cout << "Graph is connected\n";
        cout << "total cost : " << G.m_minCost << endl;
        cout << "use edges:\n";
        for (auto index : G.getEdges()) {
            cout << "No." << index << " : ";
            auto [from, to, cost] = G.m_rawEdges[index];
            cout << from << " -> " << to << '\n';
        }
    }
}
```

```
#输出如下
Graph is connected
total cost : 120
use edges:
No.3 : 0 -> 3
No.4 : 4 -> 3
No.5 : 4 -> 2
No.6 : 4 -> 5
No.7 : 5 -> 1

```

