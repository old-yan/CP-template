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

   目前费用流助手只支持 `SSP` 算法模板。

#### 2.上下限费用流助手

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的流量类型。

   模板参数 `typename _Fp` ，表示图中边权的费用类型。

   模板参数 `typename _Solver` ，表示费用流解法。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

   构造参数 `uint32_t __source` ，表示源点。默认值为 `-1` ，表示令编号 `__vertexNum` 的结点（虚拟源点）为源点。

   构造参数 `uint32_t __target` ，表示汇点。默认值为 `-1` ，表示令编号 `__vertexNum + 1` 的结点（虚拟汇点）为汇点。

   构造参数 `_Tp __infiniteCap` ，表示无穷大容量。默认为 `_Tp` 类的最大值的一半。

   构造参数 `_Tp __infiniteCost` ，表示无穷大费用。默认为 `_Fp` 类的最大值的一半。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   上下限费用流助手可以在限定边的流的上下限的情况下计算最小费用流。

   `addEdge` 方法不仅能加边，而且设置好了上下限。

   `isPossible` 方法返回可行性以及此时源到汇的流量和最小费用。

   `minFlow` 方法求出最小可行流和最小费用。

   `maxFlow` 方法求出最大可行流和最小费用。
   
   `recover` 方法可以在任何时机恢复原图；
   
   `getPath` 方法可以求出每条边的流量值。
   
   **注意：**
   
   上下限费用流助手可以输入自环，但是自环只会跑最小流。
   
   在使用虚拟源汇点为源汇的情况下， `isPossible` 返回的流量无意义， `minFlow` 和 `maxFlow` 方法均无意义。此时仅关注可行性和最小费用。

#### 3.负环费用流助手

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的流量类型。

   模板参数 `typename _Fp` ，表示图中边权的费用类型。

   模板参数 `typename _Solver` ，表示费用流解法。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

   构造参数 `uint32_t __source` ，表示源点。

   构造参数 `uint32_t __target` ，表示汇点。

   构造参数 `_Tp __infiniteCap` ，表示无穷大容量。默认为 `_Tp` 类的最大值的一半。

   构造参数 `_Tp __infiniteCost` ，表示无穷大费用。默认为 `_Fp` 类的最大值的一半。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   负环费用流助手可以在有负（费用）环的情况下计算最小费用最大流。

   `addEdge` 方法可以加边。

   `calc` 方法返回最小费用最大流。

   **注意：**

   负环费用流助手可以输入自环，但是自环不会有流。

   负环费用流助手没有默认源汇点，即必须用实点做源汇，不能用虚拟源汇做源汇。也就是说，不存在可行性判定问题，只存在优化问题。

#### 4.带负圈上下限费用流助手

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的流量类型。

   模板参数 `typename _Fp` ，表示图中边权的费用类型。

   模板参数 `typename _Solver` ，表示费用流解法。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

   构造参数 `uint32_t __source` ，表示源点。默认值为 `-1` ，表示令编号 `__vertexNum` 的结点（虚拟源点）为源点。

   构造参数 `uint32_t __target` ，表示汇点。默认值为 `-1` ，表示令编号 `__vertexNum + 1` 的结点（虚拟汇点）为汇点。

   构造参数 `_Tp __infiniteCap` ，表示无穷大容量。默认为 `_Tp` 类的最大值的一半。

   构造参数 `_Tp __infiniteCost` ，表示无穷大费用。默认为 `_Fp` 类的最大值的一半。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   带负圈上下限费用流助手可以在有负（费用）环，且在限定边的流的上下限的情况下计算最小费用流。

   `addEdge` 方法可以加边。

   `isPossible` 方法返回可行性以及此时源到汇的流量和最小费用。

   `minFlow` 方法求出最小可行流和最小费用。

   `maxFlow` 方法求出最大可行流和最小费用。
   
   `recover` 方法可以在任何时机恢复原图；
   
   `getPath` 方法可以求出每条边的流量值。
   
   **注意：**
   
   带负圈上下限费用流助手可以输入自环，但是自环只会跑最小流。
   
   在使用虚拟源汇点为源汇的情况下， `isPossible` 返回的流量无意义， `minFlow` 和 `maxFlow` 方法均无意义。此时仅关注可行性和最小费用。
   
   `isPossible` 方法返回可行性。

