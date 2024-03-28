#include "GRAPH/Tarjan_scc.h"
#include "IO/FastIO.h"

int main() {
    OY::SCC::Graph G(5, 5);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    G.add_edge(3, 3);
    G.add_edge(3, 1);

    auto sol = G.calc();
    cout << "There are " << sol.group_count() << " groups of SCC\n";
    auto groups = sol.get_groups();
    for (int i = 0; i < groups.size(); i++) {
        cout << "No." << i << " group:\n";
        for (auto a : groups[i]) cout << a << ' ';
        cout << endl;
    }
}
/*
#输出如下
There are 3 groups of SCC
No.0 group:
4 
No.1 group:
3 
No.2 group:
0 1 2 

*/