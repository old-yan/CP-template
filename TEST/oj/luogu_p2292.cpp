#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P2292 [HNOI2004] L 语言](https://www.luogu.com.cn/problem/P2292)
*/
/**
 * 本题为多模式字符串匹配
 * 可以使用 ac 自动机解决
 */

template <typename Node>
struct NodeWrap {
    uint32_t m_length, m_mask;
};
using AC = OY::AC::Automaton<NodeWrap, 26>;
using node = AC::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    AC ac;
    ac.reserve(1001);
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        auto p = ac.get_node(ac.insert_lower(s));
        p->m_length = s.size();
        p->m_mask = 1 << s.size();
    }
    ac.prepare();
    ac.do_for_extending_nodes([&](uint32_t a) {
        ac.get_node(a)->m_mask |= ac.get_fail_node(a)->m_mask;
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
            if (mask & ac.get_node(last_pos)->m_mask) mask++, ans = j;
            mask <<= 1;
        }
        cout << ans + 1 << endl;
    }
}