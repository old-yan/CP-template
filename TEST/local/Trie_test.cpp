#include "IO/FastIO.h"
#include "STR/Trie.h"

void test() {
    // 最简单的字典树，可以插入字符串，可以查找字符串、查找前缀
    std::string words[] = {"a", "app", "banana", "apple", "banned"};
    // 在默认情况下，会将 'a'~'z' 映射到 [0, 25] 作为孩子结点的下标
    OY::StaticTrie<> trie;
    for (std::string &word : words) {
        auto it = trie.insert_lower(word);
        it->m_is_end = true;
    }
    auto it = trie.find_lower(std::string("apple"));
    if (it->m_is_end)
        cout << "\"apple\" in the Trie\n";
    it = trie.find_lower(std::string("apps"));
    if (!it or !it->m_is_end)
        cout << "\"apps\" not in the Trie\n";
    it = trie.find_lower(std::string("ban"));
    if (it and !it->m_is_end)
        cout << "\"ban\" in the Trie as a prefix\n";
    cout << endl;
}

int main() {
    test();
}
/*
#输出如下
"apple" in the Trie
"apps" not in the Trie
"ban" in the Trie as a prefix

*/