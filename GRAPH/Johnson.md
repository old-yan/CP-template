### 一、模板类别

​	数据结构：`Johnson` 算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

   构造参数 `_Tp __infiniteDistance` ，表示无穷大边权值。默认为 `_Tp` 类的最大值的一半。
   
2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Johnson` 算法处理的问题为有向图的全源最短路问题。

   如果图为无向图，需要将原图的边视为正反两个方向的有向边来构建。
   
   本数据结构可以处理带负权的边。

   本数据结构可以接受重边和自环。
   
   **注意：**
   
   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。对于无向图，本参数须为图的边数的二倍。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示有向边的起点编号。

   输入参数 `uint32_t __b` ，表示有向边的终点编号。

   输入参数 `_Tp __distance` ，表示有向边的边权。

2. 时间复杂度

   $O(1)$。

3. 备注

   **注意：**在无向图中，需要按正反两个反向进行加边。

#### 3.预备

1. 数据类型

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法用于在加完所有边后，进行一些预处理。

#### 4.计算最短路

1. 数据类型

   返回类型 `bool` ，表示是否可以计算出全源最短路。

2. 时间复杂度

   $O(n\cdot m\cdot \log n)$ 。

3. 备注

   在调用本方法之前请先预备。
   
   对于无负圈的情况，返回 `true` ，且可以求出最短路。
   
   对于负圈存在的情况，返回 `false` ，且无法求出最短路。

### 三、模板示例

```c++
#include "GRAPH/Johnson.h"
#include "IO/FastIO.h"

int main() {
    //有向图
    OY::Johnson<int> G(5, 6);
    //加五条边
    G.addEdge(0, 1, 100);
    G.addEdge(1, 2, -30);
    G.addEdge(2, 0, 70);
    G.addEdge(3, 3, 10);
    G.addEdge(3, 1, 20);
    G.addEdge(1, 4, 100);
    G.prepare();
    //求最短路，检查有无负圈
    if (!G.calc()) {
        cout << "has negative cycle\n";
    } else {
        cout << "no negative cycle\n";
        cout << "min distance from 3 to 0 is: " << G.m_distances[3][0] << endl;
    }
}
```

```
#输出如下
no negative cycle
min distance from 3 to 0 is: 60

```

