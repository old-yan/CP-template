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

   `recover` 方法可以在任何时机恢复原图；

   `getPath` 方法可以求出每条边的流量值。

   **注意：**

   最大流助手可以输入自环，但是自环不会有流。

   当使用最大流助手时，只能使用普通的“预备”，不能使用按顺序预备。

   **注意：** 目前，预流推进法求最大流的模板，在求解最大流之后每个点并不满足流量平衡。所以如果采用预流推进法，不能输出方案。

#### 2.上下限流助手

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   模板参数 `typename _Solver` ，表示最大流解法。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

   构造参数 `uint32_t __source` ，表示源点。默认值为 `-1` ，表示令编号 `__vertexNum` 的结点（虚拟源点）为源点。

   构造参数 `uint32_t __target` ，表示汇点。默认值为 `-1` ，表示令编号 `__vertexNum + 1` 的结点（虚拟汇点）为汇点。

   构造参数 `_Tp __infiniteCap` ，表示无穷大容量。默认为 `_Tp` 类的最大值的一半。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   上下限流助手可以在限定边的流的上下限的情况下计算最大流。

   `addEdge` 方法不仅能加边，而且设置好了上下限。

   `isPossible` 方法返回可行性以及此时源到汇的流量。

   `minFlow` 方法求出最小可行流。

   `maxFlow` 方法求出最大可行流。

   `recover` 方法可以在任何时机恢复原图；

   `getPath` 方法可以求出每条边的流量值。

   **注意：**

   上下限流助手可以输入自环，但是自环只会跑最小流。

   在使用虚拟源汇点为源汇的情况下， `isPossible` 返回的流量无意义， `minFlow` 和 `maxFlow` 方法均无意义。此时仅关注可行性。

### 三、模板示例

```c++
#include "GRAPH/FlowHelper.h"
#include "IO/FastIO.h"
#include <assert.h>

int main() {
    // 建图
    OY::FlowHelper<int, OY::Dinic> G1(4, 5);
    // 加五条边
    G1.addEdge(3, 1, 300);
    G1.addEdge(3, 2, 200);
    G1.addEdge(1, 2, 200);
    G1.addEdge(1, 0, 300);
    G1.addEdge(0, 2, 400);
    G1.prepare();
    // 计算最大流
    cout << "max flow from 3 to 2: " << G1.calc(3, 2) << endl;
    // 输出方案
    auto path = G1.getPath();
    for (uint32_t i = 0; i < 5; i++) {
        auto [from, to, cap] = G1.m_sol.m_rawEdges[i];
        cout << "from " << from << " to " << to << ": " << path[i] << endl;
    }

    //*************************************************************************************************
    // 建图，在无源汇情况下计算可行性
    cout << "\nbound flow graph with no source and target:\n";
    OY::BoundFlow<int, OY::Dinic> G2(4, 5);
    // 加五条边
    G2.addEdge(0, 2, 100, 200);
    G2.addEdge(3, 0, 100, 300);
    G2.addEdge(3, 1, 0, 300);
    G2.addEdge(1, 2, 0, 100);
    G2.addEdge(2, 3, 300, 400);
    G2.prepare();
    // 先查看是否可行
    if (G2.isPossible().first) {
        auto path = G2.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap] = G2.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
    } else {
        cout << "Impossible\n";
    }

    //*************************************************************************************************
    // 建图，计算从 2 到 3 的最小可行流、最大可行流
    cout << "\nbound flow graph with source and target:\n";
    OY::BoundFlow<int, OY::Dinic> G3(4, 5, 2, 3);
    // 加五条边
    G3.addEdge(0, 2, 100, 200);
    G3.addEdge(3, 0, 100, 300);
    G3.addEdge(2, 1, 0, 300);
    G3.addEdge(1, 3, 0, 100);
    G3.addEdge(2, 3, 300, 400);
    G3.prepare();
    // 先查看是否可行
    if (auto [res, flow] = G3.isPossible(); res) {
        cout << "flow from 2 to 3: " << flow << endl;
        auto path = G3.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap] = G3.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
        // 在可行前提下，可以计算最小流和最大流
        cout << "min flow from 2 to 3: " << G3.minFlow() << endl;
        path = G3.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap] = G3.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
        G3.recover();
        assert(G3.isPossible().first);
        cout << "max flow from 2 to 3: " << G3.maxFlow() << endl;
        path = G3.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap] = G3.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
    } else {
        cout << "Impossible";
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

bound flow graph with no source and target:
from 0 to 2: 200
from 3 to 0: 200
from 3 to 1: 100
from 1 to 2: 100
from 2 to 3: 300

bound flow graph with source and target:
flow from 2 to 3: 200
from 0 to 2: 100
from 3 to 0: 100
from 2 to 1: 0
from 1 to 3: 0
from 2 to 3: 300
min flow from 2 to 3: 100
from 0 to 2: 200
from 3 to 0: 200
from 2 to 1: 0
from 1 to 3: 0
from 2 to 3: 300
max flow from 2 to 3: 400
from 0 to 2: 100
from 3 to 0: 100
from 2 to 1: 100
from 1 to 3: 100
from 2 to 3: 400

```

