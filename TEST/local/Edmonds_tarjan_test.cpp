#include "GRAPH/Edmonds_tarjan.h"
#include "IO/FastIO.h"

int main() {
    OY::EdmondsTarjan::Graph<uint32_t> G(5, 6);
    G.add_edge(2, 3, 100);
    G.add_edge(2, 4, 120);
    G.add_edge(0, 2, 10);
    G.add_edge(0, 1, 180);
    G.add_edge(0, 4, 200);
    G.add_edge(4, 1, 50);

    auto res = G.calc<true>(0);
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag) {
        cout << "There is no Tree\n";
    } else {
        cout << "There is Tree\n";
        cout << "Tree total cost: " << sol.total_cost() << endl;
        cout << "used edges:\n";
        sol.do_for_used_edges([&](int index) {
            uint32_t from = G.m_edges[index].m_from, to = G.m_edges[index].m_to;
            auto cost=G.m_edges[index].m_cost;
            cout << "use No." << index << " edge, from " << from << " to " << to << ", cost = " << cost << endl;
        });
    }
}
/*
#输出如下
There is Tree
Tree total cost: 280
used edges:
use No.0 edge, from 2 to 3, cost = 100
use No.1 edge, from 2 to 4, cost = 120
use No.2 edge, from 0 to 2, cost = 10
use No.5 edge, from 4 to 1, cost = 50

*/