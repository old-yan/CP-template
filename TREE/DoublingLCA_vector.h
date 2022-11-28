#ifndef __OY_DOUBLINGLCA_VECTOR__
#define __OY_DOUBLINGLCA_VECTOR__

#include <bit>

#include "VectorTree.h"

namespace OY {
    template <typename _Tree>
    struct DoublingLCA_vector {
        _Tree &m_tree;
        uint32_t m_level;
        std::vector<uint32_t> m_depth;
        std::vector<std::vector<uint32_t>> m_fa;
        DoublingLCA_vector(_Tree &__tree) : m_tree(__tree), m_depth(__tree.m_vertexNum) {
            uint32_t maxDepth = 0;
            auto dfs = [&](auto self, uint32_t i, uint32_t depth) -> void {
                chmax(maxDepth, m_depth[i] = depth++);
                for (auto &adj : m_tree.m_adj[i])
                    if (adj.to != __tree.m_parent[i]) self(self, adj.to, depth);
            };
            dfs(dfs, m_tree.m_root, 0);
            m_fa.resize(m_level = 32 - std::__countl_zero(maxDepth), m_tree.m_parent);
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