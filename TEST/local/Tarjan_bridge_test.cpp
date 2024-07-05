#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

int main() {
    OY::EBCC::Graph G(5, 5);
    // 加五条边
    G.add_edge(0, 3);
    G.add_edge(0, 4);
    G.add_edge(1, 4);
    G.add_edge(3, 2);
    G.add_edge(4, 3);

    // 单独获取桥边
    cout << "get bridges:\n";
    auto bridges = G.get_bridges();
    for (uint32_t i = 0; i < bridges.size(); i++) {
        uint32_t from = G.m_raw_edges[bridges[i]].m_from;
        uint32_t to = G.m_raw_edges[bridges[i]].m_to;
        cout << "No." << i << " bridge edge: index = " << bridges[i] << ", from " << from << " to " << to << '\n';
    }
    cout << '\n';

    // 单独获取边双连通分量
    cout << "get ebccs:\n";
    auto ebccs = G.get_ebccs();
    for (uint32_t i = 0; i < ebccs.size(); i++) {
        cout << "No." << i << " group:\n";
        for (auto a : ebccs[i]) cout << a << ' ';
        cout << endl;
    }
    cout << '\n';

    // 如果既要获取桥边，也要获取边双
    auto sol = G.calc<true, true>();
    sol.do_for_each_bridge([&](uint32_t index) {
        uint32_t from = G.m_raw_edges[index].m_from;
        uint32_t to = G.m_raw_edges[index].m_to;
        cout << "bridge edge: index = " << index << ", from " << from << " to " << to << '\n';
    });
    auto print_ebcc = [&](uint32_t *first, uint32_t *last) {
        cout << "ebcc:";
        for (auto it = first; it != last; ++it) cout << ' ' << *it;
        cout << endl;
    };
    sol.do_for_each_ebcc(print_ebcc);
}
/*
#输出如下
get bridges:
No.0 bridge edge: index = 2, from 1 to 4
No.1 bridge edge: index = 3, from 3 to 2

get ebccs:
No.0 group:
2 
No.1 group:
1 
No.2 group:
0 3 4 

bridge edge: index = 2, from 1 to 4
bridge edge: index = 3, from 3 to 2
ebcc: 2
ebcc: 1
ebcc: 0 3 4

*/