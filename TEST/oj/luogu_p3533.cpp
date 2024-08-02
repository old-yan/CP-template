#include "GRAPH/Psuedotree_ig.h"
#include "IO/FastIO.h"

/*
[P3533 [POI2012] RAN-Rendezvous](https://www.luogu.com.cn/problem/P3533)
*/
/**
 * 本题为基环树模板题
 * 求内向基环树两点移动相遇的位置
 */

int main() {
    uint32_t n, k;
    cin >> n >> k;
    OY::PsuedoIG::Graph<bool, uint32_t, true> G(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        G.set_edge(i, x - 1);
    }
    G.prepare();

    while (k--) {
        uint32_t a, b;
        cin >> a >> b;
        auto info_a = G.get_vertex_info(--a);
        auto info_b = G.get_vertex_info(--b);
        if (info_a.cycle_id() != info_b.cycle_id())
            cout << "-1 -1\n";
        else {
            auto cycle = G.get_cycle_info(info_a.cycle_id());
            if (info_a.cycle_join() == info_b.cycle_join()) {
                auto join = G.join(a, b);
                auto info_join = G.get_vertex_info(join);
                cout << info_a.step_to_cycle() - info_join.step_to_cycle() << ' ' << info_b.step_to_cycle() - info_join.step_to_cycle() << endl;
            } else {
                // 相遇在 a 的入环口
                uint32_t xa = info_a.step_to_cycle();
                uint32_t ya = info_b.step_to_cycle() + (info_a.cycle_join() >= info_b.cycle_join() ? info_a.cycle_join() - info_b.cycle_join() : info_a.cycle_join() + cycle.size() - info_b.cycle_join());
                // 相遇在 b 的入环口
                uint32_t xb = info_a.step_to_cycle() + (info_a.cycle_join() <= info_b.cycle_join() ? info_b.cycle_join() - info_a.cycle_join() : info_b.cycle_join() + cycle.size() - info_a.cycle_join());
                uint32_t yb = info_b.step_to_cycle();
                if (std::max(xa, ya) < std::max(xb, yb))
                    cout << xa << ' ' << ya << endl;
                else if (std::max(xa, ya) > std::max(xb, yb))
                    cout << xb << ' ' << yb << endl;
                else if (std::min(xa, ya) < std::min(xb, yb))
                    cout << xa << ' ' << ya << endl;
                else if (std::min(xa, ya) > std::min(xb, yb))
                    cout << xb << ' ' << yb << endl;
                else
                    cout << xb << ' ' << yb << endl;
            }
        }
    }
}