#include "GRAPH/Psuedotree_ig.h"
#include "IO/FastIO.h"

/*
[P2661 [NOIP2015 提高组] 信息传递](https://www.luogu.com.cn/problem/P2661)
*/
/**
 * 环套树找最小环
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::PsuedoIG::Graph<bool> G(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t to;
        cin >> to;
        G.set_edge(i, to - 1);
    }
    G.prepare();
    uint32_t ans = 0x3f3f3f3f;
    for (uint32_t id = 0; id != G.cycle_count(); id++) {
        ans = std::min(ans, G.get_cycle_info(id).size());
    }
    cout << ans;
}