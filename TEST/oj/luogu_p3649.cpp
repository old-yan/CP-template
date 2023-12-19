#include "IO/FastIO.h"
#include "STR/PAM.h"
#include "STR/RollbackPAM.h"

/*
[P3649 [APIO2014] 回文串](https://www.luogu.com.cn/problem/P3649)
*/
/**
 * 本题为回文串题目，可以有多种做法
 * 可以使用回文自动机模板解决
 */

struct Node {
    uint32_t m_cnt;
};
void solve_PAM() {
    using PAM = OY::StaticPAM_string<Node, 26>;
    // using PAM = OY::StaticRollbackPAM_string<NodeWrap, 26, 19>;
    using node = PAM::node;
    std::string s;
    cin >> s;

    PAM pam(s.size(), [&](uint32_t i) { return s[i] - 'a'; });
    for (int i = 0; i < s.size(); i++) pam.get_node(pam.query_node_index(i))->m_cnt++;

    uint64_t ans = 0;
    pam.do_for_failing_nodes([&](uint32_t a) {
        node *p = pam.get_node(a);
        ans = std::max(ans, uint64_t(p->m_length) * p->m_cnt);
        pam.get_fail_node(a)->m_cnt += p->m_cnt;
    });
    cout << ans << endl;
}

int main() {
    solve_PAM();
}