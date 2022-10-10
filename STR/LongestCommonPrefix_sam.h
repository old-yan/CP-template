#ifndef __OY_LONGESTCOMMONPREFIX_SAM__
#define __OY_LONGESTCOMMONPREFIX_SAM__

#include "../TREE/HeavyLightDecompositionLCA.h"
#include "SuffixAutomaton.h"

namespace OY {
    template <typename _Tp = char, uint32_t _MAXN = 1000000, typename _Mapping = TrieLowerMapping, template <typename...> typename _Solver = HeavyLightDecompositionLCA>
    struct LongestCommonPrefix_sam {
        using SAM = SuffixAutomaton<_Mapping, SAMDefaultInfo, _MAXN * 2>;
        using TrieNode = SAM::TrieNode;
        uint32_t m_length, m_rootIndex;
        std::vector<uint32_t> m_nodelength, m_pos;
        std::basic_string<_Tp> m_text;
        Tree<_MAXN, bool> m_tree;
        _Solver<Tree<_MAXN, bool>> m_lca;
        template <typename _Iterator>
        Tree<_MAXN, bool> _initTree(_Iterator __first, _Iterator __last) {
            m_pos.resize(m_length = __last - __first);
            SAM::Init();
            m_rootIndex = SAM::s_trie.m_root.index;
            for (uint32_t i = m_length - 1; ~i; i--) m_pos[i] = SAM::Insert(*(__first + i)).index - m_rootIndex;
            m_nodelength.reserve(SAM::s_trie.s_cursor - m_rootIndex);
            Tree<_MAXN, bool> tree(SAM::s_trie.s_cursor - m_rootIndex);
            for (uint32_t i = m_rootIndex; i < SAM::s_trie.s_cursor; i++) {
                m_nodelength.push_back(TrieNode(i)->m_length);
                if (uint32_t p = TrieNode(i).parent().index) tree.addEdge(i - m_rootIndex, p - m_rootIndex);
            }
            tree.prepare();
            tree.setRoot(0);
            return tree;
        }
        template <typename _Iterator>
        LongestCommonPrefix_sam(_Iterator __first, _Iterator __last) : m_tree(_initTree(__first, __last)), m_lca(m_tree), m_text(__first, __last) {}
        uint32_t lcp(uint32_t __a, uint32_t __b, uint32_t __limit) const { return std::min(__limit, m_nodelength[m_lca.calc(m_pos[__a], m_pos[__b])]); }
        uint32_t lcp(uint32_t __a, uint32_t __b) const { return m_nodelength[m_lca.calc(m_pos[__a], m_pos[__b])]; }
        int compare(uint32_t __l1, uint32_t __r1, uint32_t __l2, uint32_t __r2) const {
            uint32_t len1 = __r1 - __l1 + 1, len2 = __r2 - __l2 + 1, len = lcp(__l1, __l2, std::min(len1, len2));
            if (len == len1)
                return len == len2 ? 0 : -1;
            else if (len == len2)
                return 1;
            else
                return m_text[__l1 + len] < m_text[__l2 + len] ? -1 : 1;
        }
    };
}

#endif