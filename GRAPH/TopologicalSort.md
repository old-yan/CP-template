### 一、模板类别

​	数据结构：拓扑排序算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本数据结构可以接受重边和自环。

   **注意：**
   
   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。

   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示有向边的起点编号。

   输入参数 `uint32_t __b` ，表示有向边的终点编号。

2. 时间复杂度

   $O(1)$ 。


#### 3.预备

1. 数据类型

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法用于在加完所有边后，进行一些预处理。

#### 4.拓扑排序

1. 数据类型

   返回类型 `bool` ，表示原图是否可以进行拓扑排序。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   在调用本方法前，请先预备。

   本方法按照分层广度优先遍历的方法，进行拓扑排序。

   如果图中无环，可以进行拓扑排序，返回 `true` 。

   如果图中有环，无法进行拓扑排序，返回 `false` 。


#### 5.获取所有源

1. 数据类型

   返回类型 `std::basic_string_view<uint32_t>` ，表示所有入度为零的结点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前请先调用拓扑排序方法。
   

### 三、模板示例

```c++
#include "GRAPH/TopologicalSort.h"
#include "IO/FastIO.h"

int main() {
    //有向图
    OY::TopologicalSort G(5, 5);
    //加三条边
    G.addEdge(0, 1);
    G.addEdge(1, 2);
    G.addEdge(2, 4);
    G.addEdge(1, 4);
    G.addEdge(3, 4);
    G.prepare();
    //拓扑排序
    if (!G.calc()) {
        cout << "There is cycle in the graph\n";
    } else {
        cout << "There is no cycle in the graph\n";
        for (auto a : G.m_queue) cout << a << ' ';
        cout << "\nsources:\n";
        for (auto a : G.getSources()) cout << a << ' ';
    }
}
```

```
#输出如下
There is no cycle in the graph
0 3 1 2 4 
sources:
0 3 

```

