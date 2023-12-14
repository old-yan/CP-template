#include "IO/FastIO.h"
#include "STR/PAM.h"
#include "STR/RollbackPAM.h"

/*
[P5685 [JSOI2013] 快乐的 JYY](https://www.luogu.com.cn/problem/P5685)
*/
/**
 * 本题为回文串问题
 * 可以使用回文自动机模板解决
 */

template <typename Node>
struct NodeWrap {
    uint32_t m_cnt1, m_cnt2;
};
void solve_PAM() {
    std::string s1, s2;
    cin >> s1 >> s2;
    using PAM = OY::StaticPAM_string<NodeWrap, 28>;
    // using PAM = OY::StaticRollbackPAM_string<NodeWrap, 28, 16>;
    // 本题处理方式类似 P5555
    // 将 s1 和 s2 用两个特殊字符连接，然后插入 PAM 中
    // 两个串对遍历到的结点各自计数
    PAM pam;
    pam.reserve(s1.size() + s2.size() + 2);
    for (char c : s1) {
        pam.push_back(c - 'A');
        pam.get_node(pam.query_node_index(pam.size() - 1))->m_cnt1++;
    }
    pam.push_back(26);
    pam.push_back(27);
    for (char c : s2) {
        pam.push_back(c - 'A');
        pam.get_node(pam.query_node_index(pam.size() - 1))->m_cnt2++;
    }
    uint64_t ans = 0;
    pam.do_for_failing_nodes([&](uint32_t a) {
        pam.get_fail_node(a)->m_cnt1 += pam.get_node(a)->m_cnt1;
        pam.get_fail_node(a)->m_cnt2 += pam.get_node(a)->m_cnt2;
        ans += uint64_t(pam.get_node(a)->m_cnt1) * pam.get_node(a)->m_cnt2;
    });
    cout << ans << endl;
}

int main() {
    solve_PAM();
}