#ifndef __OY_LONGSHORTDECOMPOSITIONLCA_VECTOR__
#define __OY_LONGSHORTDECOMPOSITIONLCA_VECTOR__

#include "VectorTree.h"

namespace OY {
    template <typename _Tree>
    struct LongShortDecompositionLCA_vector {
        _Tree &m_tree;
        std::vector<uint32_t> m_height, m_depth, m_heavySon, m_belongTopVal, m_upDown;
        std::vector<std::vector<uint32_t>> m_fa;
        LongShortDecompositionLCA_vector(_Tree &__tree) : m_tree(__tree), m_height(__tree.m_vertexNum), m_depth(__tree.m_vertexNum), m_heavySon(__tree.m_vertexNum), m_belongTopVal(__tree.m_vertexNum), m_upDown(__tree.m_vertexNum * 2) {
            auto getHeavy = [&](auto self, uint32_t i, uint32_t d) -> void {
                m_height[i] = 1;
                m_depth[i] = d++;
                m_heavySon[i] = -1;
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i])
                        if (self(self, to, d); chmax(m_height[i], m_height[to] + 1)) m_heavySon[i] = to;
            };
            getHeavy(getHeavy, m_tree.m_root, 0);
            std::vector<bool> visit(m_tree.m_vertexNum);
            uint32_t cursor = 0, level = 32 - std::__countl_zero(*std::max_element(m_depth.begin(), m_depth.end()));
            m_fa.resize(level, m_tree.m_parent);
            for (uint32_t j = 1; j < level; j++)
                for (uint32_t i = 0; i < m_tree.m_vertexNum; i++) m_fa[j][i] = ~m_fa[j - 1][i] ? m_fa[j - 1][m_fa[j - 1][i]] : -1;
            auto setTop = [&](uint32_t top) {
                uint32_t h = m_height[top];
                for (uint32_t k = cursor + h, kend = cursor + h * 2, cur = top; k < kend; k++, cur = m_heavySon[cur]) m_upDown[k] = cur;
                for (uint32_t k = cursor + h - 1, kend = cursor + 1, cur = top; k > kend && ~cur; k--) m_upDown[k] = cur = m_tree.m_parent[cur];
                return h + cursor - m_depth[top];
            };
            auto getOrder = [&](auto self, uint32_t i, uint32_t topVal) -> void {
                visit[i] = true;
                m_belongTopVal[i] = topVal;
                cursor += 2;
                if (!~m_heavySon[i]) return;
                self(self, m_heavySon[i], topVal);
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; !visit[to]) self(self, to, setTop(to));
            };
            getOrder(getOrder, m_tree.m_root, setTop(m_tree.m_root));
        }
        uint32_t getAncestor(uint32_t __a, uint32_t __n) const {
            if (!__n) return __a;
            return m_upDown[m_belongTopVal[m_fa[31 - std::__countl_zero(__n)][__a]] + (m_depth[__a] - __n)];
        }
        uint32_t calc(uint32_t __a, uint32_t __b) const {
            if (m_depth[__a] > m_depth[__b]) std::swap(__a, __b);
            uint32_t d = m_depth[__b] - m_depth[__a], low = 0, high = m_depth[__a], p1 = __a, p2 = __a;
            while (low < high) {
                uint32_t mid = (low + high) / 2;
                if (p1 = getAncestor(__a, mid), p2 = getAncestor(__b, mid + d); p1 != p2)
                    low = mid + 1;
                else
                    high = mid;
            }
            return p1 == p2 ? p1 : m_tree.m_parent[p1];
        }
    };
}

#endif