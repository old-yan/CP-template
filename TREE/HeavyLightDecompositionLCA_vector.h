#ifndef __OY_HEAVYLIGHTDECOMPOSITIONLCA_VECTOR__
#define __OY_HEAVYLIGHTDECOMPOSITIONLCA_VECTOR__

#include "VectorTree.h"

namespace OY {
    template <typename _Tree>
    struct HeavyLightDecompositionLCA_vector {
        struct _LinkInfo {
            uint32_t id, depth, parent;
        };
        _Tree &m_tree;
        std::vector<uint32_t> m_size, m_heavySon;
        std::vector<_LinkInfo> m_linkInfo;
        HeavyLightDecompositionLCA_vector(_Tree &__tree) : m_tree(__tree), m_size(__tree.m_vertexNum), m_heavySon(__tree.m_vertexNum), m_linkInfo(__tree.m_vertexNum) {
            auto dfs = [&](auto self, uint32_t i) -> void {
                m_size[i] = 1;
                m_heavySon[i] = -1;
                uint32_t heavy = 0;
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i]) {
                        self(self, to);
                        m_size[i] += m_size[to];
                        if (chmax(heavy, m_size[to])) m_heavySon[i] = to;
                    }
            };
            dfs(dfs, m_tree.m_root);
            uint32_t cursor = 0;
            std::vector<bool> visit(m_tree.m_vertexNum, false);
            auto dfs2 = [&](auto self, uint32_t i, uint32_t d, uint32_t ld, uint32_t p) -> void {
                visit[i] = true;
                m_linkInfo[i] = {cursor, ld, p};
                if (!~m_heavySon[i]) {
                    cursor++;
                    return;
                }
                self(self, m_heavySon[i], ++d, ld, p);
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; !visit[to]) self(self, to, d, d, i);
            };
            dfs2(dfs2, m_tree.m_root, 0, 0, -1);
        }
        uint32_t calc(uint32_t __a, uint32_t __b) const {
            while (m_linkInfo[__a].id != m_linkInfo[__b].id)
                if (m_linkInfo[__a].depth < m_linkInfo[__b].depth)
                    __b = m_linkInfo[__b].parent;
                else
                    __a = m_linkInfo[__a].parent;
            return m_size[__a] < m_size[__b] ? __b : __a;
        }
    };
}

#endif