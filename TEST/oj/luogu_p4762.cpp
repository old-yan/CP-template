#include "IO/FastIO.h"
#include "STR/PalindromicAutomaton.h"

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
    std::string sss[] = {"A", "T", "C", "G"};
    for (uint32_t i = 0; i < t; i++) {
        std::string s;
        cin >> s;
        OY::StaticAutomaton_string<NodeWrap, 4> pam(s.size(), [&](uint32_t i) { return get_id[s[i]]; });
        
        // 找到自动机里每个结点的自身长度一半以内的转移结点
        for (uint32_t a = 1; a < pam.m_data.size(); a++) {
            auto p = pam.get_node(a);
            for (uint32_t c = 0; c < 4; c++) {
                auto child = p->get_child(c);
                if (child) {
                    auto pc = pam.get_node(child);
                    if (pc->m_length <= 2)
                        pc->m_half_index = 2;
                    else {
                        auto z = p->m_half_index;
                        while (s[pc->m_first_appear - 1 - pam.get_node(z)->m_length - 1] != s[pc->m_first_appear - 1] || (pam.get_node(z)->m_length + 2) * 2 > pc->m_length) z = pam.get_fail(z);
                        pc->m_half_index = pam.get_node(z)->get_child(c);
                    }
                }
            }
        }
        
        uint32_t ans = 0x3f3f3f3f;
        std::queue<uint32_t> Q;
        Q.push(1), pam.get_node(1)->m_dp = 0;
        Q.push(2), pam.get_node(2)->m_dp = 1;
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

int main() {
    solve_PAM();
}