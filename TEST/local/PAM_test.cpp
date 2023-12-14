#include "IO/FastIO.h"
#include "STR/PAM.h"

#include <map>

void test_find_longest() {
    cout << "test find longest:\n";
    std::string s = "abacabadabadabacabad";

    using PAM = OY::StaticPAM_string<>;
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

    using PAM = OY::StaticPAM_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto cur = pam.query_node_index(18);
    do {
        cout << s.substr(18 - pam.get_node(cur)->m_length + 1, pam.get_node(cur)->m_length) << endl;
        cur = pam.query_fail(cur);
    } while (pam.is_node(cur));
    cout << endl;
}

struct Node {
    std::map<uint32_t, uint32_t> m_child;
    bool has_child(uint32_t index) const { return m_child.find(index) != m_child.end(); }
    void add_child(uint32_t index, uint32_t child) { m_child[index] = child; }
    void remove_child(uint32_t index) { m_child.erase(index); }
    uint32_t get_child(uint32_t index) const { return m_child.find(index)->second; }
};
void test_map_node() {
    cout << "test map node:\n";
    std::string s = "abacabadabadabacabad";

    using PAM = OY::PAM::Automaton<Node, std::string>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto cur = pam.query_node_index(18);
    do {
        cout << s.substr(18 - pam.get_node(cur)->m_length + 1, pam.get_node(cur)->m_length) << endl;
        cur = pam.query_fail(cur);
    } while (pam.is_node(cur));
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
abacabadabadabacaba
abacaba
aba
a

*/