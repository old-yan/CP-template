#include "GRAPH/BronKerbosch.h"
#include "IO/FastIO.h"

int main() {
    // 无向图
    OY::BK::Graph<uint64_t, 100> G(5);
    // 加边
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(0, 2);
    G.add_edge(3, 1);
    // 求最大团
    cout << "max clique size: " << G.max_clique() << '\n';
    // 求最大独立集
    cout << "max independant set size: " << G.max_independant_set() << '\n';
    // 求字典序最小的最大团方案
    auto max_clique = G.get_max_clique();
    cout << "max clique:\n";
    for (int i = 0; i < max_clique.size(); i++) cout << max_clique[i] << " \n"[i == max_clique.size() - 1];
    // 求字典序最小的最大独立集方案
    auto max_independant_set = G.get_max_independant_set();
    cout << "max independant set:\n";
    for (int i = 0; i < max_independant_set.size(); i++) cout << max_independant_set[i] << " \n"[i == max_independant_set.size() - 1];
    // 遍历所有的团
    cout << "cliques:\n";
    G.enumerate_clique([](const std::vector<uint32_t> &clique) {
        for (int i = 0; i < clique.size(); i++) cout << clique[i] << " \n"[i == clique.size() - 1];
    });
    // 遍历所有的独立集
    cout << "independant sets:\n";
    G.enumerate_independant_set([](const std::vector<uint32_t> &independant_set) {
        for (int i = 0; i < independant_set.size(); i++) cout << independant_set[i] << " \n"[i == independant_set.size() - 1];
    });
}
/*
#输出如下
max clique size: 3
max independant set size: 3
max clique:
0 1 2
max independant set:
0 3 4
cliques:
0
0 1
0 1 2
0 2
1
1 2
1 3
2
3
4
independant sets:
0
0 3
0 3 4
0 4
1
1 4
2
2 3
2 3 4
2 4
3
3 4
4

*/