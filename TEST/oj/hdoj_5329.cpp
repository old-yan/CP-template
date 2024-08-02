#include "DS/LinkBucket.h"
#include "GRAPH/Psuedotree_ug.h"
#include "IO/FastIO.h"

/*
[Question for the Leader](https://acm.hdu.edu.cn/showproblem.php?pid=5329)
*/
/**
 * 图结构为大小为 n 的基环树
 * 枚举 n 的因子 k，在环外树上做树形 dp 检验 k 是否可以
 */

int main() {
    uint32_t n;
    while (cin >> n) {
        OY::PsuedoUG::Graph<bool> G(n);
        for (uint32_t i = 0; i != n; i++) {
            uint32_t to;
            cin >> to;
            G.add_edge(i, to - 1);
        }
        G.prepare();
        // 预处理一下环上的环外树的子树大小
        auto cycle = G.get_cycle_info(0);
        OY::LBC::LinkBucket<uint32_t> buckets(cycle.size(), n);
        for (uint32_t i = 0; i != cycle.size(); i++) {
            auto dfs = [&](auto &&self, uint32_t a) -> uint32_t {
                uint32_t size = 1;
                G.do_for_out_edges(a, [&](uint32_t to) {
                    size += self(self, to);
                });
                buckets[i].push_front(size);
                return size;
            };
            dfs(dfs, cycle[i]);
        }
        auto check = [&](uint32_t k) -> bool {
            // check 的话
            // 环外部分：
            // 设以 cycle[i] 为根的树大小为 size
            // 那么这棵树必须包含 size/k 个模 k 为 0 的子树
            // 环内部分：
            // 环上的分割，按照前缀和的余数来算
            // presum[i]%k 相同的可以分为一组
            std::vector<uint32_t> rem(k);
            uint32_t cycle_sum = 0;
            for (uint32_t i = 0; i != cycle.size(); i++) {
                uint32_t size = buckets[i].front(), cnt = 0;
                for (auto sub_size : buckets[i]) cnt += sub_size % k == 0;
                if (cnt != size / k) return false;
                cycle_sum += (size - 1) % k + 1;
                rem[cycle_sum % k]++;
            }
            uint32_t need = cycle_sum / k;
            for (uint32_t i = 0; i != k; i++)
                if (rem[i] == need) return true;
            return false;
        };

        uint32_t ans = 0;
        for (uint32_t k = 1; k <= n; k++)
            if (n % k == 0) ans += check(k);
        cout << ans << endl;
    }
}