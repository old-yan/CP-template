#include "IO/FastIO.h"
#include "TREE/LinkTree.h"
#include "TREE/RaySeg.h"

/*
[P3596 [POI2015] MOD](https://www.luogu.com.cn/problem/P3596)
*/
/**
 * 本题为射线线段助手模板题
 */

static constexpr uint32_t N = 500000;
OY::RAYSEG::Diameter<OY::LinkTree::Tree<bool, N>, uint32_t, N> G;
int main() {
    uint32_t n;
    cin >> n;
    OY::LinkTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);

    OY::RAYSEG::Table<decltype(S), uint32_t, N> sol(&S);
    uint32_t min_ans = n, max_ans = 0;
    uint32_t min_up_vertex, min_down_vertex, min_up_dis, min_down_dis;
    uint32_t max_up_vertex, max_down_vertex, max_up_dis, max_down_dis;

    // dfs，在 dfs 的过程中尝试拆掉每条边，然后组装新的树
    auto dfs = [&](auto self, uint32_t a, uint32_t p) -> void {
        S.do_for_each_adj_vertex(a, [&](uint32_t to) {
            if (to != p) {
                auto down = sol.query_down_seg(to);
                auto up = sol.max_ray_seg_except_son(a, to, 1).m_seg;
                // 组装新树，令直径尽量小，那么就要连接两部分的直径的中点
                uint32_t min = std::max({down, up, (down + 1 >> 1) + (up + 1 >> 1) + 1});
                if (min < min_ans) {
                    min_ans = min, min_up_vertex = a, min_down_vertex = to, min_up_dis = up + 1 >> 1, min_down_dis = down + 1 >> 1;
                }
                // 组装新树，令直径尽量大，那么就要连接两部分的直径的端点
                uint32_t max = down + up + 1;
                if (max > max_ans) {
                    max_ans = max, max_up_vertex = a, max_down_vertex = to, max_up_dis = up, max_down_dis = down;
                }
                self(self, to, a);
            }
        });
    };
    dfs(dfs, 0, -1);

    auto get_diameter_middle = [&](uint32_t source, uint32_t ban) {
        auto filter = [&](uint32_t a) { return a != ban; };
        std::vector<uint32_t> path;
        auto call = [&](uint32_t a) { path.push_back(a); };
        OY::RAYSEG::Diameter<decltype(S), uint32_t, N>::solve(&S, source, filter, call);
        return path[path.size() / 2];
    };

    // 求出分割后，上侧和下侧的直径中点
    uint32_t min_up_middle = get_diameter_middle(min_up_vertex, min_down_vertex);
    uint32_t min_down_middle = get_diameter_middle(min_down_vertex, min_up_vertex);
    cout << min_ans << ' ' << min_up_vertex + 1 << ' ' << min_down_vertex + 1 << ' ' << min_up_middle + 1 << ' ' << min_down_middle + 1 << endl;

    auto get_diameter_end = [&](uint32_t source, uint32_t ban) {
        auto filter = [&](uint32_t a) { return a != ban; };
        uint32_t res;
        auto call = [&](uint32_t a) { res = a; };
        OY::RAYSEG::Diameter<decltype(S), uint32_t, N>::solve(&S, source, filter, call);
        return res;
    };

    // 求出分割后，上侧和下侧的直径端点
    uint32_t max_up_end = get_diameter_end(max_up_vertex, max_down_vertex);
    uint32_t max_down_end = get_diameter_end(max_down_vertex, max_up_vertex);
    cout << max_ans << ' ' << max_up_vertex + 1 << ' ' << max_down_vertex + 1 << ' ' << max_up_end + 1 << ' ' << max_down_end + 1 << endl;
}
