#include "DS/AdjDiff.h"
#include "IO/FastIO.h"
#include "STR/Manacher.h"
#include "STR/PAM.h"

/*
[Best Reward](https://acm.hdu.edu.cn/showproblem.php?pid=3613)
*/
/**
 * 本题为回文串题目，可以有多种做法
 * 可以使用马拉车算法解决
 * 可以使用回文树或者序列哈希模板解决
 */

void solve() {
    uint32_t t;
    cin >> t;
    while (t--) {
        int value[26];
        for (auto &a : value) cin >> a;
        std::string s;
        cin >> s;
        OY::Manacher<true, true> Checker(s);
        // OY::StaticPAM_string<26> Checker(s.size(), [&](int i) { return s[i] - 'a'; });
        OY::AdjDiff::Table<int, false> S(s.size(), [&](uint32_t i) {
            return value[s[i] - 'a'];
        });

        S.switch_to_presum();
        int ans = INT_MIN;
        // 枚举所有的分割成两部分的分割点
        for (int i = 1; i < s.size(); i++) {
            // 对于每次分割，使用马拉车模板判断分割结果是否回文
            int val_left = Checker.query(0, i - 1) ? S.query(0, i - 1) : 0;
            int val_right = Checker.query(i, s.size() - 1) ? S.query(i, s.size() - 1) : 0;
            ans = std::max(ans, val_left + val_right);
        }
        cout << ans << endl;
    }
}

int main() {
    solve();
}