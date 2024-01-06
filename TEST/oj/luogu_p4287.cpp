#include "IO/FastIO.h"
#include "STR/Manacher.h"
#include "STR/PAM.h"
#include "STR/RollbackPAM.h"

/*
[P4287 [SHOI2011] 双倍回文](https://www.luogu.com.cn/problem/P4287)
*/
/**
 * 本题为回文串问题
 * 可以使用回文自动机模板来解决
 * 可以使用马拉车模板解决
 */

void solve_manacher() {
    uint32_t n;
    std::string s;
    cin >> n >> s;

    OY::Manacher<true, false> M(s);
    uint32_t ans = 0;
    struct node {
        uint32_t m_center, m_end;
    };
    std::vector<node> stack[2];
    uint32_t cursor[2]{};
    for (uint32_t i = 1; i < s.size(); i++) {
        uint32_t len = M.query_max_even(i - 1, i);
        if (cursor[i % 2] == stack[i % 2].size() || i + len - 1 > stack[i % 2].back().m_end) stack[i % 2].push_back({i, i + len - 1});
        while (cursor[(i + 1) % 2] < stack[(i + 1) % 2].size() && stack[(i + 1) % 2][cursor[(i + 1) % 2]].m_end < i) cursor[(i + 1) % 2]++;
        if (cursor[(i + 1) % 2] < stack[(i + 1) % 2].size()) {
            if (M.query(stack[(i + 1) % 2][cursor[(i + 1) % 2]].m_center, i)) ans = std::max(ans, (i - stack[(i + 1) % 2][cursor[(i + 1) % 2]].m_center + 1) * 2);
        }
    }
    cout << ans << endl;
}

void solve_PAM() {
    uint32_t n;
    std::string s;
    cin >> n >> s;

    OY::StaticPAM_string<> pam;
    // OY::StaticRollbackPAM_string<> pam;
    pam.reserve(s.size());
    uint32_t ans = 0;
    for (uint32_t i = 0; i < s.size(); i++) {
        pam.push_back(s[i] - 'a');
        auto p = pam.get_node(pam.query_node_index(i));
        if (p->m_length % 4 == 0)
            if (pam.query(i - p->m_length / 2 + 1, i)) ans = std::max(ans, p->m_length);
    }
    cout << ans << endl;
}

int main() {
    solve_manacher();
    // solve_PAM();
}