#ifndef __OY_HEAVYLIGHTDECOMPOSITIONLCA__
#define __OY_HEAVYLIGHTDECOMPOSITIONLCA__

#include <bitset>

#include "Tree.h"

namespace OY {
    template<typename _Tree>
    struct HeavyLightDecompositionLCA {
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        _Tree &m_tree;
        uint32_t m_size[_MAXN];
        uint32_t m_heavySon[_MAXN];
        uint32_t m_linkId[_MAXN];
        uint32_t m_linkDepth[_MAXN];
        uint32_t m_linkParent[_MAXN];
        HeavyLightDecompositionLCA(_Tree &__tree) : m_tree(__tree) {
            auto dfs = [&](auto self, uint32_t i) -> void {
                m_size[i] = 1;
                m_heavySon[i] = -1;
                uint32_t heavy = 0;
                for (uint32_t cur = m_tree.m_starts[i] + (i != m_tree.m_root), end = m_tree.m_starts[i + 1]; cur != end; cur++) {
                    uint32_t to = m_tree.m_to[cur];
                    self(self, to);
                    m_size[i] += m_size[to];
                    if (chmax(heavy, m_size[to])) m_heavySon[i] = to;
                }
            };
            dfs(dfs, m_tree.m_root);
            uint32_t cursor = 0;
            std::bitset<_MAXN> visit;
            auto dfs2 = [&](auto self, uint32_t i, uint32_t d, uint32_t ld, uint32_t p) -> void {
                visit.set(i);
                m_linkId[i] = cursor;
                m_linkDepth[i] = ld;
                m_linkParent[i] = p;
                if (!~m_heavySon[i]) {
                    cursor++;
                    return;
                }
                self(self, m_heavySon[i], ++d, ld, p);
                for (uint32_t cur = m_tree.m_starts[i], end = m_tree.m_starts[i + 1]; cur != end; cur++)
                    if (uint32_t to = m_tree.m_to[cur]; !visit[to]) self(self, to, d, d, i);
            };
            dfs2(dfs2, m_tree.m_root, 0, 0, -1);
        }
        uint32_t calc(uint32_t __a, uint32_t __b) const {
            while (m_linkId[__a] != m_linkId[__b])
                if (m_linkDepth[__a] < m_linkDepth[__b])
                    __b = m_linkParent[__b];
                else
                    __a = m_linkParent[__a];
            return m_size[__a] < m_size[__b] ? __b : __a;
        }
    };
}

#endif