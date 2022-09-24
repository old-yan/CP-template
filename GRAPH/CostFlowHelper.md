### 一、模板类别

​	数据结构：费用流助手。

### 二、模板功能

#### 1.费用流助手

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的流量类型。

   模板参数 `typename _Fp` ，表示图中边权的费用类型。

   模板参数 `typename _Solver` ，表示费用流解法。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   普通的费用流模板，在跑一次费用流之后，边上的容量都已不再是原来的值，如果想要换源换汇重跑费用流，非常不便；而且在跑费用流时，不能输出方案。而费用流助手解决了这一问题。

   `recover` 方法可以在跑完费用流之后恢复原图；
   
   `getPath` 方法可以求出每条边的流量值。

   **注意：**
   
   费用流助手可以输入自环，但是自环不会有流。

   当使用费用流助手时，只能使用普通的“预备”，不能使用按顺序预备。

#### 2.上下限费用流助手

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的流量类型。

   模板参数 `typename _Fp` ，表示图中边权的费用类型。

   模板参数 `typename _Solver` ，表示费用流解法。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   上下限费用流助手可以在限定边的流的上下限的情况下计算最小费用流。

   `addEdge` 方法不仅能加边，而且设置好了上下限。

   在预备之前，需要先调用 `setting` 方法；

   `isPossible` 方法返回可行性。

   `minFlow` 方法求出最小费用最小流。

   `maxFlow` 方法求出最小费用最大流。

   `getPath` 方法可以求出每条边的流量值。

   **注意：**

   上下限费用流助手可以输入自环，但是自环只会跑最小流。

   当使用上下限费用流助手时，只能使用普通的“预备”，不能使用按顺序预备。

#### 3.负环费用流助手

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的流量类型。

   模板参数 `typename _Fp` ，表示图中边权的费用类型。

   模板参数 `typename _Solver` ，表示费用流解法。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   负环费用流助手可以在有负（费用）环的情况下计算最小费用最大流。

   `addEdge` 方法可以加边。

   在预备之前，需要先调用 `setting` 方法；

   `calc` 方法返回最小费用最小流。

   `getPath` 方法可以求出每条边的流量值。

   **注意：**

   负环费用流助手可以输入自环，但是自环不会有流。

   当使用负环费用流助手时，只能使用普通的“预备”，不能使用按顺序预备。

### 三、模板示例

```c++
#include "GRAPH/CostFlowHelper.h"
#include "IO/FastIO.h"

int main() {
    //建图
    OY::CostFlowHelper<int, int, OY::Dinic_mcmf> G1(4, 5);
    //加五条边
    G1.addEdge(3, 1, 300, 1);
    G1.addEdge(3, 2, 200, 1);
    G1.addEdge(1, 2, 200, 1);
    G1.addEdge(1, 0, 300, 1);
    G1.addEdge(0, 2, 400, 1);
    G1.prepare();
    //计算最小费用最大流
    auto [flow, cost] = G1.calc(3, 2);
    cout << "max flow from 3 to 2: " << flow << endl;
    cout << "meanwhile cost: " << cost << endl;
    //输出方案
    auto path = G1.getPath();
    for (uint32_t i = 0; i < 5; i++) {
        auto [from, to, cap, cost] = G1.m_rawEdges[i];
        cout << "from " << from << " to " << to << ": " << path[i] << endl;
    }

    //建图
    OY::BoundCostFlow<int, int, OY::Dinic_mcmf> G2(4, 5);
    //加五条边
    G2.addEdge(3, 1, 100, 300, 1);
    G2.addEdge(3, 2, 100, 200, 1);
    G2.addEdge(1, 2, 100, 200, 1);
    G2.addEdge(1, 0, 100, 300, 1);
    G2.addEdge(0, 2, 100, 400, 1);
    //设置
    G2.setting(3, 2);
    G2.prepare();
    //先查看是否可行
    if (G2.isPossible()) {
        auto [flow, cost] = G2.minFlow();
        cout << "max flow from 3 to 2: " << flow << endl;
        cout << "meanwhile cost: " << cost << endl;
        auto path = G2.getPath();
        for (uint32_t i = 0; i < 5; i++) {
            auto [from, to, cap, cost] = G2.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
    }
    G2.recover();
    //先查看是否可行
    if (G2.isPossible()) {
        auto [flow, cost] = G2.maxFlow();
        cout << "max flow from 3 to 2: " << flow << endl;
        cout << "meanwhile cost: " << cost << endl;
        auto path = G2.getPath();
        for (uint32_t i = 0; i < 5; i++) {
            auto [from, to, cap, cost] = G2.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
    }

    //建图
    OY::NegativeCostFlow<int, int, OY::Dinic_mcmf> G3(4, 6);
    //加五条边
    G3.addEdge(3, 1, 300, 1);
    G3.addEdge(3, 2, 200, 1);
    G3.addEdge(1, 2, 200, 1);
    G3.addEdge(1, 0, 300, 1);
    G3.addEdge(0, 2, 400, 1);
    G3.addEdge(2, 0, 900, -10);
    //设置
    G3.setting(3, 2);
    G3.prepare();
    std::tie(flow, cost) = G3.calc();
    cout << "max flow from 3 to 2: " << flow << endl;
    cout << "meanwhile cost: " << cost << endl;
    path = G3.getPath();
    for (uint32_t i = 0; i < 6; i++) {
        auto [from, to, cap, cost] = G3.m_rawEdges[i];
        if (path[i] > 0)
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        else
            cout << "from " << to << " to " << from << ": " << -path[i] << endl;
    }
}
```

```
#输出如下
max flow from 3 to 2: 500
meanwhile cost: 900
from 3 to 1: 300
from 3 to 2: 200
from 1 to 2: 200
from 1 to 0: 100
from 0 to 2: 100
max flow from 3 to 2: 300
meanwhile cost: 600
from 3 to 1: 200
from 3 to 2: 100
from 1 to 2: 100
from 1 to 0: 100
from 0 to 2: 100
max flow from 3 to 2: 500
meanwhile cost: 900
from 3 to 1: 300
from 3 to 2: 200
from 1 to 2: 200
from 1 to 0: 100
from 0 to 2: 100
max flow from 3 to 2: 500
meanwhile cost: -1800
from 3 to 1: 300
from 3 to 2: 200
from 1 to 2: 200
from 1 to 0: 100
from 0 to 2: 400
from 2 to 0: 300

```

