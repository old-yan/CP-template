#include "DS/Queue.h"
#include "GRAPH/Psuedotree_ug.h"
#include "IO/FastIO.h"

/*
[#126. 【NOI2013】快餐店](https://uoj.ac/problem/126)
*/
/**
 * 快餐要么出现在树边上，要么出现在环上
 * 枚举每个边
 * 可以当基环树重心模板
 */

static constexpr uint32_t N = 100000;
uint64_t top2[N][2];
int main() {
    uint32_t n;
    cin >> n;
    OY::PsuedoUG::Graph<uint32_t, uint64_t, false> G(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t a, b, len;
        cin >> a >> b >> len;
        G.add_edge(a - 1, b - 1, len);
    }
    G.prepare();

    // 计算环上每个点的最深树点，记录每个点由各个孩子贡献的最长链前两名
    for (uint32_t i = 0; i != n; i++)
        if (!G.get_vertex_info(i).step_to_cycle()) {
            auto dfs = [&](auto &&self, uint32_t a) mutable -> void {
                G.do_for_out_edges(a, [&](uint32_t to, uint32_t dis) {
                    self(self, to);
                    if (top2[to][0] + dis > top2[a][0])
                        top2[a][1] = top2[a][0], top2[a][0] = top2[to][0] + dis;
                    else if (top2[to][0] + dis > top2[a][1])
                        top2[a][1] = top2[to][0] + dis;
                });
            };
            dfs(dfs, i);
        }

    // 本题只有一个环
    auto cycle = G.get_cycle_info(0);
    // 由于答案可能带有 0.5，所以按照二倍存储答案
    uint64_t ans = 0x3f3f3f3f3f3f3f3f;
    auto get_min_max = [&](uint64_t l, uint64_t m, uint64_t r) {
        if (l + m <= r)
            return r * 2;
        else if (r + m <= l)
            return l * 2;
        else
            return l + m + r;
    };

    auto solve_on_cycle = [&] {
        auto get_pos = [&](uint32_t index) {
            return index < cycle.size() ? cycle.position(index) : cycle.length() + cycle.position(index - cycle.size());
        };
        // 假设断开 cycle[i] 和 cycle[i+1] 之间的边
        // cycle[i] 由左侧路径供餐
        // cycle[i+1] 向右侧路径供餐
        // 假定最佳快餐点位于 cycle[j] 和 cycle[j+1] 之间
        // 即 cycle[i+1~j] 处的树边上的点都由快餐点出发，经由 cycle[j] 分发
        // 即 cycle[j+1~i] 处的树边上的点都由快餐点出发，经由 cycle[j+1] 分发
        // 设 cycle[j] 和 cycle[j+1] 之间距离为 dis
        // 一定有 max_dis(j -> i+1~j) + dis > max_dis(j+1 -> j+1~i)
        // 一定有 max_dis(j+1 -> j+1~i) + dis >= max_dis(j -> i+1~j)
        // 通过这个规则可以唯一确定 j 和 j+1 的位置
        // 注意，下标是循环往复的

        // 计算 j+1~i -> j+1 的最大距离，即 pos[x]-pos[j+1]+off_cycle[x]
        // 所以 q_l 维护 off_cycle[x]+pos[x]
        // 注意，j+1 一开始位于 [0, cycle.size())，之后 j+1 可能会过渡到 [cycle.size(),cycle.size()*2)。x 要在圈数上作相应调整

        // 计算 i+1~j -> j 的最大距离，即 pos[j]-pos[x]+off_cycle[x]
        // 所以 q_r 维护 off_cycle[x]-pos[x]
        // 注意，j 一开始位于 [0, cycle.size())，之后 j 可能会过渡到 [cycle.size(),cycle.size()*2)。x 要在圈数上作相应调整

        using Queue = OY::MaxQueue<int64_t>;
        Queue q_l, q_r;
        for (uint32_t i = 2; i != cycle.size(); i++) q_l.push(top2[cycle[i]][0] + cycle.position(i));
        q_l.push(top2[cycle[0]][0] + cycle.length() + cycle.position(0));
        q_r.push(top2[cycle[1]][0] - cycle.position(1));
        uint32_t j = 1;
        while (j + 1 != cycle.size() && q_r.query_all() + get_pos(j + 1) < q_l.query_all() - get_pos(j + 1)) {
            j++;
            q_l.pop();
            q_r.push(top2[cycle[j]][0] - cycle.position(j));
        }
        for (uint32_t i = 0; i != cycle.size(); i++) {
            uint64_t l_dis = q_l.query_all() - get_pos(j + 1);
            uint64_t m_dis = get_pos(j + 1) - get_pos(j);
            uint64_t r_dis = q_r.query_all() + get_pos(j);
            ans = std::min(ans, get_min_max(l_dis, m_dis, r_dis));
            if (i + 1 != cycle.size()) {
                q_r.pop();
                q_l.push(top2[cycle[i + 1]][0] + cycle.length() + cycle.position(i + 1));
                while (j + 1 != cycle.size() + i + 1 && (q_r.empty() || q_r.query_all() + get_pos(j + 1) < q_l.query_all() - get_pos(j + 1))) {
                    j++;
                    q_l.pop();
                    q_r.push(top2[cycle[j < cycle.size() ? j : j - cycle.size()]][0] - get_pos(j));
                }
            }
        }
    };
    auto solve_off_cycle = [&] {
        // cycle[i] 既向左可以往一些点送餐，也向右往一些点送餐
        // 随着 i 增大，向左的点和向右的点的分界点在右移
        // 右侧点到 cycle[i] 的距离是，off_cycle[x]+pos[x]-pos[i]，所以需要维护 off_cycle[x]+pos[x] 的最大值
        // 左侧点到 cycle[i] 的距离是，off_cycle[x]+pos[i]-pos[x]，所以需要维护 off_cycle[x]-pos[x] 的最大值
        using Queue = OY::MaxQueue<int64_t>;
        Queue q_l, q_r;
        uint32_t mid;
        // 确定 mid 初始位置，[1,mid] 都从右边送餐，[mid+1,cycle.size()-1] 都从左边送餐
        for (mid = 0; mid + 1 < cycle.size() && cycle.position(mid + 1) < cycle.length() - cycle.position(mid + 1); mid++) {}
        // 根据 mid 维护 q_l q_r
        for (uint32_t i = mid + 1; i < cycle.size(); i++) q_l.push(top2[cycle[i]][0] - (cycle.position(i) - cycle.length()));
        for (uint32_t i = 1; i <= mid; i++) q_r.push(top2[cycle[i]][0] + cycle.position(i));
        for (uint32_t i = 0; i != cycle.size(); i++) {
            auto get_pos = [&](uint32_t i) { return i >= cycle.size() ? cycle.length() + cycle.position(i - cycle.size()) : cycle.position(i); };
            // 假如快餐店设置在 cycle[i] 的环外树边上
            uint64_t up_dis = 0;
            if (!q_l.empty()) up_dis = std::max(up_dis, q_l.query_all() + get_pos(i));
            if (!q_r.empty()) up_dis = std::max(up_dis, q_r.query_all() - get_pos(i));
            auto dfs = [&](auto &&self, uint32_t a, uint64_t up_dis) -> void {
                G.do_for_out_edges(a, [&](uint32_t to, uint32_t dis) {
                    uint64_t other_dis = std::max(top2[to][0] + dis == top2[a][0] ? top2[a][1] : top2[a][0], up_dis);
                    ans = std::min(ans, get_min_max(other_dis, dis, top2[to][0]));
                    self(self, to, other_dis + dis);
                });
            };
            dfs(dfs, cycle[i], up_dis);
            // 随着 i 增大，分界点 mid 右移。注意 mid 可能越过 cycle.size()
            if (i + 1 < cycle.size()) {
                q_l.push(top2[cycle[i]][0] - get_pos(i));
                if (!q_r.empty())
                    q_r.pop();
                else
                    q_l.pop(), mid++;
                while (true) {
                    uint64_t dis_l = (mid + 1 < cycle.size() ? cycle.length() + cycle.position(i + 1) - cycle.position(mid + 1) : cycle.position(i + 1) - cycle.position(mid + 1 - cycle.size()));
                    uint64_t dis_r = (mid + 1 < cycle.size() ? cycle.position(mid + 1) - cycle.position(i + 1) : cycle.length() + cycle.position(mid + 1 - cycle.size()) - get_pos(i + 1));
                    if (dis_r > dis_l) break;
                    mid++;
                    q_l.pop();
                    q_r.push(mid < cycle.size() ? top2[cycle[mid]][0] + cycle.position(mid) : top2[cycle[mid - cycle.size()]][0] + cycle.length() + cycle.position(mid - cycle.size()));
                }
            }
        }
    };
    solve_on_cycle();
    solve_off_cycle();
    if (ans % 2)
        cout << ans / 2 << ".5";
    else
        cout << ans / 2 << ".0";
}