#ifndef __OY_DOUBLINGLCA__
#define __OY_DOUBLINGLCA__

#include "Tree.h"

namespace OY {
    template <typename _Tree>
    struct DoublingLCA {
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        _Tree &m_tree;
        uint32_t m_level;
        uint32_t m_depth[_MAXN];
        uint32_t m_fa[32 - std::__countl_zero(_MAXN - 1)][_MAXN];
        DoublingLCA(_Tree &__tree) : m_tree(__tree) {
            uint32_t maxDepth = 0;
            auto dfs = [&](auto self, uint32_t i, uint32_t depth, uint32_t p) -> void {
                chmax(maxDepth, depth);
                m_depth[i] = depth;
                m_fa[0][i] = p;
                for (uint32_t cur = m_tree.m_starts[i] + (i != m_tree.m_root), end = m_tree.m_starts[i + 1]; cur != end; cur++) self(self, m_tree.m_to[cur], depth + 1, i);
            };
            dfs(dfs, m_tree.m_root, 0, -1);
            m_level = 32 - std::__countl_zero(maxDepth);
            for (uint32_t j = 1; j < m_level; j++)
                for (uint32_t i = 0; i < m_tree.m_vertexNum; i++) m_fa[j][i] = ~m_fa[j - 1][i] ? m_fa[j - 1][m_fa[j - 1][i]] : -1;
        }
        uint32_t getAncestor(uint32_t __a, uint32_t __n) const {
            if (__n > m_depth[__a]) return -1;
            while (__n) {
                uint32_t k = std::__countr_zero(__n);
                __a = m_fa[k][__a];
                __n -= uint32_t(1) << k;
            }
            return __a;
        }
        uint32_t calc(uint32_t __a, uint32_t __b) {
            if (m_depth[__a] > m_depth[__b]) std::swap(__a, __b);
            if (uint32_t x = m_depth[__b] - m_depth[__a]; x)
                for (uint32_t j = 0; x; j++, x >>= 1)
                    if (x & 1) __b = m_fa[j][__b];
            if (__a == __b) return __a;
            for (uint32_t j = m_level - 1; ~j; j--)
                if (m_fa[j][__a] != m_fa[j][__b]) {
                    __a = m_fa[j][__a];
                    __b = m_fa[j][__b];
                }
            return m_fa[0][__a];
        }
    };
}

#endif