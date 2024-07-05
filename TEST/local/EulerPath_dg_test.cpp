#include "GRAPH/EulerPath_dg.h"
#include "IO/FastIO.h"

int main() {
    OY::EulerPathDG::Graph G(5, 5);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 4);
    G.add_edge(4, 1);
    G.add_edge(1, 3);

    auto sol = G.calc(0);

    if (sol.is_Euler_graph()) {
        cout << "Graph is EulerGraph\n";
        sol.trace([](int index, int from, int to) {
            cout << "No." << index << " edge, from " << from << " to " << to << endl;
        });
    } else if (sol.is_half_Euler_graph()) {
        cout << "Graph is half EulerGraph\n";
        sol.trace([](int index, int from, int to) {
            cout << "No." << index << " edge, from " << from << " to " << to << endl;
        });
    } else {
        cout << "Graph isn't EulerGraph\n";
    }
}
/*
#输出如下
Graph is half EulerGraph
No.0 edge, from 0 to 1
No.1 edge, from 1 to 2
No.2 edge, from 2 to 4
No.3 edge, from 4 to 1
No.4 edge, from 1 to 3

*/