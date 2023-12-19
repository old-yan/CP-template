#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P3796 【模板】AC 自动机（加强版）](https://www.luogu.com.cn/problem/P3796)
*/
/**
 * 本题为 ac 自动机模板题
 * 建好 ac 自动机之后，按照 fail 序求前缀和，即为子串出现次数
 */

struct Node {
    uint32_t m_index, m_cnt;
};
using AC = OY::AC::Automaton<Node, 26>;
using node = AC::node;
int main() {
    while (true) {
        uint32_t n;
        cin >> n;
        if (!n) break;
        AC ac;
        ac.reserve(70 * n);
        std::vector<std::string> words(n);
        for (uint32_t i = 0; i < n; i++) {
            cin >> words[i];
            int pos = ac.insert_lower(words[i]);
            ac.get_node(pos)->m_index = i + 1;
        }
        ac.prepare();

        std::string s;
        cin >> s;
        uint32_t last_pos = 0;
        for (char c : s) {
            last_pos = ac.next(last_pos, c - 'a');
            ac.get_node(last_pos)->m_cnt++;
        }

        // 在 fail 树上从叶到根倒推
        uint32_t ans = 0;
        std::vector<uint32_t> ans_pos;
        ac.do_for_failing_nodes([&](uint32_t a) {
            node *p = ac.get_node(a);
            if (p->m_cnt) {
                if (p->m_index)
                    if (p->m_cnt > ans) {
                        ans = p->m_cnt;
                        ans_pos.clear();
                        ans_pos.push_back(p->m_index);
                    } else if (p->m_cnt == ans)
                        ans_pos.push_back(p->m_index);
                ac.get_fail_node(a)->m_cnt += p->m_cnt;
            }
        });

        std::sort(ans_pos.begin(), ans_pos.end());
        cout << ans << endl;
        for (auto a : ans_pos) cout << words[a - 1] << endl;
    }
}