### 三、模板示例

```c++
#include "GRAPH/CostFlowHelper.h"
#include "IO/FastIO.h"
#include <assert.h>

int main() {
    // 建图
    OY::CostFlowHelper<int, int, OY::Dinic_mcmf> G1(4, 5);
    // 加五条边
    G1.addEdge(3, 1, 300, 1);
    G1.addEdge(3, 2, 200, 2);
    G1.addEdge(1, 2, 200, 3);
    G1.addEdge(1, 0, 300, 4);
    G1.addEdge(0, 2, 400, 5);
    G1.prepare();
    // 计算最小费用最大流
    auto [flow1, cost1] = G1.calc(3, 2);
    cout << "max flow from 3 to 2: " << flow1 << endl;
    cout << "min cost: " << cost1 << endl;
    // 输出方案
    auto path1 = G1.getPath();
    for (uint32_t i = 0; i < path1.size(); i++) {
        auto [from, to, cap, cost] = G1.m_sol.m_rawEdges[i];
        cout << "from " << from << " to " << to << ": " << path1[i] << endl;
    }

    //*************************************************************************************************
    // 建图，在无源汇情况下计算可行性
    cout << "\nbound cost flow graph with no source and target:\n";
    OY::BoundCostFlow<int, int, OY::Dinic_mcmf> G2(4, 5);
    // 加五条边
    G2.addEdge(0, 2, 100, 200, 1);
    G2.addEdge(3, 0, 100, 300, 2);
    G2.addEdge(3, 1, 0, 300, 3);
    G2.addEdge(1, 2, 0, 100, 4);
    G2.addEdge(2, 3, 300, 400, 5);
    G2.prepare();
    // 先查看是否可行
    if (auto [res, _, cost] = G2.isPossible(); res) {
        cout << "min cost:" << cost << endl;
        auto path = G2.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap, cost] = G2.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
    } else {
        cout << "Impossible\n";
    }

    //*************************************************************************************************
    // 建图，计算从 2 到 3 的最小可行流、最大可行流
    cout << "\nbound cost flow graph with source and target:\n";
    OY::BoundCostFlow<int, int, OY::Dinic_mcmf> G3(4, 5, 2, 3);
    // 加五条边
    G3.addEdge(0, 2, 100, 200, 1);
    G3.addEdge(3, 0, 100, 300, 2);
    G3.addEdge(2, 1, 0, 300, 3);
    G3.addEdge(1, 3, 0, 100, 4);
    G3.addEdge(2, 3, 300, 400, 5);
    G3.prepare();
    // 先查看是否可行
    if (auto [res, flow, cost] = G3.isPossible(); res) {
        cout << "flow from 2 to 3: " << flow << endl;
        cout << "meanwhile min cost: " << cost << endl;
        auto path = G3.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap, _cost] = G3.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
        // 在可行前提下，可以计算最小流和最大流
        auto [minflow, cost2] = G3.minFlow();
        cout << "min flow from 2 to 3: " << minflow << endl;
        cout << "meanwhile min cost: " << cost2 << endl;
        path = G3.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap, _cost] = G3.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
        G3.recover();
        assert(std::get<0>(G3.isPossible()));
        auto [maxflow, cost3] = G3.maxFlow();
        cout << "max flow from 2 to 3: " << maxflow << endl;
        cout << "meanwhile min cost: " << cost3 << endl;
        path = G3.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap, _cost] = G3.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
    } else {
        cout << "Impossible";
    }

    //*************************************************************************************************
    // 建图，计算带负圈图里的最小费用最大流
    cout << "\ncost flow graph with negative cost:\n";
    OY::NegativeCostFlow<int, int, OY::Dinic_mcmf> G4(4, 5, 3, 2);
    // 加五条边
    G4.addEdge(3, 1, 300, -1);
    G4.addEdge(3, 2, 200, -2);
    G4.addEdge(1, 2, 200, -3);
    G4.addEdge(1, 0, 300, -4);
    G4.addEdge(0, 2, 400, -5);
    G4.prepare();
    auto [flow4, cost4] = G4.calc();
    cout << "flow from 2 to 3: " << flow4 << endl;
    cout << "meanwhile min cost: " << cost4 << endl;
    auto path4 = G4.getPath();
    for (uint32_t i = 0; i < path4.size(); i++) {
        auto [from, to, cap, _cost] = G4.m_sol.m_rawEdges[i];
        cout << "from " << from << " to " << to << ": " << path4[i] << endl;
    }

    //*************************************************************************************************
    // 建图，计算从 2 到 3 的最小可行流、最大可行流
    cout << "\nbound cost flow graph with source and target and negative cost:\n";
    OY::NegativeBoundCostFlow<int, int, OY::Dinic_mcmf> G6(4, 5, 2, 3);
    // 加五条边
    G6.addEdge(0, 2, 100, 200, -1);
    G6.addEdge(3, 0, 100, 300, -2);
    G6.addEdge(2, 1, 0, 300, -3);
    G6.addEdge(1, 3, 0, 100, -4);
    G6.addEdge(2, 3, 300, 400, -5);
    G6.prepare();
    // 先查看是否可行
    if (auto [res, flow, cost] = G6.isPossible(); res) {
        cout << "flow from 2 to 3: " << flow << endl;
        cout << "meanwhile min cost: " << cost << endl;
        auto path = G6.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap, _cost] = G6.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
        // 在可行前提下，可以计算最小流和最大流
        auto [minflow, cost2] = G6.minFlow();
        cout << "min flow from 2 to 3: " << minflow << endl;
        cout << "meanwhile min cost: " << cost2 << endl;
        path = G6.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap, _cost] = G6.m_sol.m_rawEdges[i];
            cout << "from " << from << " to " << to << ": " << path[i] << endl;
        }
        G6.recover();
        assert(std::get<0>(G6.isPossible()));
        auto [maxflow, cost3] = G6.maxFlow();
        cout << "max flow from 2 to 3: " << maxflow << endl;
        cout << "meanwhile min cost: " << cost3 << endl;
        path = G6.getPath();
        for (uint32_t i = 0; i < path.size(); i++) {
            auto [from, to, cap, _cost] = G6.m_sol.m_rawEdges[i];
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
min cost: 2200
from 3 to 1: 300
from 3 to 2: 200
from 1 to 2: 200
from 1 to 0: 100
from 0 to 2: 100

bound cost flow graph with no source and target:
min cost:2800
from 0 to 2: 200
from 3 to 0: 200
from 3 to 1: 100
from 1 to 2: 100
from 2 to 3: 300

bound cost flow graph with source and target:
flow from 2 to 3: 200
meanwhile min cost: 1800
from 0 to 2: 100
from 3 to 0: 100
from 2 to 1: 0
from 1 to 3: 0
from 2 to 3: 300
min flow from 2 to 3: 100
meanwhile min cost: 2100
from 0 to 2: 200
from 3 to 0: 200
from 2 to 1: 0
from 1 to 3: 0
from 2 to 3: 300
max flow from 2 to 3: 400
meanwhile min cost: 3000
from 0 to 2: 100
from 3 to 0: 100
from 2 to 1: 100
from 1 to 3: 100
from 2 to 3: 400

cost flow graph with negative cost:
flow from 2 to 3: 500
meanwhile min cost: -3400
from 1 to 3: -300
from 2 to 3: -200
from 2 to 1: 0
from 0 to 1: -300
from 2 to 0: -300

bound cost flow graph with source and target and negative cost:
flow from 2 to 3: 300
meanwhile min cost: -3300
from 2 to 0: -200
from 0 to 3: -200
from 1 to 2: -100
from 3 to 1: -100
from 3 to 2: -400
min flow from 2 to 3: 100
meanwhile min cost: -2100
from 2 to 0: -200
from 0 to 3: -200
from 1 to 2: 0
from 3 to 1: 0
from 3 to 2: -300
max flow from 2 to 3: 400
meanwhile min cost: -3000
from 2 to 0: -100
from 0 to 3: -100
from 1 to 2: -100
from 3 to 1: -100
from 3 to 2: -400

```

