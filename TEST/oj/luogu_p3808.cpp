#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P3808 【模板】AC 自动机（简单版）](https://www.luogu.com.cn/problem/P3808)
*/
/**
 * 本题为 ac 自动机模板题
 * 建好 ac 自动机之后，按照 fail 序求前缀和，即为子串出现次数
 */

using AC = OY::ACAM<26>;
int main() {
    uint32_t n;
    cin >> n;
    AC ac;
    ac.reserve(1000000);
    std::vector<uint32_t> pos(n);
    std::vector<int> cnt(1000001);
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        int pos = ac.insert_lower(s);
        cnt[pos]++;
    }
    ac.prepare();

    std::string s;
    cin >> s;
    uint32_t last_pos = 0;
    uint32_t ans = 0;
    // 可以边搜边改，搜过的就清空，以避免重复统计；同时也保证了时间复杂度
    for (char c : s) {
        last_pos = ac.next(last_pos, c - 'a');
        for (uint32_t x = last_pos; x && cnt[x]; x = ac.query_fail(x)) {
            ans += cnt[x];
            cnt[x] = 0;
        }
    }
    cout << ans << endl;
}