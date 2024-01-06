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
        OY::StaticPAM_string<4> pam(s.size(), [&](uint32_t i) { return get_id[s[i]]; });

        // 找到自动机里每个结点的自身长度一半以内的转移结点
        std::vector<uint32_t> dp(pam.m_data.size()), half(pam.m_data.size());
        for (uint32_t a = 1; a < pam.m_data.size(); a++) {
            auto p = pam.get_node(a);
            for (uint32_t c = 0; c < 4; c++) {
                auto child = p->get_child(c);
                if (child) {
                    auto pc = pam.get_node(child);
                    if (pc->m_length <= 2)
                        half[child] = 1;
                    else {
                        auto z = half[a];
                        while (s[pc->m_size_when_appear - 1 - pam.get_node(z)->m_length - 1] != s[pc->m_size_when_appear - 1] || (pam.get_node(z)->m_length + 2) * 2 > pc->m_length) z = pam.query_fail(z);
                        half[child] = pam.get_node(z)->get_child(c);
                    }
                }
            }
        }

        uint32_t ans = 0x3f3f3f3f;
        std::queue<uint32_t> Q;
        Q.push(pam.odd_root()), dp[pam.odd_root()] = 0;
        Q.push(pam.even_root()), dp[pam.even_root()] = 1;
        while (Q.size()) {
            uint32_t a = Q.front();
            Q.pop();
            auto p = pam.get_node(a);
            for (uint32_t c = 0; c < 4; c++) {
                auto child = p->get_child(c);
                if (child) {
                    auto pc = pam.get_node(child);
                    if (pc->m_length % 2 == 0)
                        dp[child] = std::min({pc->m_length, dp[a] + 1, dp[half[child]] + 1 + pc->m_length / 2 - pam.get_node(half[child])->m_length});
                    else
                        dp[child] = std::min({pc->m_length, dp[a] + 2});
                    ans = std::min<uint32_t>(ans, dp[child] + s.size() - pc->m_length);
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
        OY::StaticRollbackPAM_string<4, 17> pam(s.size(), [&](uint32_t i) { return get_id[s[i]]; });

        std::vector<uint32_t> dp(pam.m_data.size());
        uint32_t ans = 0x3f3f3f3f;
        dp[pam.even_root()] = 1;
        pam.do_for_extending_nodes([&](uint32_t i) {
            auto p = pam.get_node(i);
            uint32_t fa = p->m_fa;
            if (p->m_length % 2 == 0) {
                // 找到自动机里每个结点的自身长度一半以内的转移结点，试图蹭车
                auto half = pam.query_fail_to_half(i);
                auto half_node = pam.get_node(half);
                dp[i] = std::min({p->m_length, dp[fa] + 1, dp[half] + 1 + p->m_length / 2 - half_node->m_length});
            } else
                dp[i] = std::min({p->m_length, dp[fa] + 2});
            ans = std::min<uint32_t>(ans, dp[i] + s.size() - p->m_length);
        });

        cout << ans << endl;
    }
}

int main() {
    solve_PAM();
    solve_RollbackPAM();
}