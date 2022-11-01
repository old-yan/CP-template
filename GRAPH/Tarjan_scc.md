### 一、模板类别

​	数据结构：`Tarjan` 算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Tarjan` 算法处理的问题为有向图的强连通分量问题。

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

#### 4.计算强连通分量

1. 数据类型

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   在调用本方法前，请先预备。
   
   本方法对所有结点赋予 `id` ，相同 `id` 的结点视为在同一个强连通分量中。
   
   **注意：**
   
   特别的，所有结点按照 `id` 非降序排列，恰好为原图的拓扑排序。


#### 5.按组获取所有强连通分量

1. 数据类型

   返回类型 `std::vector<std::basic_string_view<uint32_t>>` ，表示分好组的结点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前请先调用计算强连通分量方法。
   

### 三、模板示例

```c++
#include "GRAPH/Tarjan_scc.h"
#include "IO/FastIO.h"

int main() {
    //有向图
    OY::Tarjan_scc G(5, 5);
    //加三条边
    G.addEdge(0, 1);
    G.addEdge(1, 2);
    G.addEdge(2, 0);
    G.addEdge(3, 3);
    G.addEdge(3, 1);
    G.prepare();
    //求强连通分量
    G.calc();
    cout << "There are " << G.m_idCount << " groups of SCC\n";
    auto groups = G.groups();
    for (uint32_t i = 0; i < groups.size(); i++) {
        cout << "No." << i << " group:\n";
        for (auto a : groups[i]) cout << a << ' ';
        cout << endl;
    }
}
```

```
#输出如下
There are 3 groups of SCC
No.0 group:
4 
No.1 group:
3 
No.2 group:
0 1 2 

```

