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

void solve_PAM() {
    using PAM = OY::StaticPAM_string<26>;
    // using PAM = OY::StaticRollbackPAM_string<26, 19>;
    using node = PAM::node;
    std::string s;
    cin >> s;

    PAM pam(s.size(), [&](uint32_t i) { return s[i] - 'a'; });
    std::vector<uint32_t> cnt(s.size() + 2);
    for (int i = 0; i < s.size(); i++) cnt[pam.query_node_index(i)]++;

    uint64_t ans = 0;
    pam.do_for_failing_nodes([&](uint32_t a) {
        node *p = pam.get_node(a);
        ans = std::max(ans, uint64_t(p->m_length) * cnt[a]);
        cnt[p->m_fail] += cnt[a];
    });
    cout << ans << endl;
}

int main() {
    solve_PAM();
}