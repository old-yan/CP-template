#include "GRAPH/Psuedotree_ig.h"
#include "IO/FastIO.h"

/*
[P2921 [USACO08DEC] Trick or Treat on the Farm G](https://www.luogu.com.cn/problem/P2921)
*/
/**
 * 本题为基环树模板题
 * 计算内向基环树一条路走到黑的简单路径，只需要把环外长度和环长相加即可
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::PsuedoIG::Graph<bool> G(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        G.set_edge(i, x - 1);
    }
    G.prepare();
    for (uint32_t i = 0; i != n; i++) {
        auto info = G.get_vertex_info(i);
        auto cycle = G.get_cycle_info(info.cycle_id());
        cout << info.step_to_cycle() + cycle.size() << endl;
    }
}