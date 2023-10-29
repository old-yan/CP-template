### 一、模板类别

​	数据结构：`Dinic` 费用流算法。

### 二、模板功能

​	本模板内含四个图结构，第一个为 `Graph` ，可以解决最小费用最大流问题；第二个为 `BoundGraph` ，可以解决无源汇上下界最小费用可行流、有源汇上下界最小费用最小流、有源汇上下界最小费用最大流问题；第三个为 `NegativeGraph` ，可以解决负费用（包括负圈）情况下的最小费用最大流问题；第四个为 `NegativeBoundGraph` ，可以解决负费用（包括负圈）情况下的无源汇上下界最小费用可行流、有源汇上下界最小费用最小流、有源汇上下界最小费用最大流问题。

​	下文中， 先介绍 `Graph` 的接口，再介绍其他类的接口。

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename Tp` ，表示容量、流量的类型。

   模板参数 `typename Fp` ，表示费用的类型。费用类型和流量类型相乘后，仍为费用类型。

   模板参数 `size_type MAX_VERTEX` ，表示最大结点数。

   模板参数 `size_type MAX_EDGE` ，表示最大边数。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   构造参数 `size_type edge_cnt` ，表示边数。默认为 `0` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   `Dinic` 算法处理的问题为费用流网络的最小费用最大流问题。

   本数据结构不能处理带负权的边。
   
   本数据结构可以接受重边和自环。


#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

   输入参数 `size_type edge_cnt` ，表示边数。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type from`​ ，表示有向边的起点编号。

   输入参数 `size_type to` ，表示有向边的终点编号。

   输入参数 `const Tp &cap` ，表示有向边的边容量。

   输入参数 `const Fp &cost` ，表示有向边上单位流量的费用。

2. 时间复杂度

   $O(1)$ 。


#### 4.计算最小费用最大流(calc)

1. 数据类型

   输入参数 `size_type source` ，表示源点。

   输入参数 `size_type target` ，表示汇点。

   输入参数 `const Tp &infinite_cap` ，表示无穷大容量，默认为 `Tp` 类型最大值的一半。

   输入参数 `const Fp &infinite_cost` ，表示无穷大费用，默认为 `Fp` 类型最大值的一半。

   返回类型 `std::pair<Tp, Fp>` ，前者表示从源到汇的最大流，后者表示在最大流前提下的最小费用。

2. 时间复杂度

   $O(n\cdot m\cdot f)$ ，此处 `f` 指最大流。
   
3. 备注

   约定俗成的，在涉及费用流题目中，不会卡 `Dinic` 算法。

#### 5.清空(clear)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本方法用于在计算完一次最小费用最大流之后，更换源点、汇点跑最小费用最大流时使用。

   本方法并不会清空图的数据，只会清空上一次跑最小费用最大流时在图里造成的影响。

#### 6.对找到的流调用回调(do_for_flows)

1. 数据类型

   输入参数 `Callback &&call` ，表示要对找到的流调用的回调。

2. 时间复杂度

    $O(n+m)$ 。

3. 备注

   本方法用于对找到的每条边的流量调用回调。

   回调函数有两个参数，参数一为边的编号，参数二为边的流量（而非容量）。

#### 7.设置源汇(set)

1. 数据类型

   输入参数 `size_type source` ，表示源点。默认为 `-1` ，表示不设置源点，启用虚拟源点。

   输入参数 `size_type target` ，表示汇点。默认为 `-1` ，表示不设置汇点，启用虚拟汇点。

   输入参数 `const Tp &infinite_cap` ，表示无穷大容量，默认为 `Tp` 类型最大值的一半。

   输入参数 `const Tp &infinite_cost` ，表示无穷大费用，默认为 `Fp` 类型最大值的一半。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法仅用于 `BoundGraph` 、`NegativeGraph` 、 `NegativeBoundGraph`  中。

   当未设置源汇点时，不存在最小流、最大流概念，只能求可行流。
   
   **注意：**
   
   在 `NegativeGraph` 中，本方法的 `source` 和 `target` 参数无默认值，也不允许不设置源汇。

