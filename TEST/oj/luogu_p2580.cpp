#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/Trie.h"

/*
[P2580 于是他错误的点名开始了](https://www.luogu.com.cn/problem/P2580)
*/
// 显然，本题只需要一个去重集合，我们这里用字典树
// 当然，库的哈希是最快的

OY::GHASH::UnorderedMap<std::string, bool, false, 16> GS;
void solve_hash() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        GS.insert(s);
    }
    uint32_t m;
    cin >> m;
    while (m--) {
        std::string s;
        cin >> s;
        auto it = GS.find(s);
        if (!it)
            cout << "WRONG\n";
        else if (!it->m_mapped)
            it->m_mapped = true, cout << "OK\n";
        else
            cout << "REPEAT\n";
    }
}

template <typename Node>
struct NodeWrap {
    uint8_t m_flag;
};
void solve_trie() {
    using Trie = OY::StaticTrie<NodeWrap>;
    Trie S;
    uint32_t n;
    cin >> n;
    S.reserve(50 * n);
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        S.insert_lower(s)->m_flag = 1;
    }
    uint32_t m;
    cin >> m;
    while (m--) {
        std::string s;
        cin >> s;
        auto it = S.find_lower(s);
        if (!it || !it->m_flag)
            cout << "WRONG\n";
        else if (it->m_flag == 1)
            it->m_flag = 2, cout << "OK\n";
        else
            cout << "REPEAT\n";
    }
}

int main() {
    solve_hash();
    // solve_trie();
}