### 一、模板类别

​	数据结构：仙人掌图。

### 二、模板功能

  本模板继承于圆方树模板，构造函数、成员方法均与圆方树相同。

  本模板具有一些特殊的性质，因而有一些特殊的成员方法。

#### 1.计算最大独立集

1. 数据类型

   返回类型 `uint32_t` ，表示最大独立集大小。

2. 时间复杂度

   $O(n)$ 。   

### 三、模板示例

```c++
#include "GRAPH/Cactus.h"
#include "IO/FastIO.h"

int main() {
    OY::Cactus G(5, 5);
    // 加五条边
    G.addEdge(0, 1);
    G.addEdge(1, 2);
    G.addEdge(2, 3);
    G.addEdge(0, 2);
    G.addEdge(0, 4);
    // 运行主算法
    G.calc<true>(0);
    // 计算最大独立集大小
    cout << "max Independant Set: " << G.maxIndependantSet() << endl;
    // 计算最小支配大小
    cout << "min Dominating Set: " << G.m_tarjan.m_vertexNum - G.maxIndependantSet() << endl;
}
```

```
#输出如下
max Independant Set: 3
min Dominating Set: 2

```

