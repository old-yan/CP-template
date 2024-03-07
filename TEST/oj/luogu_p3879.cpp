#include "IO/FastIO.h"
#include "DS/FHQTreap.h"
#include "STR/Trie.h"

/*
[P3879 [TJOI2010] 阅读理解](https://www.luogu.com.cn/problem/P3879)
*/
// 显然，本题为字典树模板题
// 只要在插入的字符串的结点上做标记，就可以知道这个结点的出现位置

using FHQ = OY::FHQTreap<uint32_t, std::less<uint32_t>, 5000001>;
template <typename Node>
struct NodeWrap {
    FHQ m_pos;
};
void solve_trie() {
    using Trie = OY::StaticTrie<NodeWrap, 26>;
    Trie S;
    uint32_t n;
    cin >> n;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t k;
        cin >> k;
        while (k--) {
            std::string s;
            cin >> s;
            auto it = S.insert_lower(s);
            if (!it->m_pos.modify_by_key(i, [](auto...) {})) it->m_pos.insert_by_key(i);
        }
    }
    uint32_t m;
    cin >> m;
    while (m--) {
        std::string s;
        cin >> s;
        auto it = S.find_lower(s);
        if (it)
            it->m_pos.do_for_each([](auto p) {
                cout << p->get() << ' ';
            });
        cout << endl;
    }
}

int main() {
    solve_trie();
}