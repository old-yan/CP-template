#include "GRAPH/TopologicalSort.h"
#include "IO/FastIO.h"

int main() {
    OY::TOPO::Graph G(5, 5);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 4);
    G.add_edge(1, 4);
    G.add_edge(3, 4);

    auto res = G.calc();
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag) {
        cout << "There is cycle in the graph\n";
    } else {
        cout << "There is no cycle in the graph\n";
        sol.trace([](int i) {
            cout << i << ' ';
        });
    }
}
/*
#输出如下
There is no cycle in the graph
0 3 1 2 4 

*/