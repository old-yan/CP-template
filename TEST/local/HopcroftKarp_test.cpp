#include "GRAPH/HopcroftKarp.h"
#include "IO/FastIO.h"

int main() {
    // 建立二分图
    OY::HK::Graph G(3, 5);
    // 加五条边，G.addEdge(a,b) 表示男孩 a 喜欢女孩 b
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 1);
    G.add_edge(2, 2);
    G.add_edge(1, 1);
    // 求最大匹配
    cout << "max match = " << G.calc() << endl;
    // 看每个男孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "boy " << i << " matches girl " << int(G.find_right(i)) << endl;
    }
    // 看每个女孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "girl " << i << " matches boy " << int(G.find_left(i)) << endl;
    }
}
/*
#输出如下
max match = 2
boy 0 matches girl 1
boy 1 matches girl 2
boy 2 matches girl -1
girl 0 matches boy -1
girl 1 matches boy 0
girl 2 matches boy 1

*/