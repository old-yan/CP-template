#include "DS/MonoAVL.h"
#include "DS/MonoSplay.h"
#include "IO/FastIO.h"

/*
[正义从不打背身](https://ac.nowcoder.com/acm/problem/277862)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/89237/I
 * 两种操作
 * reverse 可以用内置操作解决
 * flip 可以计算一下
 */

using Tree = OY::MonoSplaySequence<uint32_t, true>;
// using Tree = OY::MonoAVLSequence<uint32_t, true>;
int main() {
    uint32_t n, m;
    std::string s;
    cin >> n >> m >> s;
    Tree::_reserve(m + 1);
    Tree S;
    for (uint32_t i = 0; i < m; i++) {
        S.insert(S.size(), i);
        S.reverse();
    }
    S.enumerate([&](Tree::node *p) {
        uint32_t i = p->m_val;
        cout << ((m - i) % 2 ? s[i] == 'B' : s[i] == 'P') << ' ';
    });
    for (uint32_t i = m; i != n; i++) cout << (s[i] == 'P') << ' ';
}