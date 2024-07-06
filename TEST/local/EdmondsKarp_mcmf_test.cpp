#include "GRAPH/EdmondsKarp_mcmf.h"
#include "IO/FastIO.h"

void test() {
    cout << "test of normal flow-network:\n";
    // 普通的最大流
    OY::EKMCMF::Graph<int, int, true> G(4, 5);
    // 加五条边
    G.add_edge(3, 1, 300, -1);
    G.add_edge(3, 2, 200, -2);
    G.add_edge(1, 2, 200, -3);
    G.add_edge(1, 0, 300, -4);
    G.add_edge(0, 2, 400, -5);
    // 计算最大流
    auto res = G.calc(3, 2);
    cout << "max flow from 3 to 2: " << res.first << endl;
    cout << "meanwhile, min cost from 3 to 2: " << res.second << endl;
    // 输出方案
    G.do_for_flows([&](int i, int flow) {
        auto &&e = G.m_raw_edges[i];
        cout << "No." << i << " edge: from " << e.m_from << " to " << e.m_to << ", flow = " << flow << endl;
    });
    cout << '\n';
}

void test_bound_graph_no_source_target() {
    cout << "test of bound-graph without source and target:\n";
    // 无源汇上下界可行费用流
    OY::EKMCMF::BoundGraph<int, int> G(4, 5);
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
            auto &&e = G.m_graph.m_raw_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });
    } else
        cout << "Impossible\n";
    cout << '\n';
}

void test_bound_graph_with_source_target() {
    cout << "test of bound-graph with source and target:\n";
    // 有源汇上下界最小、最大流
    OY::EKMCMF::BoundGraph<int, int> G(4, 5);
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
            auto &&e = G.m_graph.m_raw_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });

        // 在可行前提下，可以计算最小流和最大流
        auto minflow_cost = G.min_flow();
        cout << "\nmin flow from 2 to 3: " << minflow_cost.first << endl;
        cout << "meanwhile min cost: " << minflow_cost.second << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_raw_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });

        // 须先重置，再计算最大流
        G.clear();
        G.is_possible();
        auto maxflow_cost = G.max_flow();
        cout << "\nmax flow from 2 to 3: " << maxflow_cost.first << endl;
        cout << "meanwhile min cost: " << maxflow_cost.second << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_raw_edges[i];
            cout << "from " << e.m_from << " to " << e.m_to << ": " << flow << endl;
        });
    } else
        cout << "Impossible\n";
    cout << '\n';
}

void test_negative_cycle_graph() {
    cout << "test of negative-cycle-graph:\n";
    // 带负圈的最小费用最大流
    OY::EKMCMF::NegativeCycleGraph<int, int> G(4, 6);
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
        auto &&e = G.m_graph.m_raw_edges[i];
        if (flow >= 0)
            cout << "No." << i << " edge: from " << e.m_from << " to " << e.m_to << ", flow = " << flow << endl;
        else
            cout << "No." << i << " edge: from " << e.m_to << " to " << e.m_from << ", flow = " << -flow << endl;
    });
    cout << '\n';
}

void test_negative_cycle_bound_graph_no_source_target() {
    cout << "test of negative-cycle-bound-graph without source and target:\n";
    // 无源汇带负圈的上下界可行费用流
    OY::EKMCMF::NegativeCycleBoundGraph<int, int> G(4, 6);
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
            auto &&e = G.m_graph.m_raw_edges[i];
            if (flow >= 0)
                cout << "No." << i << " edge: from " << e.m_from << " to " << e.m_to << ", flow = " << flow << endl;
            else
                cout << "No." << i << " edge: from " << e.m_to << " to " << e.m_from << ", flow = " << -flow << endl;
        });
    } else
        cout << "Impossible\n";
    cout << '\n';
}

