#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P5357 【模板】AC 自动机（二次加强版）](https://www.luogu.com.cn/problem/P5357)
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
    uint32_t n;
    cin >> n;

    AC ac;
    ac.reserve(200000);
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
        ac.get_node(last_pos)->m_cnt++;
    }
    ac.do_for_failing_nodes([&](uint32_t a) {
        ac.get_fail_node(a)->m_cnt += ac.get_node(a)->m_cnt;
    });

    for (uint32_t i = 0; i < n; i++) cout << ac.get_node(pos[i])->m_cnt << endl;
}