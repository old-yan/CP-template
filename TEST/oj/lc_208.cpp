#include "IO/LeetcodeIO.h"
#include "STR/Trie.h"
using namespace std;

/*
[208. 实现 Trie (前缀树)](https://leetcode.cn/problems/implement-trie-prefix-tree)
*/
/**
 * 显然，本题为模板题
*/

template <typename Node>
struct NodeWrap {
    bool m_is_end;
};
using MyTrie = OY::StaticTrie<NodeWrap>;
class Trie {
    MyTrie m_inner;

public:
    void insert(string word) {
        m_inner.insert_lower(word)->m_is_end = true;
    }
    bool search(string word) {
        auto it = m_inner.find_lower(word);
        return it && it->m_is_end;
    }
    bool startsWith(string prefix) {
        return m_inner.find_lower(prefix);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_CLASS(Trie);
    REGISTER_MEMBERFUNCTION_CLASS(Trie, insert, search, startsWith);

    while (true) {
        executor.constructClass();
        while (executor) {
            executor.executeClass();
        }
    }
}
#endif