#include "GRAPH/KthPath.h"
#include "IO/FastIO.h"

void test_directed() {
    // 有向图在不带环的情况下，路径数是有限的
    OY::KPATH::Graph<long long, long long, true> G(7, 9);
    G.add_edge(0, 1, 1);
    G.add_edge(1, 2, 1);
    G.add_edge(2, 3, 1);
    G.add_edge(3, 4, 1);
    G.add_edge(4, 5, 1);
    G.add_edge(5, 6, 1);
    G.add_edge(3, 4, 210);
    G.add_edge(4, 5, 200);
    G.add_edge(4, 5, 220);

    if (!G.calc(0, 6))
        cout << "can't go from 0 to 6\n";
    else
        for (int i = 0;; i++) {
            long long length = G.next();
            if (length > 9999999) break;
            cout << "No." << i << " path length: " << length << endl;
        }
}

void test_undirected() {
    // 如果图中出现了环，就有可能有无穷多的路径
    // 无向图本质上都是带环的有向图
    OY::KPATH::Graph<long long, long long, true> G(2, 2);
    G.add_edge(0, 1, 10);
    G.add_edge(1, 0, 3);

    if (!G.calc(0, 1))
        cout << "can't go from 0 to 1\n";
    else
        for (int i = 0; i < 10; i++) {
            long long length = G.next();
            cout << "No." << i << " path length: " << length << endl;
        }
}

int main() {
    test_directed();
    test_undirected();
}
/*
#输出如下
No.0 path length: 6
No.1 path length: 205
No.2 path length: 215
No.3 path length: 225
No.4 path length: 414
No.5 path length: 434
No.0 path length: 10
No.1 path length: 23
No.2 path length: 36
No.3 path length: 49
No.4 path length: 62
No.5 path length: 75
No.6 path length: 88
No.7 path length: 101
No.8 path length: 114
No.9 path length: 127

*/