#include "GRAPH/DynamicSPFA.h"
#include "IO/FastIO.h"

void test_run() {
    // 全源最短路
    OY::VectorDynamicSPFAs<OY::DSPFA::AddGroup<int>> G(4);
    // 一次性加入很多边，然后 run 一次
    G.add_edge(0, 1, 100);
    G.add_edge(1, 0, 100);
    G.add_edge(1, 2, 200);
    G.add_edge(1, 3, 300);
    G.add_edge(1, 2, 10);
    G.add_edge(2, 3, 50);

    G.run();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) cout << (G.query(i, j) == G.infinite() ? -1 : G.query(i, j)) << " \n"[j == 3];
    cout << endl;
}

void test_update_by_edge() {
    // 全源最短路
    OY::VectorDynamicSPFAs<OY::DSPFA::AddGroup<int>> G(4);
    // 也可以每次加入一条边，就立刻更新
    G.update_by_edge(0, 1, 100);
    G.update_by_edge(1, 0, 100);
    G.update_by_edge(1, 2, 200);
    G.update_by_edge(1, 3, 300);
    G.update_by_edge(1, 2, 10);
    G.update_by_edge(2, 3, 50);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) cout << (G.query(i, j) == G.infinite() ? -1 : G.query(i, j)) << " \n"[j == 3];
    cout << endl;
}

void test3() {
    // 单源或多源最短路
    OY::VectorDynamicSPFA<OY::DSPFA::AddGroup<int>> G(4);
    G.add_edge(0, 1, 100);
    G.add_edge(1, 0, 100);
    G.add_edge(1, 2, 200);
    G.add_edge(1, 3, 300);
    G.add_edge(1, 2, 10);
    G.add_edge(2, 3, 50);

    // 设置两个源
    G.set_distance(0, 0);
    G.set_distance(2, 0);
    G.run();

    for (int j = 0; j < 4; j++) cout << (G.query(j) == G.infinite() ? -1 : G.query(j)) << " \n"[j == 3];
    cout << endl;
}

int main() {
    test_run();
    test_update_by_edge();
    test3();
}
/*
#输出如下
0 100 110 160
100 0 10 60
-1 -1 0 50
-1 -1 -1 0

0 100 110 160
100 0 10 60
-1 -1 0 50
-1 -1 -1 0

0 100 0 50

*/