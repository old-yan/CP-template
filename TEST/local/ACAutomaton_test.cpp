#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

template <typename Node>
struct NodeWrap_index {
    uint32_t m_index = -1;
};
void test_find_index() {
    std::string p[] = {"ab", "abca", "bc", "a", "b", "c", "d", "ca"};
    std::string s = "abcaabcaab";

    // 比如我们想知道匹配到的模式串的下标，那么我们可以写一个维护下标的结构体
    using AC = OY::AC::Automaton<NodeWrap_index, 26>;
    AC ac;
    for (uint32_t i = 0; i < 8; i++) {
        auto index = ac.insert_lower(p[i]);
        ac.get_node(index)->m_index = i;
    }
    ac.prepare();

    uint32_t searched = 0;
    // 观察，发现每次迭代搜到的模式串，都是文本串的已搜部分的一个后缀
    uint32_t pos = 0;
    for (char c : s) {
        searched++;
        pos = ac.next(pos, c - 'a');
        cout << "now we searched " << s.substr(0, searched) << endl;
        for (auto x = pos; x; x = ac.query_fail(x)) {
            if (ac.get_node(x)->m_index != -1) {
                cout << s << " contains " << p[ac.get_node(x)->m_index] << endl;
            }
        }
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }
}

template <typename Node>
struct NodeWrap_cnt {
    uint32_t m_size = 0;
};
void test_find_count() {
    std::string p[] = {"ab", "abca", "bc", "a", "b", "c", "d", "ca"};
    std::string s = "abcaabcaab";
    using AC = OY::AC::Automaton<NodeWrap_cnt, 26>;
    AC ac;
    // 插入时不动 size 信息，仅仅把每个字符串要找的结点记录下来
    int ins_pos[8];
    for (uint32_t i = 0; i < 8; i++) {
        ins_pos[i] = ac.insert_lower(p[i]);
    }
    ac.prepare();
    // 在匹配过程中，将所有匹配的串的大小加一
    uint32_t pos = 0;
    for (char c : s) {
        pos = ac.next(pos, c - 'a');
        for (auto x = pos; x; x = ac.query_fail(x)) ac.get_node(x)->m_size++;
    }
    // 输出模式串出现次数
    for (uint32_t i = 0; i < 8; i++) {
        cout << p[i] << " appear " << ac.get_node(ins_pos[i])->m_size << " times\n";
    }
}
int main() {
    test_find_index();
    test_find_count();
}
/*
#输出如下
now we searched a
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched ab
abcaabcaab contains ab
abcaabcaab contains b
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abc
abcaabcaab contains bc
abcaabcaab contains c
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abca
abcaabcaab contains abca
abcaabcaab contains ca
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaa
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaab
abcaabcaab contains ab
abcaabcaab contains b
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaabc
abcaabcaab contains bc
abcaabcaab contains c
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaabca
abcaabcaab contains abca
abcaabcaab contains ca
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaabcaa
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaabcaab
abcaabcaab contains ab
abcaabcaab contains b
~~~~~~~~~~~~~~~~~~~~~~~~~
ab appear 3 times
abca appear 2 times
bc appear 2 times
a appear 5 times
b appear 3 times
c appear 2 times
d appear 0 times
ca appear 2 times

*/