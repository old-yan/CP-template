#include "IO/FastIO.h"
#include "STR/PalindromicAutomaton.h"

void test_find_longest() {
    std::string s = "abacabadabadabacabad";

    using PAM = OY::StaticAutomaton_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });
    cout << "longest palindromic string endsWith index 19 is: \n"
         << s.substr(19 - pam.get_node(pam.query_node_index(19))->m_length + 1, pam.get_node(pam.query_node_index(19))->m_length) << endl;

    cout << "longest palindromic string endsWith index 18 is: \n"
         << s.substr(18 - pam.get_node(pam.query_node_index(18))->m_length + 1, pam.get_node(pam.query_node_index(18))->m_length) << endl;

    cout << "longest palindromic string endsWith index 14 is: \n"
         << s.substr(14 - pam.get_node(pam.query_node_index(14))->m_length + 1, pam.get_node(pam.query_node_index(14))->m_length) << endl;
}

void test_find_all() {
    std::string s = "abacabadabadabacabad";

    using PAM = OY::StaticAutomaton_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto cur = pam.query_node_index(18);
    do {
        cout << s.substr(18 - pam.get_node(cur)->m_length + 1, pam.get_node(cur)->m_length) << endl;
        cur = pam.get_fail(cur);
    } while (cur > 2);
}

int main() {
    test_find_longest();
    test_find_all();
}
/*
#输出如下
longest palindromic string endsWith index 19 is:
dabacabad
longest palindromic string endsWith index 18 is:
abacabadabadabacaba
longest palindromic string endsWith index 14 is:
abadabadaba
abacabadabadabacaba
abacaba
aba
a

*/