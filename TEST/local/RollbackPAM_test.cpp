#include "IO/FastIO.h"
#include "STR/RollbackPAM.h"

#include <map>

void test_find_longest() {
    cout << "test find longest:\n";

    std::string s = "abacabadabadabacabad";
    cout << "original string:\n ["
         << s << "] \n";
    using PAM = OY::StaticRollbackPAM_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto show_endwith = [&](int pos) {
        int len = pam.get_node(pam.query_node_index(pos))->m_length;
        cout << "longest palindromic string endsWith index " << pos << " is: \n"
             << s.substr(0, pos + 1 - len) << " [" << s.substr(pos + 1 - len, len) << "] " << s.substr(pos + 1) << endl;
    };

    show_endwith(19);
    show_endwith(18);
    show_endwith(14);
    show_endwith(8);

    cout << endl;
}

void test_find_all() {
    cout << "test find all:\n";
    std::string s = "abacabadabadabacabad";
    cout << "original string:\n ["
         << s << "] \n";
    using PAM = OY::StaticRollbackPAM_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto show_endwith = [&](int pos) {
        cout << "palindromic substr endsWidth index " << pos << ":\n";
        auto cur = pam.query_node_index(pos);
        do {
            auto len = pam.get_node(cur)->m_length;
            cout << s.substr(0, pos + 1 - len) << " [" << s.substr(pos + 1 - len, len) << "] " << s.substr(pos + 1) << endl;
            cur = pam.query_fail(cur);
        } while (pam.is_node(cur));
    };
    show_endwith(18);
    show_endwith(14);
    cout << endl;
}

void test_find_serieses() {
    cout << "test find serieses:\n";
    std::string u = "abcba";
    std::string u2 = "bab" + u + u + u;
    std::string s = u2 + u2 + u2 + u2 + "zya";
    cout << "original string:\n ["
         << s << "] \n";
    using PAM = OY::StaticRollbackPAM_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto show_series = [&](int pos) {
        cout << "palindromic substr endsWidth index " << pos << ":\n";
        auto print = [&](PAM::series x) {
            int longest = x.m_longest;
            int shortest = x.m_shortest;
            int delta = x.m_delta;
            for (int len = shortest; len <= longest; len += delta) {
                cout << s.substr(0, pos + 1 - len) << " [" << s.substr(pos + 1 - len, len) << "] " << s.substr(pos + 1) << endl;
            }
        };
        pam.do_for_each_series(pos, print);
    };
    // 可以看到，以下标 71 位置为结尾的对称子串长度，形成了三个 series(等差数列)
    show_series(71);
    cout << endl;
}

struct Node {
    std::map<uint32_t, uint32_t> m_child;
    void add_child(uint32_t index, uint32_t child) { m_child[index] = child; }
    void remove_child(uint32_t index) { m_child.erase(index); }
    uint32_t get_child(uint32_t index) const {
        auto it = m_child.find(index);
        return it == m_child.end() ? 0 : it->second;
    }
};
void test_map_node() {
    cout << "test map node:\n";

    std::string s = "abacabadabadabacabad";
    cout << "original string:\n ["
         << s << "] \n";
    using PAM = OY::RollbackPAM::Automaton<Node, std::string, 10>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto show_endwith = [&](int pos) {
        int len = pam.get_node(pam.query_node_index(pos))->m_length;
        cout << "longest palindromic string endsWith index " << pos << " is: \n"
             << s.substr(0, pos + 1 - len) << " [" << s.substr(pos + 1 - len, len) << "] " << s.substr(pos + 1) << endl;
    };

    show_endwith(19);
    show_endwith(18);
    show_endwith(14);
    show_endwith(8);

    cout << endl;
}

int main() {
    test_find_longest();
    test_find_all();
    test_find_serieses();
    test_map_node();
}
/*
#输出如下
test find longest:
original string:
 [abacabadabadabacabad] 
longest palindromic string endsWith index 19 is: 
abacabadaba [dabacabad] 
longest palindromic string endsWith index 18 is: 
 [abacabadabadabacaba] d
longest palindromic string endsWith index 14 is: 
abac [abadabadaba] cabad
longest palindromic string endsWith index 8 is: 
abacab [ada] badabacabad

test find all:
original string:
 [abacabadabadabacabad] 
palindromic substr endsWidth index 18:
 [abacabadabadabacaba] d
abacabadabad [abacaba] d
abacabadabadabac [aba] d
abacabadabadabacab [a] d
palindromic substr endsWidth index 14:
abac [abadabadaba] cabad
abacabad [abadaba] cabad
abacabadabad [aba] cabad
abacabadabadab [a] cabad

test find serieses:
original string:
 [bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbazya] 
palindromic substr endsWidth index 71:
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabab [abcbaabcbaabcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabab [abcbaabcbaabcbabababcbaabcbaabcba] zya
bababcbaabcbaabcbabab [abcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcba] zya
bab [abcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcba [abcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcba [abcbaabcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabab [abcbaabcbaabcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcb [a] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcba [abcba] zya

test map node:
original string:
 [abacabadabadabacabad] 
longest palindromic string endsWith index 19 is: 
abacabadaba [dabacabad] 
longest palindromic string endsWith index 18 is: 
 [abacabadabadabacaba] d
longest palindromic string endsWith index 14 is: 
abac [abadabadaba] cabad
longest palindromic string endsWith index 8 is: 
abacab [ada] badabacabad

*/