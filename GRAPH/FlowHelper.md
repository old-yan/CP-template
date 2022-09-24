### 一、模板类别

​	数据结构：网络流助手。

### 二、模板功能

#### 1.最大流助手

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   模板参数 `typename _Solver` ，表示最大流解法。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   普通的最大流模板，在跑一次最大流之后，边上的容量都已不再是原来的值，如果想要换源换汇重跑最大流，非常不便；而且在跑最大流时，不能输出方案。而最大流助手解决了这一问题。

   `recover` 方法可以在跑完最大流之后恢复原图；
   
   `getPath` 方法可以求出每条边的流量值。

   **注意：**
   
   最大流助手可以输入自环，但是自环不会有流。

   当使用最大流助手时，只能使用普通的“预备”，不能使用按顺序预备。

#### 2.上下限流助手

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   模板参数 `typename _Solver` ，表示最大流解法。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   上下限流助手可以在限定边的流的上下限的情况下计算最大流。

   `addEdge` 方法不仅能加边，而且设置好了上下限。

   在预备之前，需要先调用 `setting` 方法；

   `isPossible` 方法返回可行性。

   `minFlow` 方法求出最小可行流。

   `maxFlow` 方法求出最大可行流。

   `getPath` 方法可以求出每条边的流量值。

   **注意：**

   上下限流助手可以输入自环，但是自环只会跑最小流。

   当使用上下限流助手时，只能使用普通的“预备”，不能使用按顺序预备。

### 三、模板示例

```c++
#include "GRAPH/FlowHelper.h"
#include "IO/FastIO.h"

int main() {
    //建图
    OY::FlowHelper<int, OY::Dinic> G1(4, 5);
    //加五条边
    G1.addEdge(3, 1, 300);
    G1.addEdge(3, 2, 200);
    G1.addEdge(1, 2, 200);
    G1.addEdge(1, 0, 300);
    G1.addEdge(0, 2, 400);
    G1.prepare();
    //计算最大流
    cout << "max flow from 3 to 2: " << G1.calc(3, 2) << endl;
    //输出方案
    auto path = G1.getPath();
    for (uint32_t i = 0; i < 5; i++) {
        auto [from, to, cap] = G1.m_rawEdges[i];
        cout << "from " << from << " to " << to << ": " << path[i] << endl;
    }

    //建图
    OY::BoundFlow<int, OY::Dinic> G2(4, 5);
    //加五条边
    G2.addEdge(3, 1, 100, 300);
    G2.addEdge(3, 2, 100, 200);
    G2.addEdge(1, 2, 100, 200);
    G2.addEdge(1, 0, 100, 300);
    G2.addEdge(0, 2, 100, 400);
    //设置
    G2.setting(3, 2);
    G2.prepare();
    //先查看是否可行
    if (G2.isPossible()) {
        cout << "min flow from 3 to 2: " << G2.minFlow() << endl;
        auto path = G2.getPath();
        for (uint32_t i = 0; i < 5; i++) {
            auto [from, to, cap] = G2.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
    }
    G2.recover();
    //先查看是否可行
    if (G2.isPossible()) {
        cout << "max flow from 3 to 2: " << G2.maxFlow() << endl;
        auto path = G2.getPath();
        for (uint32_t i = 0; i < 5; i++) {
            auto [from, to, cap] = G2.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
    }
}
```

```
#输出如下
max flow from 3 to 2: 500
from 3 to 1: 300
from 3 to 2: 200
from 1 to 2: 200
from 1 to 0: 100
from 0 to 2: 100
min flow from 3 to 2: 300
from 3 to 1: 200
from 3 to 2: 100
from 1 to 2: 100
from 1 to 0: 100
from 0 to 2: 100
max flow from 3 to 2: 500
from 3 to 1: 300
from 3 to 2: 200
from 1 to 2: 200
from 1 to 0: 100
from 0 to 2: 100

```

