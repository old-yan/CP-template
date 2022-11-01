### 一、模板类别

​	数据结构：`Floyd` 算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `_Tp __infiniteDistance` ，表示无穷大边权值。默认为 `_Tp` 类的最大值的一半。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

   `Floyd` 算法处理的问题为有向图的最短路问题。

   如果图为无向图，需要将原图的边视为正反两个方向的有向边来构建。
   
   对于负权图，只有在确保图中没有负圈的情况下可以计算最短路。

   本数据结构可以接受重边和自环。
   
   **注意：**
   
   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   由于存图采取的是邻接矩阵方法，所以无须对边数进行预估。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示有向边的起点编号。

   输入参数 `uint32_t __b` ，表示有向边的终点编号。

   输入参数 `_Tp __distance` ，表示有向边的边权。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：**在无向图中，需要按正反两个反向进行加边。

#### 3.计算最短路

1. 数据类型

2. 时间复杂度

   $O(n^3)$ 。

3. 备注

   在调用本方法之前请先设置好源点的初始距离。

   本方法一般只在确保无负圈的情况下使用，而不用来检测负圈。

### 三、模板示例

```c++
#include "GRAPH/Floyd.h"
#include "IO/FastIO.h"

int main() {
    //有向图
    OY::Floyd<int> G(3);
    //加边
    G.addEdge(0, 1, 100);
    G.addEdge(1, 2, -10);
    G.addEdge(0, 2, 80);
    G.addEdge(2, 1, 15);
    //求最短路
    G.calc();
    cout << "min distance from 0 to 1 is: " << G.m_distances[0][1] << endl;
    cout << "min distance from 0 to 2 is: " << G.m_distances[0][2] << endl;
}
```

```
#输出如下
min distance from 0 to 1 is: 95
min distance from 0 to 2 is: 80

```

