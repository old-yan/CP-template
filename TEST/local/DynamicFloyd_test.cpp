#include "GRAPH/DynamicFloyd.h"
#include "IO/FastIO.h"

void test_run() {
    OY::VectorDynamicFloyd<OY::DFLOYD::AddGroup<int>> G(4);
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
    OY::VectorDynamicFloyd<OY::DFLOYD::AddGroup<int>> G(4);
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

void test_update_by_vertex() {
    OY::VectorDynamicFloyd<OY::DFLOYD::AddGroup<int>> G(4);
    G.add_edge(0, 1, 100);
    G.add_edge(1, 0, 100);
    G.add_edge(1, 2, 200);
    G.add_edge(1, 3, 300);
    G.add_edge(1, 2, 10);
    G.add_edge(2, 3, 50);
    
    // 也可以手动调用“对点更新”
    // 更新顺序可以随便
    G.update_by_vertex(2);
    G.update_by_vertex(0);
    G.update_by_vertex(3);
    G.update_by_vertex(1);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) cout << (G.query(i, j) == G.infinite() ? -1 : G.query(i, j)) << " \n"[j == 3];
    cout << endl;
}

int main() {
    test_run();
    test_update_by_edge();
    test_update_by_vertex();
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

0 100 110 160
100 0 10 60
-1 -1 0 50
-1 -1 -1 0

*/