void test_negative_cycle_bound_graph_with_source_target() {
    cout << "test of negative-cycle-bound-graph with source and target:\n";
    // 有源汇上下界最小费用最小、最大流
    OY::EKMCMF::NegativeCycleBoundGraph<int, int> G(4, 6);
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
            auto &&e = G.m_graph.m_raw_edges[i];
            if (flow >= 0)
                cout << "No." << i << " edge: from " << e.m_from << " to " << e.m_to << ", flow = " << flow << endl;
            else
                cout << "No." << i << " edge: from " << e.m_to << " to " << e.m_from << ", flow = " << -flow << endl;
        });

        // 在可行前提下，可以计算最小流和最大流
        auto minflow_cost = G.min_flow();
        cout << "\nmin flow from 2 to 3: " << minflow_cost.first << endl;
        cout << "meanwhile min cost: " << minflow_cost.second << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_raw_edges[i];
            if (flow >= 0)
                cout << "No." << i << " edge: from " << e.m_from << " to " << e.m_to << ", flow = " << flow << endl;
            else
                cout << "No." << i << " edge: from " << e.m_to << " to " << e.m_from << ", flow = " << -flow << endl;
        });

        // 须先重置，再计算最大流
        G.clear();
        G.is_possible();
        auto maxflow_cost = G.max_flow();
        cout << "\nmax flow from 2 to 3: " << maxflow_cost.first << endl;
        cout << "meanwhile min cost: " << maxflow_cost.second << endl;
        G.do_for_flows([&](int i, int flow) {
            auto &&e = G.m_graph.m_raw_edges[i];
            if (flow >= 0)
                cout << "No." << i << " edge: from " << e.m_from << " to " << e.m_to << ", flow = " << flow << endl;
            else
                cout << "No." << i << " edge: from " << e.m_to << " to " << e.m_from << ", flow = " << -flow << endl;
        });
    } else
        cout << "Impossible\n";
    cout << '\n';
}

int main() {
    test();
    test_bound_graph_no_source_target();
    test_bound_graph_with_source_target();
    test_negative_cycle_graph();
    test_negative_cycle_bound_graph_no_source_target();
    test_negative_cycle_bound_graph_with_source_target();
}
/*
#输出如下
test of normal flow-network:
max flow from 3 to 2: 500
meanwhile, min cost from 3 to 2: -3400
No.0 edge: from 3 to 1, flow = 300
No.1 edge: from 3 to 2, flow = 200
No.2 edge: from 1 to 2, flow = 0
No.3 edge: from 1 to 0, flow = 300
No.4 edge: from 0 to 2, flow = 300

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

test of negative-cycle-graph:
max flow from 3 to 2: 500
meanwhile, min cost from 3 to 2: -1007400
No.0 edge: from 3 to 1, flow = 300
No.1 edge: from 3 to 2, flow = 200
No.2 edge: from 2 to 1, flow = 0
No.3 edge: from 1 to 0, flow = 1300
No.4 edge: from 0 to 2, flow = 300
No.5 edge: from 0 to 1, flow = 1000

test of negative-cycle-bound-graph without source and target:
meanwhile min cost = -1005800
No.0 edge: from 0 to 2, flow = 200
No.1 edge: from 3 to 0, flow = 200
No.2 edge: from 3 to 1, flow = 1100
No.3 edge: from 1 to 2, flow = 100
No.4 edge: from 2 to 3, flow = 300
No.5 edge: from 1 to 3, flow = 1000

test of negative-cycle-bound-graph with source and target:
flow from 2 to 3: 100
meanwhile min cost: -1006300
No.0 edge: from 0 to 2, flow = 200
No.1 edge: from 3 to 0, flow = 200
No.2 edge: from 3 to 1, flow = 1100
No.3 edge: from 1 to 2, flow = 100
No.4 edge: from 2 to 3, flow = 400
No.5 edge: from 1 to 3, flow = 1000

min flow from 2 to 3: 0
meanwhile min cost: -1005800
No.0 edge: from 0 to 2, flow = 200
No.1 edge: from 3 to 0, flow = 200
No.2 edge: from 3 to 1, flow = 1100
No.3 edge: from 1 to 2, flow = 100
No.4 edge: from 2 to 3, flow = 300
No.5 edge: from 1 to 3, flow = 1000

max flow from 2 to 3: 300
meanwhile min cost: -1005300
No.0 edge: from 0 to 2, flow = 100
No.1 edge: from 3 to 0, flow = 100
No.2 edge: from 3 to 1, flow = 1000
No.3 edge: from 2 to 1, flow = 0
No.4 edge: from 2 to 3, flow = 400
No.5 edge: from 1 to 3, flow = 1000

*/