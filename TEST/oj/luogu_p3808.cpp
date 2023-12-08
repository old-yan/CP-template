#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P3808 【模板】AC 自动机（简单版）](https://www.luogu.com.cn/problem/P3808)
*/
/**
 * 本题为 ac 自动机模板题
 * 建好 ac 自动机之后，按照 fail 序求前缀和，即为子串出现次数
 */

template <typename Node>
struct NodeWrap {
    bool m_flag;
};
using AC = OY::AC::Automaton<NodeWrap, 26>;
using node = AC::node;
int main() {
    uint32_t n;
    cin >> n;
    AC ac;
    ac.reserve(1000001);
    std::vector<uint32_t> pos(n);
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        pos[i] = ac.insert_lower(s);
    }
    ac.prepare();

    std::string str;
    cin >> str;
    uint32_t last_pos = 0;
    for (char c : str) {
        last_pos = ac.next(last_pos, c - 'a');
        ac.get_node(last_pos)->m_flag = true;
    }
    ac.do_for_failing_nodes([&](uint32_t a) {
        if (ac.get_node(a)->m_flag) ac.get_fail_node(a)->m_flag = true;
    });

    uint32_t ans = 0;
    for (uint32_t i = 0; i < n; i++)
        if (ac.get_node(pos[i])->m_flag) ans++;
    cout << ans << endl;
}