#### 8.计算是否有可行流(is_possible)

1. 数据类型

   返回类型 `std::tuple<Tp, Fp, bool>` ，第一部分表示求出的可行流大小，第二部分表示在当前流情况下的最小费用，第三部分表示是否有可行流。

2. 时间复杂度

   $O(n\cdot m\cdot f)$ 。

3. 备注

   本方法仅用于 `BoundGraph` 、 `NegativeBoundGraph`  中。

   当没有可行流时，返回值的第一部分无意义。

   当未设置源汇点时，返回值的第一部分无意义。

   仅当设置源汇点，且有可行流时，返回值第一部分有意义。

#### 9.计算最小流(min_flow)

1. 数据类型

   返回类型 `std::pair<Tp, Fp>` ，前者表示从源到汇的最小流，后者表示在最小流前提下的最小费用。

2. 时间复杂度

   $O(n\cdot m\cdot f)$ 。

3. 备注

   本方法仅用于 `BoundGraph` 、 `NegativeBoundGraph`  中。

   仅当设置源汇点，且有可行流时，本方法有意义。

   调用本方法前，必须先调用 `is_possible` 方法。

#### 10.计算最大流(max_flow)

1. 数据类型

   返回类型 `std::pair<Tp, Fp>` ，前者表示从源到汇的最大流，后者表示在最大流前提下的最小费用。

2. 时间复杂度

   $O(n\cdot m\cdot f)$ 。

3. 备注

   本方法仅用于 `BoundGraph` 、 `NegativeBoundGraph`  中。

   仅当设置源汇点，且有可行流时，本方法有意义。

   调用本方法前，必须先调用 `is_possible` 方法。

### 三、模板示例

