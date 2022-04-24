#ifndef __OY_ACAUTOMATON__
#define __OY_ACAUTOMATON__

#include "Trie.h"

namespace OY {
    struct PAMDefaultInfo {};
    template <typename _Mapping = TrieLowerMapping, typename _Info = PAMDefaultInfo, uint32_t _PoolSize = 2000000>
    struct PalindromicAutomaton {
        struct _PAMInfo : _Info {
            typename Trie<_Mapping, _PAMInfo, _PoolSize>::TrieNode m_fail;
            uint32_t m_length;
        };
        using TrieNode = typename Trie<_Mapping, _PAMInfo, _PoolSize>::TrieNode;
        static inline Trie<_Mapping, _PAMInfo, _PoolSize> s_trie;
        static inline TrieNode s_evenRoot;
        static inline TrieNode s_last;
        static void Init() {
            if (!~s_trie.m_root->m_length) s_trie = Trie<_Mapping, _PAMInfo, _PoolSize>();
            s_trie.m_root->m_length = -1;
            s_trie.m_root->m_fail = TrieNode(0);
            s_evenRoot = TrieNode::newNode(0);
            s_evenRoot->m_length = 0;
            s_evenRoot->m_fail = s_trie.m_root;
            s_last = s_trie.m_root;
            TrieNode(0)->m_length = -2;
            for (uint32_t i = 0; i < _Mapping::range(); i++) TrieNode(0).child(i) = s_evenRoot;
        }
        template <typename _Sequence>
        static TrieNode Insert(_Sequence &__sequence, uint32_t __index) {
            uint32_t i = s_trie.s_map(__sequence[__index]);
            s_last = Next(__sequence, __index, s_last);
            TrieNode &child = s_last.child(i);
            if (!child) {
                child = TrieNode::newNode(s_last);
                child->m_length = s_last->m_length + 2;
                child->m_fail = Next(__sequence, __index, s_last->m_fail).child(i);
            }
            return s_last = child;
        }
        template <typename _Sequence>
        static TrieNode Next(_Sequence &__sequence, uint32_t __index, TrieNode __cur) {
            if (!__cur) return __cur;
            while (__index == __cur->m_length || __sequence[__index - __cur->m_length - 1] != __sequence[__index]) __cur = __cur->m_fail;
            return __cur;
        }
    };
}

#endif