#include "GRAPH/Kruskal.h"
#include "IO/FastIO.h"

int main() {
    OY::Kruskal::Graph<int> G(4, 5);
    G.add_edge(0, 1, 100);
    G.add_edge(1, 2, 20);
    G.add_edge(2, 3, 40);
    G.add_edge(0, 2, 110);
    G.add_edge(2, 3, 30);

    auto res = G.calc<true>();
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag) {
        cout << "There is no MST\n";
    } else {
        cout << "There is MST\n";
        cout << "MST total cost: " << sol.total_cost() << endl;
        cout << "used edges:\n";
        sol.do_for_used_edges([&](int index, int from, int to, int cost) {
            cout << "use No." << index << " edge, from " << from << " to " << to << ", cost = " << cost << endl;
        });
    }
}
/*
#输出如下
There is MST
MST total cost: 150
used edges:
use No.1 edge, from 1 to 2, cost = 20
use No.4 edge, from 2 to 3, cost = 30
use No.0 edge, from 0 to 1, cost = 100

*/