```c++
#include "GRAPH/Dinic_mcmf.h"
#include "IO/FastIO.h"

void test() {
    cout << "test of normal flow-network:\n";
    // 普通的最大流
    OY::DINICMCMF::Graph<int, int, 1000, 1000> G(4, 5);
    // 加五条边
    G.add_edge(3, 1, 300, 1);
    G.add_edge(3, 2, 200, 2);
    G.add_edge(1, 2, 200, 3);
    G.add_edge(1, 0, 300, 4);
    G.add_edge(0, 2, 400, 5);
    // 计算最大流
    auto res = G.calc(3, 2);
    cout << "max flow from 3 to 2: " << res.first << endl;
    cout << "meanwhile, min cost from 3 to 2: " << res.second << endl;
    // 输出方案
    G.do_for_flows([&](int i, int flow) {
        auto &&e = G.m_edges[i];
        cout << "No." << i << " edge: from " << e.m_from << " to " << e.m_to << ", flow = " << flow << endl;
    });
    cout << '\n';
}

void test_bound_graph_no_source_target() {
    cout << "test of bound-graph without source and target:\n";
    // 无源汇上下界可行费用流
    OY::DINICMCMF::BoundGraph<int, int, 1000, 1000> G(4, 5);
    // 加五条边，设置最小流量和最大流量
    G.add_edge(0, 2, 100, 200, 1);
    G.add_edge(3, 0, 100, 300, 2);
    G.add_edge(3, 1, 0, 300, 3);
    G.add_edge(1, 2, 0, 100, 4);
    G.add_edge(2, 3, 300, 400, 5);
    // 无参调用 set，表示无源汇
    G.set();
    // 先查看是否可行
    auto res = G.is_possible();
    if (std::get<2>(res)) {
        // 在当前流量下，最小费用
        cout << "meanwhile min cost = " << std::get<1>(res) << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });
    } else
        cout << "Impossible\n";
    cout << '\n';
}

void test_bound_graph_with_source_target() {
    cout << "test of bound-graph with source and target:\n";
    // 有源汇上下界最小、最大流
    OY::DINICMCMF::BoundGraph<int, int, 1000, 1000> G(4, 5);
    // 加五条边，设置最小流量和最大流量
    G.add_edge(0, 2, 100, 200, 1);
    G.add_edge(3, 0, 100, 300, 2);
    G.add_edge(3, 1, 0, 300, 3);
    G.add_edge(1, 2, 0, 100, 4);
    G.add_edge(2, 3, 300, 400, 5);
    // 有参调用 set，设置源汇
    G.set(2, 3);
    // 先查看是否可行
    auto res = G.is_possible();
    if (std::get<2>(res)) {
        cout << "flow from 2 to 3: " << std::get<0>(res) << endl;
        cout << "meanwhile min cost: " << std::get<1>(res) << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });

        // 在可行前提下，可以计算最小流和最大流
        auto minflow_cost = G.min_flow();
        cout << "\nmin flow from 2 to 3: " << minflow_cost.first << endl;
        cout << "meanwhile min cost: " << minflow_cost.second << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });

        // 须先重置，再计算最大流
        G.clear();
        G.is_possible();
        auto maxflow_cost = G.max_flow();
        cout << "\nmax flow from 2 to 3: " << maxflow_cost.first << endl;
        cout << "meanwhile min cost: " << maxflow_cost.second << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });
    } else
        cout << "Impossible\n";
    cout << '\n';
}

void test_negative_graph() {
    cout << "test of negative-graph:\n";
    // 带负圈的最小费用最大流
    OY::DINICMCMF::NegativeGraph<int, int, 1000, 1000> G(4, 6);
    // 加六条边
    G.add_edge(3, 1, 300, -1);
    G.add_edge(3, 2, 200, -2);
    G.add_edge(1, 2, 200, -3);
    G.add_edge(1, 0, 1300, -4);
    G.add_edge(0, 2, 400, -5);
    G.add_edge(0, 1, 1000, -1000);
    // 设置源汇
    G.set(3, 2);
    // 计算最大流
    auto res = G.calc();
    cout << "max flow from 3 to 2: " << res.first << endl;
    cout << "meanwhile, min cost from 3 to 2: " << res.second << endl;
    // 输出方案，显然图中与主路径无关的流也贡献了费用
    G.do_for_flows([&](int i, int flow) {
        auto &&e = G.m_graph.m_edges[i];
        cout << "No." << i << " edge: from " << e.m_from << " to " << e.m_to << ", flow = " << flow << endl;
    });
    cout << '\n';
}

void test_negative_bound_graph_no_source_target() {
    cout << "test of negative-bound-graph without source and target:\n";
    // 无源汇带负圈的上下界可行费用流
    OY::DINICMCMF::NegativeBoundGraph<int, int, 1000, 1000> G(4, 6);
    // 加六条边，设置最小流量和最大流量
    G.add_edge(0, 2, 100, 200, -1);
    G.add_edge(3, 0, 100, 300, -2);
    G.add_edge(3, 1, 0, 1300, -3);
    G.add_edge(1, 2, 0, 100, -4);
    G.add_edge(2, 3, 300, 400, -5);
    G.add_edge(1, 3, 0, 1000, -1000);
    // 无参调用 set，表示无源汇
    G.set();
    // 先查看是否可行
    auto res = G.is_possible();
    if (std::get<2>(res)) {
        // 在当前流量下，最小费用
        cout << "meanwhile min cost = " << std::get<1>(res) << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });
    } else
        cout << "Impossible\n";
    cout << '\n';
}

void test_negative_bound_graph_with_source_target() {
    cout << "test of negative-bound-graph with source and target:\n";
    // 有源汇上下界最小费用最小、最大流
    OY::DINICMCMF::NegativeBoundGraph<int, int, 1000, 1000> G(4, 6);
    // 加六条边，设置最小流量和最大流量
    G.add_edge(0, 2, 100, 200, -1);
    G.add_edge(3, 0, 100, 300, -2);
    G.add_edge(3, 1, 0, 1300, -3);
    G.add_edge(1, 2, 0, 100, -4);
    G.add_edge(2, 3, 300, 400, -5);
    G.add_edge(1, 3, 0, 1000, -1000);
    // 有参调用 set，设置源汇
    G.set(2, 3);
    // 先查看是否可行
    auto res = G.is_possible();
    if (std::get<2>(res)) {
        cout << "flow from 2 to 3: " << std::get<0>(res) << endl;
        cout << "meanwhile min cost: " << std::get<1>(res) << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });

        // 在可行前提下，可以计算最小流和最大流
        auto minflow_cost = G.min_flow();
        cout << "\nmin flow from 2 to 3: " << minflow_cost.first << endl;
        cout << "meanwhile min cost: " << minflow_cost.second << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });

        // 须先重置，再计算最大流
        G.clear();
        G.is_possible();
        auto maxflow_cost = G.max_flow();
        cout << "\nmax flow from 2 to 3: " << maxflow_cost.first << endl;
        cout << "meanwhile min cost: " << maxflow_cost.second << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });
    } else
        cout << "Impossible\n";
    cout << '\n';
}

int main() {
    test();
    test_bound_graph_no_source_target();
    test_bound_graph_with_source_target();
    test_negative_graph();
    test_negative_bound_graph_no_source_target();
    test_negative_bound_graph_with_source_target();
}
```

