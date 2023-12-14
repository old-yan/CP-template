#include "IO/FastIO.h"
#include "STR/PAM.h"
#include "STR/RollbackPAM.h"

#include <queue>

/*
[P4762 [CERC2014] Virus synthesis](https://www.luogu.com.cn/problem/P4762)
*/
/**
 * 本题为回文串问题
 * 可以使用回文自动机模板解决
 */

template <typename Node>
struct NodeWrap {
    uint32_t m_half_index, m_dp;
};
void solve_PAM() {
    uint32_t t;
    cin >> t;
    uint32_t get_id[128];
    get_id['A'] = 0;
    get_id['T'] = 1;
    get_id['C'] = 2;
    get_id['G'] = 3;
    for (uint32_t i = 0; i < t; i++) {
        std::string s;
        cin >> s;
        OY::StaticPAM_string<NodeWrap, 4> pam(s.size(), [&](uint32_t i) { return get_id[s[i]]; });

        // 找到自动机里每个结点的自身长度一半以内的转移结点
        for (uint32_t a = 1; a < pam.m_data.size(); a++) {
            auto p = pam.get_node(a);
            for (uint32_t c = 0; c < 4; c++) {
                auto child = p->get_child(c);
                if (child) {
                    auto pc = pam.get_node(child);
                    if (pc->m_length <= 2)
                        pc->m_half_index = 1;
                    else {
                        auto z = p->m_half_index;
                        while (s[pc->m_size_when_appear - 1 - pam.get_node(z)->m_length - 1] != s[pc->m_size_when_appear - 1] || (pam.get_node(z)->m_length + 2) * 2 > pc->m_length) z = pam.query_fail(z);
                        pc->m_half_index = pam.get_node(z)->get_child(c);
                    }
                }
            }
        }

        uint32_t ans = 0x3f3f3f3f;
        std::queue<uint32_t> Q;
        Q.push(pam.odd_root()), pam.get_node(pam.odd_root())->m_dp = 0;
        Q.push(pam.even_root()), pam.get_node(pam.even_root())->m_dp = 1;
        while (Q.size()) {
            uint32_t a = Q.front();
            Q.pop();
            auto p = pam.get_node(a);
            for (uint32_t c = 0; c < 4; c++) {
                auto child = p->get_child(c);
                if (child) {
                    auto pc = pam.get_node(child);
                    if (pc->m_length % 2 == 0)
                        pc->m_dp = std::min({pc->m_length, p->m_dp + 1, pam.get_node(pc->m_half_index)->m_dp + 1 + pc->m_length / 2 - pam.get_node(pc->m_half_index)->m_length});
                    else
                        pc->m_dp = std::min({pc->m_length, p->m_dp + 2});
                    ans = std::min<uint32_t>(ans, pc->m_dp + s.size() - pc->m_length);
                    Q.push(child);
                }
            }
        }

        cout << ans << endl;
    }
}

void solve_RollbackPAM() {
    uint32_t t;
    cin >> t;
    uint32_t get_id[128];
    get_id['A'] = 0;
    get_id['T'] = 1;
    get_id['C'] = 2;
    get_id['G'] = 3;
    for (uint32_t i = 0; i < t; i++) {
        std::string s;
        cin >> s;
        OY::StaticRollbackPAM_string<NodeWrap, 4, 17> pam(s.size(), [&](uint32_t i) { return get_id[s[i]]; });

        uint32_t ans = 0x3f3f3f3f;
        // 本来应该在 trie 树里做 bfs
        // 但是恰好结点下标序就是 bfs 拓扑序，所以直接按下标来即可
        pam.get_node(pam.even_root())->m_dp = 1;
        pam.do_for_extending_nodes([&](uint32_t i) {
            auto p = pam.get_node(i);
            uint32_t fa = p->m_fa;
            if (p->m_length % 2 == 0) {
                // 找到自动机里每个结点的自身长度一半以内的转移结点，试图蹭车
                auto half_node = pam.get_node(pam.query_fail_to_half(i));
                p->m_dp = std::min({p->m_length, pam.get_node(fa)->m_dp + 1, half_node->m_dp + 1 + p->m_length / 2 - half_node->m_length});
            } else
                p->m_dp = std::min({p->m_length, pam.get_node(fa)->m_dp + 2});
            ans = std::min<uint32_t>(ans, p->m_dp + s.size() - p->m_length);
        });
        cout << ans << endl;
    }
}

int main() {
    solve_PAM();
    // solve_RollbackPAM();
}