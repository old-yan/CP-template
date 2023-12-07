#include "DS/AdjDiff.h"
#include "IO/FastIO.h"
#include "STR/Manacher.h"

/*
[Best Reward](https://acm.hdu.edu.cn/showproblem.php?pid=3613)
*/
/**
 * 本题为回文串题目，可以有多种做法
 * 可以使用马拉车算法解决
 */

void solve_manacher() {
    uint32_t t;
    cin >> t;
    while (t--) {
        int value[26];
        for (auto &a : value) cin >> a;
        std::string s;
        cin >> s;
        OY::Manacher<true, true> M(s);
        OY::AdjDiff::Table<int, false, 5000000> S(s.size(), [&](uint32_t i) {
            return value[s[i] - 'a'];
        });
        
        S.switch_to_presum();
        int ans = INT_MIN;
        for (int i = 1; i < s.size(); i++) {
            ans = std::max(ans, (M.query(0, i - 1) ? S.query(0, i - 1) : 0) + (M.query(i, s.size() - 1) ? S.query(i, s.size() - 1) : 0));
        }
        cout << ans << endl;
    }
}

int main() {
    solve_manacher();
}