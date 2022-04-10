#ifndef __OY_TRIE__
#define __OY_TRIE__

#include "../DS/MemoryPool.h"

namespace OY {
    struct TrieDigitMapping {
        static constexpr int range(){return 10;}
        int operator()(char __c) const { return __c - '0'; }
    };
    struct TrieLowerMapping {
        static constexpr int range(){return 26;}
        int operator()(char __c) const { return __c - 'a'; }
    };
    struct TrieUpperMapping {
        static constexpr int range(){return 26;}
        int operator()(char __c) const { return __c - 'A'; }
    };
    struct TrieDefaultBase {
        bool m_isEnd = false;
    };
    template <typename _Mapping = TrieLowerMapping, typename _Base = TrieDefaultBase>
    struct TrieNode : _Base {
        using node = TrieNode<_Mapping, _Base>;
        static inline _Mapping s_map = _Mapping();
        static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
        static void operator delete(void *p) { MemoryPool<node>::operator delete(p); }
        node *m_parent;
        node *m_child[_Mapping::range()];
        TrieNode(node *__parent = nullptr) : m_parent(__parent), m_child{} {}
        node *&child(int __i) {
            if (!m_child[__i]) m_child[__i] = new node(this);
            return m_child[__i];
        }
        template <typename _Iterator>
        node *insert(_Iterator __first, _Iterator __last) {
            if (__first == __last)
                return this;
            else
                return child(s_map(*__first))->insert(__first + 1, __last);
        }
        template <typename _Sequence>
        node *insert(const _Sequence &__sequence) { return insert(__sequence.begin(), __sequence.end()); }
        template <typename _Iterator>
        node *find(_Iterator __first, _Iterator __last) const {
            if (__first == __last)
                return (node *)(this);
            else if (node *child = m_child[s_map(*__first)]; child)
                return child->find(__first + 1, __last);
            else
                return nullptr;
        }
        template <typename _Sequence>
        node *find(const _Sequence &__sequence) const { return find(__sequence.begin(), __sequence.end()); }
    };
}

#endif