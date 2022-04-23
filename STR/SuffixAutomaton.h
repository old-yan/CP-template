#ifndef __OY_SUFFIXAUTOMATON__
#define __OY_SUFFIXAUTOMATON__

#include <functional>
#include "Trie.h"

namespace OY {
    struct SAMDefaultInfo {};
    template <typename _Mapping = TrieLowerMapping, typename _Info = SAMDefaultInfo, uint32_t _PoolSize = 2000000>
    struct SuffixAutomaton {
        struct _SAMInfo : _Info {
            uint32_t m_size;
            uint32_t m_length;
        };
        using TrieNode = typename Trie<_Mapping, _SAMInfo, _PoolSize>::TrieNode;
        static inline Trie<_Mapping, _SAMInfo, _PoolSize> s_trie;
        static inline TrieNode s_end;
        static void Init() {
            if (!~TrieNode(0).child(0)) s_trie = Trie<_Mapping, _SAMInfo, _PoolSize>();
            std::fill(s_trie.s_child[0], s_trie.s_child[1], -1);
            s_end = s_trie.m_root;
        }
        template <typename _Tp>
        static TrieNode Insert(_Tp __c) {
            TrieNode oldEnd = s_end;
            s_end = oldEnd.childGet(Trie<_Mapping, _SAMInfo, _PoolSize>::s_map(__c));
            s_end->m_length = oldEnd->m_length + 1;
            s_end->m_size = 1;
            while (true) {
                oldEnd = oldEnd.parent();
                if (TrieNode &child = oldEnd.child(Trie<_Mapping, _SAMInfo, _PoolSize>::s_map(__c)))
                    break;
                else
                    child = s_end;
            }
            if (!oldEnd)
                s_end.parent() = s_trie.m_root;
            else {
                TrieNode q = oldEnd.child(Trie<_Mapping, _SAMInfo, _PoolSize>::s_map(__c));
                if (q->m_length == oldEnd->m_length + 1)
                    s_end.parent() = q;
                else {
                    TrieNode newq = s_trie.s_cursor++;
                    newq.parent() = q.parent();
                    std::copy(s_trie.s_child[q.index], s_trie.s_child[q.index + 1], s_trie.s_child[newq]);
                    newq->m_length = oldEnd->m_length + 1;
                    q.parent() = s_end.parent() = newq;
                    while (oldEnd.child(Trie<_Mapping, _SAMInfo, _PoolSize>::s_map(__c)) == q) {
                        oldEnd.child(Trie<_Mapping, _SAMInfo, _PoolSize>::s_map(__c)) = newq;
                        oldEnd = oldEnd.parent();
                    }
                }
            }
            return s_end;
        }
        static void Build() {
            static uint32_t s_deg[_PoolSize];
            static TrieNode s_queue[_PoolSize];
            uint32_t head = 0, tail = 0;
            for (uint32_t i = s_trie.m_root.index; i < s_trie.s_cursor; i++) s_deg[TrieNode(i).parent()]++;
            for (uint32_t i = s_trie.m_root.index; i < s_trie.s_cursor; i++)
                if (!s_deg[i]) s_queue[tail++] = i;
            while (head < tail) {
                TrieNode p = s_queue[head++];
                p.parent()->m_size += p->m_size;
                if (!--s_deg[p.parent().index]) s_queue[tail++] = p.parent();
            }
            TrieNode(0)->m_size = 0;
        }
        template <typename _Iterator>
        static TrieNode Find(_Iterator __first, _Iterator __last) {
            if (__first == __last) return s_trie.m_root;
            TrieNode cur = s_trie.m_root;
            for (auto it = __first; it != __last; ++it) {
                if (cur = cur.child(s_trie.s_map(*it))) continue;
                return TrieNode(0);
            }
            return cur;
        }
    };
    template <typename _Mapping = TrieLowerMapping, uint32_t _PoolSize = 2000000>
    struct GetSuffixArray_sam {
        struct IndexInfo {
            uint32_t m_index;
        };
        using SAM = SuffixAutomaton<_Mapping, IndexInfo, _PoolSize>;
        using TrieNode = typename SAM::TrieNode;
        static inline uint32_t stack[_PoolSize];
        static inline uint32_t stackLen{};
        static inline TrieNode tree[_PoolSize][_Mapping::range()];
        static void makeTree(TrieNode cur) {
            for (uint32_t i = 0; i < _Mapping::range(); i++)
                if (TrieNode child = cur.child(i))
                    if (child->m_length == cur->m_length + 1) {
                        stack[stackLen++] = i;
                        tree[child.parent()][stack[stackLen - child.parent()->m_length - 1]] = child;
                        makeTree(child);
                        stackLen--;
                    }
        }
        static void preOrderTraverse(TrieNode cur, std::vector<uint32_t> &sa) {
            if (cur->m_index) sa.push_back(cur->m_index - 1);
            for (uint32_t i = 0; i < _Mapping::range(); i++)
                if (TrieNode child = tree[cur][i]) preOrderTraverse(child, sa);
        }
        template <typename _Iterator>
        std::vector<uint32_t> operator()(_Iterator __first, _Iterator __last, uint32_t __alpha) const {
            SAM::Init();
            for (uint32_t i = __last - __first - 1; ~i; i--) SAM::Insert(__first[i])->m_index = i + 1;
            makeTree(SAM::s_trie.m_root);
            std::vector<uint32_t> sa;
            sa.reserve(__last - __first);
            preOrderTraverse(SAM::s_trie.m_root, sa);
            return sa;
        }
    };
}

#endif