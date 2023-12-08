#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P3796 【模板】AC 自动机（加强版）](https://www.luogu.com.cn/problem/P3796)
*/
/**
 * 本题为 ac 自动机模板题
 * 建好 ac 自动机之后，按照 fail 序求前缀和，即为子串出现次数
 */

template <typename Node>
struct NodeWrap {
    uint32_t m_cnt;
};
using AC = OY::AC::Automaton<NodeWrap, 26>;
using node = AC::node;
int main() {
    while (true) {
        uint32_t n;
        cin >> n;
        if (!n) break;
        AC ac;
        ac.reserve(70 * n + 1);
        std::vector<std::string> words(n);
        std::vector<uint32_t> pos(n);
        for (uint32_t i = 0; i < n; i++) {
            cin >> words[i];
            pos[i] = ac.insert_lower(words[i]);
        }
        ac.prepare();

        std::string str;
        cin >> str;
        uint32_t last_pos = 0;
        for (char c : str) {
            last_pos = ac.next(last_pos, c - 'a');
            ac.get_node(last_pos)->m_cnt++;
        }
        ac.do_for_failing_nodes([&](uint32_t a) {
            ac.get_fail_node(a)->m_cnt += ac.get_node(a)->m_cnt;
        });

        uint32_t ans = 0;
        for (uint32_t i = 0; i < n; i++) ans = std::max(ans, ac.get_node(pos[i])->m_cnt);
        cout << ans << endl;
        for (uint32_t i = 0; i < n; i++)
            if (ac.get_node(pos[i])->m_cnt == ans) cout << words[i] << endl;
    }
}