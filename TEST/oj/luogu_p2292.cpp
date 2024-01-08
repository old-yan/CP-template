#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P2292 [HNOI2004] L 语言](https://www.luogu.com.cn/problem/P2292)
*/
/**
 * 本题为多模式字符串匹配
 * 可以使用 ac 自动机解决
 */

using AC = OY::ACAM<26>;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    AC ac;
    ac.reserve(1000);
    std::vector<int> len(20 * n + 1), masks(20 * n + 1);
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        auto p = ac.insert_lower(s);
        len[p] = s.size();
        masks[p] = 1 << s.size();
    }
    ac.prepare();
    ac.do_for_extending_nodes([&](uint32_t a) {
        masks[a] |= masks[ac.query_fail(a)];
    });

    for (uint32_t i = 0; i < m; i++) {
        std::string s;
        cin >> s;
        uint32_t ans = -1;
        uint32_t last_pos = 0;
        // mask 的第 i 位表示向左 i 位的 dp 值
        uint32_t mask = 2;
        for (uint32_t j = 0; j < s.size(); j++) {
            last_pos = ac.next(last_pos, s[j] - 'a');
            // m_mask 的第 i 位表示长度为 i 的匹配情况
            if (mask & masks[last_pos]) mask++, ans = j;
            mask <<= 1;
        }
        cout << ans + 1 << endl;
    }
}