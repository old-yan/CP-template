#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

#include <map>

void test_find_index() {
    std::string p[] = {"ab", "abca", "bc", "a", "b", "c", "d", "ca"};
    std::string s = "abcaabcaab";

    // 比如我们想知道匹配到的模式串的下标，那么我们需要额外维护每个结点对应的串的下标
    using AC = OY::ACAM<26>;
    AC ac;
    std::map<int, int> mp;
    for (uint32_t i = 0; i < 8; i++) {
        auto node_index = ac.insert_lower(p[i]);
        mp[node_index] = i;
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
            if (mp.count(x)) {
                cout << s << " contains " << p[mp[x]] << endl;
            }
        }
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }
}

void test_find_count() {
    std::string p[] = {"ab", "abca", "bc", "a", "b", "c", "d", "ca"};
    std::string s = "abcaabcaab";
    using AC = OY::ACAM<26>;
    AC ac;
    // 插入时不动 size 信息，仅仅把每个字符串要找的结点记录下来
    int ins_pos[8];
    for (uint32_t i = 0; i < 8; i++) {
        ins_pos[i] = ac.insert_lower(p[i]);
    }
    ac.prepare();
    // 在匹配过程中，将所有匹配的串的大小加一
    std::vector<int> sz(ac.m_data.size());
    uint32_t pos = 0;
    for (char c : s) {
        pos = ac.next(pos, c - 'a');
        for (auto x = pos; x; x = ac.query_fail(x)) sz[x]++;
    }
    // 输出模式串出现次数
    for (uint32_t i = 0; i < 8; i++) {
        cout << p[i] << " appear " << sz[ins_pos[i]] << " times\n";
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