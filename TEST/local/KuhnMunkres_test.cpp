#include "GRAPH/KuhnMunkres.h"
#include "IO/FastIO.h"

void test() {
    cout << "prefect match:\n";
    // 建立二分图，求完美匹配
    OY::KM::Graph<int> G(3, -9999);
    // 加五条边，G.add_edge(a,b,value) 表示男孩 a 喜欢女孩 b
    G.add_edge(0, 0, 10);
    G.add_edge(1, 1, 100);
    G.add_edge(2, 2, 1000);
    G.add_edge(0, 1, 200);
    G.add_edge(1, 2, 2000);
    // 求最大权完美匹配
    cout << "max match = " << G.calc() << endl;
    // 看每个男孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "boy " << i << " matches girl " << int(G.find_right(i)) << endl;
    }
    // 看每个女孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "girl " << i << " matches boy " << int(G.find_left(i)) << endl;
    }
    cout << '\n';
}

void test2() {
    cout << "prefect match:\n";
    // 建立二分图，求最大匹配（不要求完美）
    OY::KM::Graph<int> G(3, 0);
    // 加五条边，G.add_edge(a,b,value) 表示男孩 a 喜欢女孩 b
    G.add_edge(0, 0, 10);
    G.add_edge(1, 1, 100);
    G.add_edge(2, 2, 1000);
    G.add_edge(0, 1, 200);
    G.add_edge(1, 2, 2000);
    // 求最大权完美匹配
    cout << "max match = " << G.calc() << endl;
    // 看每个男孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        if (G.query(i, G.find_right(i)) != 0)
            cout << "boy " << i << " matches girl " << int(G.find_right(i)) << endl;
    }
    // 看每个女孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        if (G.query(G.find_left(i), i) != 0)
            cout << "girl " << i << " matches boy " << int(G.find_left(i)) << endl;
    }
    cout << '\n';
}

int main() {
    test();
    test2();
}
/*
#输出如下
prefect match:
max match = 1110
boy 0 matches girl 0
boy 1 matches girl 1
boy 2 matches girl 2
girl 0 matches boy 0
girl 1 matches boy 1
girl 2 matches boy 2

prefect match:
max match = 2200
boy 0 matches girl 1
boy 1 matches girl 2
girl 1 matches boy 0
girl 2 matches boy 1

*/