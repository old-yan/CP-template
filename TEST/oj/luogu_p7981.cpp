#include "GRAPH/Psuedotree_ig.h"
#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"

/*
[P7981 [JRKSJ R3] system](https://www.luogu.com.cn/problem/P7981)
*/
/**
 * 本题为基环树模板题
 * 假定 f(i)=init_val[i]
 * 我们发现
 * 初始时，a[i]=f(i)
 * 第一次旋转后，a[i]=f(f(i))
 * 第二次旋转后，a[i]=f(f(f(f(i))))
 * 第三次旋转后，a[i]=f(f(f(f(f(f(f(f(i))))))))
 * 第k次旋转后，a[i]=f^pow(2,k)(i)
 */

int main() {
    uint32_t n, k;
    cin >> n >> k;
    OY::PsuedoIG::Graph<bool, uint32_t, true> G(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t val;
        cin >> val;
        G.set_edge(i, --val);
    }
    G.prepare();

    if (k <= 60) {
        for (uint32_t i = 0; i != n; i++)
            cout << G.next(i, uint64_t(1) << k) + 1 << " \n"[i == n - 1];
    } else {
        // next 的步数为 pow(2,k)
        // 一定可以走到环上，计算一下 (pow(2,k)-dis_to_cycle)%cycle_size 就是在环上的等效步数
        auto fast_calc = [&](uint32_t join, uint32_t dis_to_cycle, uint32_t cycle_size) -> uint32_t {
            if (cycle_size == 1)
                return 0;
            else {
                using mint = OY::DynamicModInt32<0>;
                mint::set_mod(cycle_size, false);
                return (mint::raw(2).pow(k) + join - dis_to_cycle).val();
            }
        };
        for (uint32_t i = 0; i != n; i++) {
            auto info = G.get_vertex_info(i);
            auto cycle = G.get_cycle_info(info.cycle_id());
            auto end = fast_calc(info.cycle_join(), info.dis_to_cycle(), cycle.size());
            cout << cycle[end] + 1 << " \n"[i == n - 1];
        }
    }
}