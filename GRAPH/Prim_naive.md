### 一、模板类别

​	数据结构：`Prim` 算法的朴素实现。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Prim` 算法处理的问题为无向图的最小生成树问题。

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

   $O(1)$ 。

#### 3.预备

1. 数据类型

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法用于在加完所有边后，进行一些预处理。


#### 4.建立最小生成树

1. 数据类型

   输入参数 `_Tp __infiniteCost` ，表示无穷大边权值。默认为 `_Tp` 类的最大值的一半。

   返回类型 `bool` ，表示是否可以建立最小生成树。

2. 时间复杂度

   $O(m^2+m)$ 。

3. 备注

   对于全图连通的情况，返回 `true` 。

   对于不连通的情况，返回 `false` 。

   在全图连通的情况下，可以根据 `m_used` 来获知某条边是否被使用。

#### 5.获取建立最小生成树的代价

1. 数据类型

   返回类型 `_Tp` ，表示建立最小生成树的代价。

2. 时间复杂度

   $O(m)$ 。

3. 备注

   在调用本方法之前请先调用建立最小生成树方法，且确定全图连通。
   

### 三、模板示例

```c++
#include "GRAPH/Prim_naive.h"
#include "IO/FastIO.h"

int main() {
    OY::Prim_naive<int> G(4, 5);
    //加五条边
    G.addEdge(0, 1, 100);
    G.addEdge(1, 2, 20);
    G.addEdge(2, 3, 40);
    G.addEdge(0, 2, 110);
    G.addEdge(2, 3, 30);
    G.prepare();
    //建立最小生成树
    if (!G.calc()) {
        cout << "There is no MST\n";
    } else {
        cout << "There is MST\n";
        cout << "MST total cost: " << G.totalCost() << endl;
        cout << "used edges:\n";
        for (uint32_t i = 0; i < G.m_used.size(); i++)
            if (G.m_used[i]) {
                auto [from, to, cost] = G.m_rawEdges[i];
                cout << "No." << i << " edge: " << from << "->" << to << " cost = " << cost << '\n';
            }
    }
}
```

```
#输出如下
There is MST
MST total cost: 150
used edges:
No.0 edge: 0->1 cost = 100
No.1 edge: 1->2 cost = 20
No.4 edge: 2->3 cost = 30

```

