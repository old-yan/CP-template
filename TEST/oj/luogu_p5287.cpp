#include "DS/LinkBucket.h"
#include "IO/FastIO.h"
#include "STR/RollbackKMP.h"

/*
[P5287 [HNOI2019] JOJO](https://www.luogu.com.cn/problem/P5287)
*/
/**
 * 本题可以用 kmp 模板解决
 * 表面上本题需要使用可持久化数据结构
 * 实际上，只需要离线处理查询，在版本树上进行动态修改查询即可。
 * 由于数据范围较大，所以不能处理字符和字符的匹配，而是必须处理段和段的匹配
 * 一个前缀段区间 S1 和一个后缀段区间 S2 的匹配，其实可以分解成
 *      S1 = pre1 + mid1 + suf1
 *      S2 = pre2 + mid2 + suf2
 * 这里的 mid1 和 mid2 必须一模一样，可以是零个、一个或者多个段
 * pre1 和 pre2 的字符必须一样，pre2 的字符数必须多于 pre1 的字符数
 * suf1 和 suf2 的字符必须一样
 *
 * 所以，第一个段不要放入 kmp 里，而是做特判
 * 从第二个段放入 kmp 里
 */

static constexpr uint32_t N = 100000;
struct Node {
    uint32_t m_index, m_cnt;
    char m_char;
};
uint32_t id[N + 1];
int64_t ans[N + 1];
OY::LBC::LinkBucket<Node, N + 1, N + 1> buckets;
int main() {
    uint32_t n;
    cin >> n;
    buckets.resize(n + 1, n + 1);
    uint32_t cur = 0;
    // 先读入所有查询，记录好版本之间的依赖关系
    for (uint32_t i = 1; i <= n; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t cnt;
            char c;
            cin >> cnt >> c;
            buckets[cur].push_back(Node{i, cnt, c});
            id[i] = cur = i;
        } else {
            uint32_t target;
            cin >> target;
            id[i] = cur = id[target];
        }
    }

    // 在版本树上做修改、查询
    struct item {
        uint32_t m_cnt;
        char m_char;
        bool operator==(const item &rhs) const { return m_char == rhs.m_char && m_cnt == rhs.m_cnt; }
        bool operator!=(const item &rhs) const { return m_char != rhs.m_char || m_cnt != rhs.m_cnt; }
    };
    OY::RollbackKMP_vector<item> kmp;
    std::vector<item> stack;
    std::vector<uint32_t> presum{0};
    // 从版本 0 开始作为递归起点，注意第一段字符并不会被加到 kmp 里
    for (auto &&[index, cnt, c] : buckets[0]) {
        uint64_t sum = cnt * (cnt - 1) / 2;
        uint32_t first_cnt = cnt;
        char first_c = c;
        auto dfs = [&](auto self, uint32_t cur) -> void {
            ans[cur] = sum;
            for (auto &&[index, cnt, c] : buckets[cur]) {
                auto old_sum = sum;
                uint32_t searched = 0;
                // 对所有的本质不同 border ，按照从长到短的顺序调用回调，处理三段以上的匹配
                auto callback = [&](uint32_t pi) {
                    if (searched < cnt && stack[stack.size() - pi - 1].m_char == first_c && stack[stack.size() - pi - 1].m_cnt >= first_cnt && stack[pi].m_char == c && stack[pi].m_cnt > searched) {
                        uint32_t end = std::min(cnt, stack[pi].m_cnt);
                        sum += uint64_t(presum[pi] + first_cnt) * (end - searched) + (end - searched) * (end + searched + 1) / 2;
                        searched = end;
                    }
                };
                if (kmp.size()) kmp.do_for_each_distinct_border(kmp.query_Pi(kmp.size() - 1), callback);
                // 特判两段的匹配
                if (stack.size() && stack.back().m_char == first_c && stack.back().m_cnt >= first_cnt && stack[0].m_char == c && stack[0].m_cnt > searched) {
                    uint32_t end = std::min(cnt, stack[0].m_cnt);
                    sum += uint64_t(first_cnt) * (end - searched) + (end - searched) * (end + searched + 1) / 2;
                    searched = end;
                }
                // 特判一段的匹配
                if (searched != cnt && first_c == c) {
                    uint32_t Max = std::min(first_cnt, cnt);
                    if (Max == cnt)
                        sum += (Max - searched) * (Max + searched + 1) / 2;
                    else if (Max >= searched) {
                        sum += (Max - searched) * (Max + searched + 1) / 2;
                        sum += Max * (cnt - Max);
                    } else
                        sum += Max * (cnt - searched);
                }
                kmp.push_back({cnt, c});
                stack.push_back({cnt, c});
                presum.push_back(presum.back() + cnt);

                // 递归
                self(self, index);

                presum.pop_back(), kmp.pop_back(), stack.pop_back(), sum = old_sum;
            }
        };
        dfs(dfs, index);
    }
    for (uint32_t i = 1; i <= n; i++) cout << ans[id[i]] % 998244353 << endl;
}