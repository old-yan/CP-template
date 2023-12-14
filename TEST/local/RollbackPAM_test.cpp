#include "IO/FastIO.h"
#include "STR/RollbackPAM.h"

#include <map>

void test_find_longest() {
    cout << "test find longest:\n";
    std::string s = "abacabadabadabacabad";

    using PAM = OY::StaticRollbackPAM_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });
    cout << "longest palindromic string endsWith index 19 is: \n"
         << s.substr(19 - pam.get_node(pam.query_node_index(19))->m_length + 1, pam.get_node(pam.query_node_index(19))->m_length) << endl;

    cout << "longest palindromic string endsWith index 18 is: \n"
         << s.substr(18 - pam.get_node(pam.query_node_index(18))->m_length + 1, pam.get_node(pam.query_node_index(18))->m_length) << endl;

    cout << "longest palindromic string endsWith index 14 is: \n"
         << s.substr(14 - pam.get_node(pam.query_node_index(14))->m_length + 1, pam.get_node(pam.query_node_index(14))->m_length) << endl;

    cout << endl;
}

void test_find_all_occurrences() {
    cout << "test find all ocuurrences:\n";
    std::string s = "abacabadabadabacabad";

    using PAM = OY::StaticRollbackPAM_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto cur = pam.query_node_index(18);
    do {
        cout << s.substr(18 - pam.get_node(cur)->m_length + 1, pam.get_node(cur)->m_length) << endl;
        cur = pam.query_fail(cur);
    } while (pam.is_node(cur));
    cout << endl;
}

struct NodeWrap {
    std::map<uint32_t, uint32_t> m_child;
    bool has_child(uint32_t index) const { return m_child.find(index) != m_child.end(); }
    void add_child(uint32_t index, uint32_t child) { m_child[index] = child; }
    void remove_child(uint32_t index) { m_child.erase(index); }
    uint32_t get_child(uint32_t index) const { return m_child.find(index)->second; }
};
void test_map_node() {
    cout << "test map node:\n";
    std::string s = "abcbaabcbaabcbaabcbaabcbaabcba";

    // 找出以下标 29 作为结尾的所有的本质不同的回文串
    // 什么叫本质不同？当字符串出现周期之后，一个字符串的后缀子串中，超过自身长度一半的长度会形成一个等差数列
    // 对于此类的子串，调用本方法只会访问次长的及最短的
    using PAM = OY::RollbackPAM::Automaton<NodeWrap, std::string, 10>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    pam.do_for_each_distinct_node(pam.query_node_index(29), [&](uint32_t a) {
        cout << "len = " << pam.get_node(a)->m_length << " : " << s.substr(s.size() - pam.get_node(a)->m_length) << endl;
    });
    cout << endl;
}

int main() {
    test_find_longest();
    test_find_all_occurrences();
    test_map_node();
}
/*
#输出如下
test find longest:
longest palindromic string endsWith index 19 is: 
dabacabad
longest palindromic string endsWith index 18 is: 
abacabadabadabacaba
longest palindromic string endsWith index 14 is: 
abadabadaba

test find all ocuurrences:
abacabadabadabacaba
abacaba
aba
a

test map node:
len = 30 : abcbaabcbaabcbaabcbaabcbaabcba
len = 25 : abcbaabcbaabcbaabcbaabcba
len = 5 : abcba
len = 1 : a

*/