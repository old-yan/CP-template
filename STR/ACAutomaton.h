#ifndef __OY_ACAUTOMATON__
#define __OY_ACAUTOMATON__

#include "Trie.h"

namespace OY {
    struct ACDefaultInfo {};
    template <typename _Mapping = TrieLowerMapping, typename _Info = ACDefaultInfo, uint32_t _PoolSize = 1000000>
    struct ACAutomaton {
        struct _ACInfo : _Info {
            typename Trie<_Mapping, _ACInfo, _PoolSize>::TrieNode m_fail;
        };
        static inline Trie<_Mapping, _ACInfo, _PoolSize> s_trie;
        using TrieNode = typename Trie<_Mapping, _ACInfo, _PoolSize>::TrieNode;
        static void Init() {
            if (TrieNode(0).child(0)) s_trie = Trie<_Mapping, _ACInfo, _PoolSize>();
            for (uint32_t i = 0; i < _Mapping::range(); i++) TrieNode(0).child(i) = s_trie.m_root;
        }
        template <typename _Iterator>
        static TrieNode Insert(_Iterator __first, _Iterator __last) { return s_trie.insert(__first, __last); }
        static void Build() {
            static TrieNode queue[_PoolSize];
            uint32_t head = 0, tail = 0;
            queue[tail++] = s_trie.m_root;
            while (head < tail) {
                TrieNode cur = queue[head++];
                for (uint32_t i = 0; i < _Mapping::range(); i++)
                    if (TrieNode &child = cur.child(i)) {
                        child->m_fail = cur->m_fail.child(i);
                        queue[tail++] = child;
                    } else
                        child = cur->m_fail.child(i);
            }
        }
        template <typename _Iterator>
        static auto Iteration(_Iterator __first, _Iterator __last) {
            struct _Iterable {
                _Iterator first, last;
                struct iter {
                    _Iterator elem_iter;
                    mutable TrieNode node;
                    TrieNode operator*() const { return node = node.child(s_trie.s_map(*elem_iter)); }
                    bool operator!=(const iter &other) const { return elem_iter != other.elem_iter; }
                    iter &operator++() {
                        ++elem_iter;
                        return *this;
                    }
                };
                iter begin() const { return iter{first, s_trie.m_root}; }
                iter end() const { return iter{last, s_trie.m_root}; }
            };
            return _Iterable{__first, __last};
        }
    };
};

#endif