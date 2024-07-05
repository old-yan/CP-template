#include "GRAPH/Bipartite.h"
#include "IO/FastIO.h"

int main() {
    OY::Bipartite::Graph G(7, 4);

    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    G.add_edge(6, 5);

    auto res = G.calc();
    auto &&sol = res.first;
    bool flag = res.second;
    // 查看染色结果
    for (int i = 0; i < 7; i++) cout << sol.query(i) << " \n"[i == 5];

    if (!flag)
        cout << "It is not bipartite\n";
    else
        cout << "It is bipartite\n";

    if (sol.in_same_group(0, 3)) cout << "0 and 3 is in same group\n";

    if (!sol.in_same_group(5, 3)) cout << "5 and 3 is not in same group\n";
}
/*
#输出如下
0 1 0 1 2 4
5 It is bipartite
0 and 3 is in same group
5 and 3 is not in same group

*/