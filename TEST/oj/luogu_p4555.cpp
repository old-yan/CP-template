#include "IO/FastIO.h"
#include "STR/Manacher.h"
#include "STR/PAM.h"
#include "STR/RollbackPAM.h"

/*
[P4555 [国家集训队] 最长双回文串](https://www.luogu.com.cn/problem/P4555)
*/
/**
 * 本题需要快速判断子串的回文性，可以有多种做法
 * 可以用马拉车模板解决
 * 也可以用回文自动机解决
 */

void solve_manacher() {
    std::string s;
    cin >> s;
    OY::Manacher<true, true> M(s);
    int n = s.size();
    std::vector<int> to_left(n, 1), to_right(n, 1);

    struct Node {
        int m_center, m_end;
    };
    std::vector<Node> dp;
    uint32_t cursor = 0;
    // 更新向左的最大奇长度
    for (int i = 0; i < n; i++) {
        int len = M.query_max_odd(i);
        if (cursor == dp.size() || dp.back().m_end < i + len) dp.push_back({i, i + len});
        if (cursor < dp.size() && dp[cursor].m_end < i) cursor++;
        if (cursor < dp.size()) to_left[i] = std::max(to_left[i], (i - dp[cursor].m_center) * 2 + 1);
    }

    dp.clear();
    cursor = 0;
    // 更新向左的最大偶长度
    for (int i = 1; i < n; i++) {
        int len = M.query_max_even(i - 1, i);
        if (cursor == dp.size() || dp.back().m_end < i + len - 1) dp.push_back({i - 1, i + len - 1});
        if (cursor < dp.size() && dp[cursor].m_end < i) cursor++;
        if (cursor < dp.size()) to_left[i] = std::max(to_left[i], (i - dp[cursor].m_center) * 2);
    }

    dp.clear();
    cursor = 0;
    // 更新向右的最大奇长度
    for (int i = n - 1; ~i; i--) {
        int len = M.query_max_odd(i);
        if (cursor == dp.size() || dp.back().m_end > i - len) dp.push_back({i, i - len});
        if (cursor < dp.size() && dp[cursor].m_end > i) cursor++;
        if (cursor < dp.size()) to_right[i] = std::max(to_right[i], (dp[cursor].m_center - i) * 2 + 1);
    }

    dp.clear();
    cursor = 0;
    // 更新向右的最大偶长度
    for (int i = n - 1; i; i--) {
        int len = M.query_max_even(i - 1, i);
        if (cursor == dp.size() || dp.back().m_end > i - len) dp.push_back({i, i - len});
        if (cursor < dp.size() && dp[cursor].m_end > i) cursor++;
        if (cursor < dp.size()) to_right[i] = std::max(to_right[i], (dp[cursor].m_center - i) * 2);
    }

    int ans = 0;
    for (int i = 1; i < n; i++) ans = std::max(ans, to_left[i - 1] + to_right[i]);
    cout << ans << endl;
}

void solve_PAM() {
    std::string s;
    cin >> s;
    using PAM = OY::StaticPAM_string<>;
    // using PAM = OY::StaticRollbackPAM_string<>;
    PAM pam(s.size(), [&](uint32_t i) { return s[i] - 'a'; });
    PAM pam2(s.size(), [&](uint32_t i) { return s[s.size() - 1 - i] - 'a'; });

    uint32_t ans = 0;
    for (int i = 1; i < s.size(); i++) {
        ans = std::max(ans, pam.get_node(pam.query_node_index(i - 1))->m_length + pam2.get_node(pam2.query_node_index(s.size() - 1 - i))->m_length);
    }
    cout << ans << endl;
}

int main() {
    solve_manacher();
    // solve_PAM();
}