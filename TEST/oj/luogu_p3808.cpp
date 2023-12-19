#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P3808 【模板】AC 自动机（简单版）](https://www.luogu.com.cn/problem/P3808)
*/
/**
 * 本题为 ac 自动机模板题
 * 建好 ac 自动机之后，按照 fail 序求前缀和，即为子串出现次数
 */

struct Node {
    uint32_t m_cnt;
};
using AC = OY::AC::Automaton<Node, 26>;
using node = AC::node;
int main() {
    uint32_t n;
    cin >> n;
    AC ac;
    ac.reserve(1000000);
    std::vector<uint32_t> pos(n);
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        int pos = ac.insert_lower(s);
        ac.get_node(pos)->m_cnt++;
    }
    ac.prepare();

    std::string s;
    cin >> s;
    uint32_t last_pos = 0;
    uint32_t ans = 0;
    // 可以边搜边改，搜过的就清空，以避免重复统计；同时也保证了时间复杂度
    for (char c : s) {
        last_pos = ac.next(last_pos, c - 'a');
        for (uint32_t x = last_pos; x && ac.get_node(x)->m_cnt; x = ac.query_fail(x)) {
            ans += ac.get_node(x)->m_cnt;
            ac.get_node(x)->m_cnt = 0;
        }
    }
    cout << ans << endl;
}