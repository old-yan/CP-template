#include "GRAPH/StoerWagner.h"
#include "IO/FastIO.h"

int main() {
    // 无向图
    OY::StoerWagner::Graph<int> G(5);
    // 加六条边
    G.add_edge(0, 1, 100);
    G.add_edge(0, 2, 20);
    G.add_edge(1, 3, 20);
    G.add_edge(2, 3, 40);
    G.add_edge(4, 3, 20);
    G.add_edge(3, 0, 50);
    // 计算最小割
    cout << "min cut is :" << G.calc<true>() << endl;
    // 输出最小割方案
    cout << "first part:\n";
    for (int i = 0; i < 5; i++)
        if (G.is_chosen(i)) cout << i << " ";
    cout << "\nsecond part:\n";
    for (int i = 0; i < 5; i++)
        if (!G.is_chosen(i)) cout << i << " ";
}
/*
#输出如下
min cut is :20
first part:
4
second part:
0 1 2 3

*/