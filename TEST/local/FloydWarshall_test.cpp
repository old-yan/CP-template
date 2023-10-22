#include "GRAPH/FloydWarshall.h"
#include "IO/FastIO.h"

int main() {
    // 有向图
    OY::FloydWarshall::Graph<100> G(3);
    // 加边
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(0, 2);
    G.add_edge(2, 1);
    // 求可达性
    G.calc();
    if (G.m_can[0][2]) {
        cout << "0 can reach 2\n";
    } else {
        cout << "0 can't reach 2\n";
    }
    if (G.m_can[2][0]) {
        cout << "2 can reach 0\n";
    } else {
        cout << "2 can't reach 0\n";
    }
}
/*
#输出如下
0 can reach 2
2 can't reach 0

*/