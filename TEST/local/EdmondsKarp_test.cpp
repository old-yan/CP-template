#include "GRAPH/EdmondsKarp.h"
#include "IO/FastIO.h"

void test() {
    cout << "test of normal flow-network:\n";
    // 普通的最大流
    OY::EK::Graph<int, true> G(4, 5);
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
    OY::EK::BoundGraph<int> G(4, 5);
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
    OY::EK::BoundGraph<int> G(4, 5);
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
/*
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

*/