#include "GRAPH/Bipartite.h"
#include "IO/FastIO.h"

int main() {
    OY::Bipartite::Graph<1000, 1000> G(7, 4);

    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    G.add_edge(6, 5);

    auto res = G.calc();
    // 查看染色结果
    for (int i = 0; i < 7; i++) cout << res.query(i) << " \n"[i == 5];

    if (!res.is_bipartite())
        cout << "It is not bipartite\n";
    else
        cout << "It is bipartite\n";

    if (res.in_same_group(0, 3)) cout << "0 and 3 is in same group\n";

    if (!res.in_same_group(5, 3)) cout << "5 and 3 is not in same group\n";
}
/*
#输出如下
0 1 0 1 2 4
5 It is bipartite
0 and 3 is in same group
5 and 3 is not in same group

*/