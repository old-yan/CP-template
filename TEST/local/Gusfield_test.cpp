#include "GRAPH/Gusfield.h"
#include "IO/FastIO.h"

int main() {
    // 建图
    OY::Gusfield::Graph<int> G(5, 5);
    // 加五条边
    G.add_edge(0, 1, 200);
    G.add_edge(1, 2, 200);
    G.add_edge(3, 1, 300);
    G.add_edge(3, 2, 100);
    G.add_edge(0, 2, 100);
    // 计算
    G.calc();
    // 查询
    cout << "mincut between 0 and 2 is: " << G.query(0, 2) << endl;
    cout << "mincut between 1 and 4 is: " << G.query(1, 4) << endl;
}
/*
#输出如下
mincut between 0 and 2 is: 300
mincut between 1 and 4 is: 0

*/