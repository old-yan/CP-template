### 一、模板类别

​	数据结构：匈牙利算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   构造参数 `uint32_t __leftNum`​ ，表示图中左侧集合的点数。

   构造参数 `uint32_t __rightNum` ，表示图中右侧集合的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   匈牙利算法处理的问题为二分图的最大匹配。

   本数据结构可以接受重边。
   
   本数据结构没有自环概念。

   **注意：**
   
   本数据结构一开始指定的 `__leftNum` 和 `__rightNum` 参数必须是确切值。
   
   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示边的起点在左侧集合中的编号。

   输入参数 `uint32_t __b` ，表示边的终点在右侧集合中的编号。

2. 时间复杂度

   $O(1)$。


#### 3.预备

1. 数据类型

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法用于在加完所有边后，进行一些预处理。

#### 4.求最大匹配

1. 数据类型

   返回类型 `uint32_t` ，表示求出的最大匹配值。

2. 时间复杂度

   $O(n\cdot m)$ 。

3. 备注

   在调用本方法之前，请先预备。


### 三、模板示例

```c++
#include "GRAPH/Hungarian.h"
#include "IO/FastIO.h"

int main() {
    //建立二分图
    OY::Hungarian G(3, 3, 5);
    //加五条边，G.addEdge(a,b) 表示男孩 a 喜欢女孩 b
    G.addEdge(0, 1);
    G.addEdge(1, 2);
    G.addEdge(2, 1);
    G.addEdge(2, 2);
    G.addEdge(1, 1);
    G.prepare();
    //求最大匹配
    cout << "max match = " << G.calc() << endl;
    //看每个男孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "boy " << i << " matches girl " << int(G.m_leftMatch[i]) << endl;
    }
    //看每个女孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "girl " << i << " matches boy " << int(G.m_rightMatch[i]) << endl;
    }
}
```

```
#输出如下
max match = 2
boy 0 matches girl 1
boy 1 matches girl 2
boy 2 matches girl -1
girl 0 matches boy -1
girl 1 matches boy 0
girl 2 matches boy 1

```

