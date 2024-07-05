#include "GRAPH/Steiner.h"
#include "IO/FastIO.h"

void test_int_void() {
    // 无向图
    OY::STEINER::Graph<int> G(6, 8);
    // 加八条边
    G.add_edge(0, 1, 100);
    G.add_edge(1, 2, 120);
    G.add_edge(2, 0, 110);
    G.add_edge(0, 3, 20);
    G.add_edge(4, 3, 10);
    G.add_edge(4, 2, 50);
    G.add_edge(4, 5, 10);
    G.add_edge(5, 1, 30);
    // 指定三个关键点
    G.set_key(0);
    G.set_key(1);
    G.set_key(2);

    // 计算可能性
    auto res = G.calc_spfa<true>();
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag)
        cout << "There is no Steiner Tree\n";
    else {
        cout << "There is Steiner Tree\n";
        cout << "total cost: " << sol.total_cost() << endl;
        cout << "used edges:\n";
        sol.do_for_used_edges([&](int index) {
            int from = G.m_edges[index].m_from;
            int to = G.m_edges[index].m_to;
            int cost = G.m_edges[index].m_cost;
            cout << "use No." << index << " edge, from " << from << " to " << to << ", cost = " << cost << endl;
        });
    }
}

void test_void_int() {
    // 无向图
    OY::STEINER::Graph<void, int> G(6, 8);
    // 加八条边
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    G.add_edge(0, 3);
    G.add_edge(4, 3);
    G.add_edge(4, 2);
    G.add_edge(4, 5);
    G.add_edge(5, 1);
    // 指定三个关键点
    G.set_key(2);
    G.set_key(3);
    G.set_key(5);
    // 指定每个点的权值
    // 不管怎么选，都会把 2 3 5 选中，所以 2 3 5 的权值是多少并不重要
    // 有两种联通方案：[2,3,5,4] [2,3,5,0,1]
    // 只要把 4 的权值设大一点，就会强迫选后者
    G.set_value(4, 100);
    G.set_value(0, 20);
    G.set_value(1, 50);

    // 计算可能性
    auto res = G.calc_spfa<true>();
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag)
        cout << "There is no Steiner Tree\n";
    else {
        cout << "There is Steiner Tree\n";
        cout << "total cost: " << sol.total_cost() << endl;
        cout << "used edges:\n";
        sol.do_for_used_edges([&](int index) {
            int from = G.m_edges[index].m_from;
            int to = G.m_edges[index].m_to;
            cout << "use No." << index << " edge, from " << from << " to " << to << endl;
        });
    }
}

int main() {
    test_int_void();
    test_void_int();
}
/*
#输出如下
There is Steiner Tree
total cost: 120
used edges:
use No.3 edge, from 0 to 3, cost = 20
use No.4 edge, from 4 to 3, cost = 10
use No.5 edge, from 4 to 2, cost = 50
use No.6 edge, from 4 to 5, cost = 10
use No.7 edge, from 5 to 1, cost = 30
There is Steiner Tree
total cost: 70
used edges:
use No.2 edge, from 2 to 0
use No.0 edge, from 0 to 1
use No.3 edge, from 0 to 3
use No.7 edge, from 5 to 1

*/