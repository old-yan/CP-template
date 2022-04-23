#ifndef __OY_SUFFIXTREE__
#define __OY_SUFFIXTREE__

#include <functional>
#include "Trie.h"

namespace OY {
    struct SuffixTreeDefaultInfo {};
    template <typename _Mapping = TrieLowerMapping, typename _Info = SuffixTreeDefaultInfo, uint32_t _PoolSize = 1000000>
    struct SuffixTree {
        struct _SuffixTreeInfo : _Info {
            uint32_t m_begin;
            uint32_t m_end;
            uint32_t m_index;
            uint32_t m_size;
            typename Trie<_Mapping, _SuffixTreeInfo, _PoolSize>::TrieNode m_link;
        };
        using TrieNode = typename Trie<_Mapping, _SuffixTreeInfo, _PoolSize>::TrieNode;
        static inline Trie<_Mapping, _SuffixTreeInfo, _PoolSize> s_trie;
        static inline uint32_t s_length;
        static inline TrieNode s_activeNode;
        static inline uint32_t s_activeChild;
        static inline uint32_t s_activeLength;
        static inline uint32_t s_leafEnd = -1;
        static inline uint32_t s_phase;
        static void Init() {
            if (!~s_trie.m_root->m_index) s_trie = Trie<_Mapping, _SuffixTreeInfo, _PoolSize>();
            s_trie.m_root->m_index = -1;
            s_length = 0;
            s_activeNode = s_trie.m_root;
            s_activeLength = 0;
            s_phase = 0;
        }
        template <typename _Iterator>
        static void Insert(_Iterator __first, _Iterator __last) {
            TrieNode waitLink;
            auto goForward = [&](TrieNode child) {
                if (s_activeLength >= child->m_end - child->m_begin) {
                    s_activeLength -= child->m_end - child->m_begin;
                    s_activeNode = child;
                    s_activeChild = s_trie.s_map(__first[s_length - s_activeLength]);
                    return true;
                } else
                    return false;
            };
            auto split = [&](TrieNode child, uint32_t index) {
                TrieNode p = TrieNode::newNode(s_activeNode, s_activeChild);
                p->m_begin = child->m_begin;
                p->m_end = child->m_begin + s_activeLength;
                p->m_index = index;
                child->m_begin += s_activeLength;
                p.child(s_trie.s_map(__first[child->m_begin])) = child;
                child.parent() = p;
                waitLink->m_link = p;
                return waitLink = p;
            };
            auto transform = [&] {
                s_phase++;
                if (s_activeNode != s_trie.m_root)
                    s_activeNode = s_activeNode->m_link;
                else if (s_activeLength) {
                    s_activeLength--;
                    s_activeChild = s_trie.s_map(__first[s_phase]);
                }
            };
            auto extend = [&](auto c) {
                waitLink = 0;
                while (s_phase <= s_length) {
                    if (!s_activeLength) s_activeChild = s_trie.s_map(c);
                    if (TrieNode child = s_activeNode.child(s_activeChild)) {
                        if (goForward(child)) continue;
                        if (__first[child->m_begin + s_activeLength] == c) {
                            waitLink->m_link = s_activeNode;
                            s_activeLength++;
                            break;
                        }
                        TrieNode q = split(child, s_leafEnd).childGet(s_trie.s_map(c));
                        std::tie(q->m_begin, q->m_end, q->m_index) = {s_length, s_leafEnd, s_phase};
                    } else {
                        TrieNode q = s_activeNode.childGet(s_activeChild);
                        std::tie(q->m_begin, q->m_end, q->m_index) = {s_length, s_leafEnd, s_phase};
                        waitLink->m_link = s_activeNode;
                        waitLink = 0;
                    }
                    transform();
                }
                s_length++;
            };
            for (auto it = __first; it != __last; ++it) extend(*it);
            waitLink = 0;
            while (s_phase < s_length) {
                if (!s_activeLength)
                    s_activeNode->m_index = s_phase;
                else if (TrieNode child = s_activeNode.child(s_activeChild)) {
                    if (goForward(child)) continue;
                    split(child, s_phase);
                }
                transform();
            }
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
                if (~p->m_index) p->m_size++;
                p.parent()->m_size += p->m_size;
                if (!--s_deg[p.parent().index]) s_queue[tail++] = p.parent();
            }
            TrieNode(0)->m_size = 0;
        }
        template <typename _Sequence, typename _Iterator>
        static auto Find(const _Sequence &__seq, _Iterator __first, _Iterator __last) {
            struct _Find_ans {
                TrieNode node;
                uint32_t length;
            };
            if (__first == __last) return _Find_ans{0, 0};
            TrieNode curNode = s_trie.m_root;
            uint32_t curLength = 0;
            for (auto it = __first; it != __last; ++it) {
                if (curLength == curNode->m_end - curNode->m_begin) {
                    if (TrieNode child = curNode.child(s_trie.s_map(*it))) {
                        curNode = child;
                        curLength = 1;
                        continue;
                    }
                } else if (__seq[curNode->m_begin + curLength] == *it) {
                    curLength++;
                    continue;
                }
                return _Find_ans{0, 0};
            }
            return _Find_ans{curNode, curLength};
        }
    };
    template <typename _Mapping = TrieLowerMapping, uint32_t _PoolSize = 2000000, bool log = true>
    struct GetSuffixArray_tree {
        using ST = SuffixTree<_Mapping, SuffixTreeDefaultInfo, _PoolSize>;
        using TrieNode = typename ST::TrieNode;
        static void dfs(TrieNode cur, std::vector<uint32_t> &sa) {
            if (~cur->m_index) sa.push_back(cur->m_index);
            for (uint32_t i = 0; i < _Mapping::range(); i++)
                if (TrieNode child = cur.child(i)) dfs(child, sa);
        }
        template <typename _Iterator>
        std::vector<uint32_t> operator()(_Iterator __first, _Iterator __last, uint32_t __alpha) const {
            ST::Init();
            ST::Insert(__first, __last);
            std::vector<uint32_t> sa;
            sa.reserve(__last - __first);
            dfs(ST::s_trie.m_root, sa);
            return sa;
        }
    };
}

#endif