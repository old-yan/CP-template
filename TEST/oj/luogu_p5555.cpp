#include "IO/FastIO.h"
#include "STR/PalindromicAutomaton.h"

/*
[P5555 秩序魔咒](https://www.luogu.com.cn/problem/P5555)
*/
/**
 * 本题为回文串问题
 * 可以使用回文自动机模板解决
 */

void solve_PAM() {
    uint32_t n1, n2;
    std::string s1, s2;
    cin >> n1 >> n2 >> s1 >> s2;
    using PAM = OY::StaticAutomaton_string<>;
    PAM pam1(s1.size(), [&](uint32_t i) { return s1[i] - 'a'; }), pam2(s2.size(), [&](uint32_t i) { return s2[i] - 'a'; });

    uint32_t ans = 0, ans_cnt = 0;
    auto dfs = [&](auto self, uint32_t a1, uint32_t a2, uint32_t len) -> void {
        if (a1 > 2) {
            if (len > ans)
                ans = len, ans_cnt = 1;
            else if (len == ans)
                ans_cnt++;
        }
        for (uint32_t c = 0; c < 26; c++)
            if (pam1.get_node(a1)->get_child(c) && pam2.get_node(a2)->get_child(c)) {
                self(self, pam1.get_node(a1)->get_child(c), pam2.get_node(a2)->get_child(c), len + 2);
            }
    };
    dfs(dfs, pam1.odd_root(), pam2.odd_root(), -1);
    dfs(dfs, pam1.even_root(), pam2.even_root(), 0);
    cout << ans << ' ' << ans_cnt << endl;
}

int main() {
    solve_PAM();
}