#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/StrHash.h"
#include "STR/Trie.h"

/*
[P8306 【模板】字典树](https://www.luogu.com.cn/problem/P8306)
*/
// 显然，本题为字典树模板题
// 只要把插入的字符串的插入路径都加一，就可以让每个结点的值等于以其为前缀的串数
// 同样的，字符串哈希也可以解决本题

template <typename Node>
struct NodeWrap {
    uint32_t m_cnt;
};
void solve_trie() {
    using Trie = OY::StaticTrie<NodeWrap, 62>;
    uint32_t t;
    cin >> t;
    Trie S;
    S.reserve(3000000);
    uint32_t map_id[128];
    for (uint32_t i = 0; i < 26; i++) map_id['a' + i] = i;
    for (uint32_t i = 0; i < 26; i++) map_id['A' + i] = 26 + i;
    for (uint32_t i = 0; i < 10; i++) map_id['0' + i] = 52 + i;
    auto mapping = [&](char c) {
        return map_id[c];
    };

    while (t--) {
        uint32_t n, q;
        cin >> n >> q;
        S.clear();
        for (uint32_t i = 0; i < n; i++) {
            std::string s;
            cin >> s;
            S.insert(
                s.size(), [&](uint32_t i) { return mapping(s[i]); }, [](auto p) { p->m_cnt++; });
        }
        while (q--) {
            std::string s;
            cin >> s;
            auto it = S.find(s.size(), [&](uint32_t i) { return mapping(s[i]); });
            if (!it)
                cout << "0\n";
            else
                cout << it->m_cnt << endl;
        }
    }
}

using mint = OY::StaticModInt32<2000000011, true>;
using table_type = OY::STRHASH::StrHashPresumTable<mint, 128>;
using hash_type = table_type::hash_type;
namespace OY {
    namespace GHASH {
        template <size_type L>
        struct Hash<hash_type, L> {
            size_type operator()(const auto &x) const { return Hash<size_t, L>()(*(size_t *)(&x)); }
        };
    }
}
OY::GHASH::UnorderedMap<hash_type, uint32_t, true, 21> GS;
void solve_hash() {
    uint32_t t;
    cin >> t;
    hash_type::s_info.prepare_unit(3000000);
    while (t--) {
        uint32_t n, q;
        cin >> n >> q;
        GS.clear();
        for (uint32_t i = 0; i < n; i++) {
            std::string s;
            cin >> s;
            hash_type x{};
            for (char c : s) {
                auto [ptr, flag] = GS.insert(x = x.append_right(hash_type::single(c)));
                if (flag)
                    ptr->m_mapped = 1;
                else
                    ptr->m_mapped++;
            }
        }
        while (q--) {
            std::string s;
            cin >> s;
            cout << GS.get(s, 0) << endl;
        }
    }
}

int main() {
    solve_trie();
    // solve_hash();
}