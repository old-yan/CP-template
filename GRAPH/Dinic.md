### 一、模板类别

​	数据结构：`Dinic` 算法。

​	练习题目：

1. [#115. 无源汇有上下界可行流](https://loj.ac/p/115)
2. [#116. 有源汇有上下界最大流](https://loj.ac/p/116)
3. [#117. 有源汇有上下界最小流](https://loj.ac/p/117)
4. [P3376 【模板】网络最大流](https://www.luogu.com.cn/problem/P3376)
5. [P5192 Zoj3229 Shoot the Bullet|东方文花帖|【模板】有源汇上下界最大流](https://www.luogu.com.cn/problem/P5192)



### 二、模板功能

​	本模板内含两个图结构，第一个为 `Graph` ，可以解决最大流问题；第二个为 `BoundGraph` ，可以解决上下界可行流、上下界最小流、上下界最大流问题。

​	下文中， 先介绍 `Graph` 的接口，再介绍 `BoundGraph` 的接口。

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename FlowType` ，表示容量、流量的类型。

   模板参数 `bool Directed` ，表示是否为有向图。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   构造参数 `size_type edge_cnt` ，表示边数。默认为 `0` 。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Dinic` 算法处理的问题为流网络的最大流问题。

   本数据结构不能处理带负权的边。
   
   本数据结构可以接受重边和自环。

   在常规最大流问题里， `Directed` 设为 `true`， 表示边是有向的；在最小割树问题里， `Directed` 设为 `false` ，表示边是无向的。


#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

   输入参数 `size_type edge_cnt` ，表示边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type from`​ ，表示有向边的起点编号。

   输入参数 `size_type to` ，表示有向边的终点编号。

   输入参数 `FlowType cap` ，表示有向边的边容量。

2. 时间复杂度

   $O(1)$ 。


#### 4.计算最大流(calc)

1. 数据类型

   模板参数 `typename SumType` ，表示最大流结果的类型。默认为 `FlowType` 。
   
   输入参数 `size_type source` ，表示源点。

   输入参数 `size_type target` ，表示汇点。

   输入参数 `FlowType infinite` ，表示无穷大容量，默认为 `FlowType` 类型最大值的一半。

   返回类型 `SumType` ，表示从源到汇的最大流。

2. 时间复杂度

   $O(n^2\cdot m)$ 。
   
3. 备注

   约定俗成的，在涉及费用流题目中，不会卡 `Dinic` 算法。

#### 5.清空(clear)

1. 数据类型

2. 时间复杂度

    $O(n+m)$ 。

3. 备注

   本方法用于在计算完一次最大流之后，更换源点、汇点跑最大流时使用。

   本方法并不会清空图的数据，只会清空上一次跑最大流时在图里造成的影响。

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

   输入参数 `FlowType infinite` ，表示无穷大容量，默认为 `FlowType` 类型最大值的一半。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法仅用于 `BoundGraph` 中。

   当未设置源汇点时，不存在最小流、最大流概念，只能求可行流。

#### 8.计算是否有可行流(is_possible)

1. 数据类型

   返回类型 `std::pair<FlowType, bool>` ，前者表示求出的可行流大小，后者表示是否有可行流。

2. 时间复杂度

   $O(n^2\cdot m)$ 。

3. 备注

   本方法仅用于 `BoundGraph` 中。

   当没有可行流时，返回值的第一部分无意义。

   当未设置源汇点时，返回值的第一部分无意义。

   仅当设置源汇点，且有可行流时，返回值第一部分有意义。

#### 9.计算最小流(min_flow)

1. 数据类型

   返回类型 `FlowType` ，表示从源到汇的最小流。

2. 时间复杂度

   $O(n^2\cdot m)$ 。

3. 备注

   本方法仅用于 `BoundGraph` 中。

   仅当设置源汇点，且有可行流时，本方法有意义。

   调用本方法前，必须先调用 `is_possible` 方法。

#### 10.计算最大流(max_flow)

1. 数据类型

   返回类型 `FlowType` ，表示从源到汇的最大流。

2. 时间复杂度

   $O(n^2\cdot m)$ 。

3. 备注

   本方法仅用于 `BoundGraph` 中。

   仅当设置源汇点，且有可行流时，本方法有意义。

   调用本方法前，必须先调用 `is_possible` 方法。

### 三、模板示例

```c++
#include "GRAPH/Dinic.h"
#include "IO/FastIO.h"

void test() {
    cout << "test of normal flow-network:\n";
    // 普通的最大流
    OY::DINIC::Graph<int, true> G(4, 5);
    // 加五条边
    G.add_edge(3, 1, 300);
    G.add_edge(3, 2, 200);
    G.add_edge(1, 2, 200);
    G.add_edge(1, 0, 300);
    G.add_edge(0, 2, 400);
    // 计算最大流
    cout << "max flow from 3 to 2: " << G.calc(3, 2) << endl;
    // 输出方案
    G.do_for_flows([&](int i, int flow) {
        auto &&e = G.m_raw_edges[i];
        cout << "No." << i << " edge: from " << e.m_from << " to " << e.m_to << ", flow = " << flow << endl;
    });
    cout << '\n';
}

void test_no_source_target() {
    cout << "test of flow-network without source and target:\n";
    // 无源汇可行流
    OY::DINIC::BoundGraph<int> G(4, 5);
    // 加五条边，设置最小流量和最大流量
    G.add_edge(0, 2, 100, 200);
    G.add_edge(3, 0, 100, 300);
    G.add_edge(3, 1, 0, 300);
    G.add_edge(1, 2, 0, 100);
    G.add_edge(2, 3, 300, 400);
    // 无参调用 set，表示无源汇
    G.set();
    // 先查看是否可行
    if (G.is_possible().second) {
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_raw_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });
    } else
        cout << "Impossible\n";
    cout << '\n';
}

void test_with_source_and_target() {
    cout << "test of flow-network with source and target:\n";
    // 建图，计算从 2 到 3 的最小可行流、最大可行流
    OY::DINIC::BoundGraph<int> G(4, 5);
    G.add_edge(0, 2, 100, 200);
    G.add_edge(3, 0, 100, 300);
    G.add_edge(2, 1, 0, 300);
    G.add_edge(1, 3, 0, 100);
    G.add_edge(2, 3, 300, 400);

    // 有参调用，说明有源汇
    G.set(2, 3);
    // 先查看是否可行
    auto res = G.is_possible();
    if (res.second) {
        cout << "possible flow from 2 to 3: " << res.first << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_raw_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });

        cout << "\nminimum flow from 2 to 3: " << G.min_flow() << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_raw_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });

        // 为了计算最大流，我们需要重置图
        G.clear();
        G.is_possible();

        cout << "\nmaximum flow from 2 to 3: " << G.max_flow() << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_raw_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });
    } else
        cout << "Impossible\n";

    cout << '\n';
}

int main() {
    test();
    test_no_source_target();
    test_with_source_and_target();
}
```

```
#输出如下
test of normal flow-network:
max flow from 3 to 2: 500
No.0 edge: from 3 to 1, flow = 300
No.1 edge: from 3 to 2, flow = 200
No.2 edge: from 1 to 2, flow = 200
No.3 edge: from 1 to 0, flow = 100
No.4 edge: from 0 to 2, flow = 100

test of flow-network without source and target:
from 0 to 2: 200
from 3 to 0: 200
from 3 to 1: 100
from 1 to 2: 100
from 2 to 3: 300

test of flow-network with source and target:
possible flow from 2 to 3: 200
from 0 to 2: 100
from 3 to 0: 100
from 2 to 1: 0
from 1 to 3: 0
from 2 to 3: 300

minimum flow from 2 to 3: 100
from 0 to 2: 200
from 3 to 0: 200
from 2 to 1: 0
from 1 to 3: 0
from 2 to 3: 300

maximum flow from 2 to 3: 400
from 0 to 2: 100
from 3 to 0: 100
from 2 to 1: 100
from 1 to 3: 100
from 2 to 3: 400


```

