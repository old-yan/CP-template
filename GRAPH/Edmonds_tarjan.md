### 一、模板类别

​	数据结构：`Edmonds-tarjan` 算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Edmonds` 算法处理的问题为最小树形图问题，即有向图的最小生成树问题。而 `tarjan` 对他的算法进行了一些改进，使其效率更高。

   本数据结构可以接受重边和自环。
   
   **注意：**

   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示有向边的起点编号。

   输入参数 `uint32_t __b` ，表示有向边的终点编号。

   输入参数 `_Tp __cost` ，表示有向边的边权。

2. 时间复杂度

   $O(1)$ 。


#### 3.计算最小树形图

1. 数据类型

   模板参数 `bool _GetPath` ，表示是否记录方案。

   输入参数 `uint32_t __root` ，表示指定的根。

   输入参数 `_Tp __infiniteCost` ，表示无穷大边权，默认为 `_Tp` 类的最大值的一半。

   返回类型 `bool` ，表示是否可以获取最小树形图。

2. 时间复杂度

   $O(m\cdot \log m)$ 。

3. 备注

   如果不能生成最小树形图，返回 `false` 。

   如果可以生成最小树形图，返回 `true` ，总代价保存在 `m_totalCost` 属性里。

   如果记录了方案，可以根据 `m_useCount` 属性获知每条边是否被使用。

### 三、模板示例

```c++
#include "GRAPH/Edmonds_tarjan.h"
#include "IO/FastIO.h"

int main() {
    OY::Edmonds_tarjan<int> G(4, 6);
    //加五条边
    G.addEdge(0, 1, 100);
    G.addEdge(0, 2, 30);
    G.addEdge(3, 0, 60);
    G.addEdge(3, 1, 65);
    G.addEdge(2, 1, 30);
    G.addEdge(2, 3, 35);
    if (!G.calc<true>(0)) {
        cout << "there is no tree with root 0\n";
    } else {
        cout << "there is a tree with root 0\n";
        cout << "total cost : " << G.m_totalCost << endl;
        cout << "used edges:\n";
        for (int i = 0; i < G.m_useCount.size(); i++) {
            if (G.m_useCount[i]) {
                auto [from, to, cost] = G.m_edges[i];
                cout << "No." << i << " edge: " << from << " -> " << to << " cost = " << cost << '\n';
            }
        }
    }
}
```

```
#输出如下
there is a tree with root 0
total cost : 95
used edges:
No.1 edge: 0 -> 2 cost = 30
No.4 edge: 2 -> 1 cost = 30
No.5 edge: 2 -> 3 cost = 35

```

