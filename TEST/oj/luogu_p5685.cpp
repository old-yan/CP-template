#include "IO/FastIO.h"
#include "STR/PalindromicAutomaton.h"

/*
[P5685 [JSOI2013] 快乐的 JYY](https://www.luogu.com.cn/problem/P5685)
*/
/**
 * 本题为回文串问题
 * 可以使用回文自动机模板解决
 */

template <typename Node>
struct NodeWrap {
    uint32_t m_cnt;
};
void solve_PAM() {
    std::string s1, s2;
    cin >> s1 >> s2;
    using PAM = OY::StaticAutomaton_string<NodeWrap, 26>;
    PAM pam1, pam2;
    pam1.reserve(s1.size()), pam2.reserve(s2.size());
    for (uint32_t i = 0; i < s1.size(); i++) {
        pam1.push_back(s1[i] - 'A');
        pam1.get_node(pam1.query_node_index(i))->m_cnt++;
    }
    for (uint32_t i = 0; i < s2.size(); i++) {
        pam2.push_back(s2[i] - 'A');
        pam2.get_node(pam2.query_node_index(i))->m_cnt++;
    }

    pam1.do_for_failing_nodes([&](uint32_t a) {
        if (a >= 3) {
            pam1.get_fail_node(a)->m_cnt += pam1.get_node(a)->m_cnt;
        }
    });
    pam2.do_for_failing_nodes([&](uint32_t a) {
        if (a >= 3) {
            pam2.get_fail_node(a)->m_cnt += pam2.get_node(a)->m_cnt;
        }
    });

    uint64_t ans = 0;
    auto dfs = [&](auto self, uint32_t a1, uint32_t a2) -> void {
        if (a1 > 2) {
            ans += uint64_t(pam1.get_node(a1)->m_cnt) * pam2.get_node(a2)->m_cnt;
        }
        for (uint32_t c = 0; c < 26; c++)
            if (pam1.get_node(a1)->get_child(c) && pam2.get_node(a2)->get_child(c)) {
                self(self, pam1.get_node(a1)->get_child(c), pam2.get_node(a2)->get_child(c));
            }
    };
    dfs(dfs, pam1.odd_root(), pam2.odd_root());
    dfs(dfs, pam1.even_root(), pam2.even_root());
    cout << ans << endl;
}

int main() {
    solve_PAM();
}