```
#输出如下
test of normal flow-network:
max flow from 3 to 2: 500
meanwhile, min cost from 3 to 2: 2200
No.0 edge: from 3 to 1, flow = 300
No.1 edge: from 3 to 2, flow = 200
No.2 edge: from 1 to 2, flow = 200
No.3 edge: from 1 to 0, flow = 100
No.4 edge: from 0 to 2, flow = 100

test of bound-graph without source and target:
meanwhile min cost = 2800
from 0 to 2: 200
from 3 to 0: 200
from 3 to 1: 100
from 1 to 2: 100
from 2 to 3: 300

test of bound-graph with source and target:
flow from 2 to 3: 200
meanwhile min cost: 1800
from 0 to 2: 100
from 3 to 0: 100
from 3 to 1: 0
from 1 to 2: 0
from 2 to 3: 300

min flow from 2 to 3: 0
meanwhile min cost: 2800
from 0 to 2: 200
from 3 to 0: 200
from 3 to 1: 100
from 1 to 2: 100
from 2 to 3: 300

max flow from 2 to 3: 300
meanwhile min cost: 2300
from 0 to 2: 100
from 3 to 0: 100
from 3 to 1: 0
from 1 to 2: 0
from 2 to 3: 400

test of negative-graph:
max flow from 3 to 2: 500
meanwhile, min cost from 3 to 2: -1007400
No.0 edge: from 1 to 3, flow = -300
No.1 edge: from 2 to 3, flow = -200
No.2 edge: from 2 to 1, flow = 0
No.3 edge: from 0 to 1, flow = -1300
No.4 edge: from 2 to 0, flow = -300
No.5 edge: from 1 to 0, flow = -1000

test of negative-bound-graph without source and target:
meanwhile min cost = -1005800
from 2 to 0: 200
from 0 to 3: 200
from 1 to 3: 1100
from 2 to 1: 100
from 3 to 2: 300
from 3 to 1: 1000

test of negative-bound-graph with source and target:
flow from 2 to 3: 100
meanwhile min cost: -1006300
from 2 to 0: 200
from 0 to 3: 200
from 1 to 3: 1100
from 2 to 1: 100
from 3 to 2: 400
from 3 to 1: 1000

min flow from 2 to 3: 0
meanwhile min cost: -1005800
from 2 to 0: 200
from 0 to 3: 200
from 1 to 3: 1100
from 2 to 1: 100
from 3 to 2: 300
from 3 to 1: 1000

max flow from 2 to 3: 300
meanwhile min cost: -1005300
from 2 to 0: 100
from 0 to 3: 100
from 1 to 3: 1000
from 2 to 1: 0
from 3 to 2: 400
from 3 to 1: 1000

```

