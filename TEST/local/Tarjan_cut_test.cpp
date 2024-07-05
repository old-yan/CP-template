#include "GRAPH/Tarjan_cut.h"
#include "IO/FastIO.h"

int main() {
    OY::VBCC::Graph G(6, 5);
    // 加五条边
    G.add_edge(0, 3);
    G.add_edge(0, 5);
    G.add_edge(1, 5);
    G.add_edge(3, 2);
    G.add_edge(5, 3);

    // 单独获取割点
    cout << "get cuts:\n";
    auto cuts = G.get_cuts();
    for (uint32_t i = 0; i < cuts.size(); i++) {
        cout << "No." << i << " cut vertex: index = " << cuts[i] << '\n';
    }
    cout << '\n';

    // 单独获取点双连通分量
    cout << "get vbccs:\n";
    auto vbccs = G.get_vbccs();
    for (uint32_t i = 0; i < vbccs.size(); i++) {
        cout << "No." << i << " group:\n";
        for (auto a : vbccs[i]) cout << a << ' ';
        cout << endl;
    }
    cout << '\n';

    // 如果既要获取割点，也要获取点双
    auto sol = G.calc<true, true>();
    sol.do_for_each_cut([&](uint32_t i) {
        cout << "cut vertex: index = " << i << '\n';
    });
    auto print_vbcc = [&](uint32_t *first, uint32_t *last) {
        cout << "vbcc:";
        for (auto it = first; it != last; ++it) cout << ' ' << *it;
        cout << endl;
    };
    sol.do_for_each_vbcc(print_vbcc);
    cout << '\n';

    /**
     * 一个特别的功能，就是把每个 vbcc 里的边的编号们找出来
     * 每个边一定会被分到且仅被分到一个 vbcc 内部
     * 需要传递两个回调
     * 第一个回调负责处理有边的 vbcc 里的所有的边
     * 第二个回调负责处理没边的 vbcc 里的单独的点（显然没边的情况下 vbcc 里只可能有一个孤立点）
     **/
    uint32_t group_id = 0;
    auto edge_call = [&](uint32_t *first, uint32_t *last) {
        cout << "vbcc group id = " << group_id++ << endl;
        for (auto it = first; it != last; ++it) {
            uint32_t index = *it;
            uint32_t from = G.m_raw_edges[index].m_from, to = G.m_raw_edges[index].m_to;
            cout << "edge index = " << index << ", from " << from << " to " << to << endl;
        }
    };
    auto single_call = [&](uint32_t i) {
        cout << "vbcc group id = " << group_id++ << endl;
        cout << "no edge, single vertex = " << i << endl;
    };
    sol.find_edges(edge_call, single_call, G);
}
/*
#输出如下
get cuts:
No.0 cut vertex: index = 3
No.1 cut vertex: index = 5

get vbccs:
No.0 group:
3 2 
No.1 group:
5 1 
No.2 group:
0 3 5 
No.3 group:
4 

cut vertex: index = 3
cut vertex: index = 5
vbcc: 3 2
vbcc: 5 1
vbcc: 0 3 5
vbcc: 4

vbcc group id = 0
edge index = 3, from 3 to 2
vbcc group id = 1
edge index = 2, from 1 to 5
vbcc group id = 2
edge index = 0, from 0 to 3
edge index = 4, from 5 to 3
edge index = 1, from 0 to 5
vbcc group id = 3
no edge, single vertex = 4

*/