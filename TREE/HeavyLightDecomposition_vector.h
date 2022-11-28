#ifndef __OY_HEAVYLIGHTDECOMPOSITION_VECTOR__
#define __OY_HEAVYLIGHTDECOMPOSITION_VECTOR__

#include "VectorTree.h"

namespace OY {
    template <typename _Tree>
    struct HeavyLightDecomposition_vector {
        _Tree &m_tree;
        std::vector<uint32_t> m_size, m_depth, m_heavySon, m_pos, m_sequence, m_belong, m_linkTop;
        uint32_t m_linkCount;
        HeavyLightDecomposition_vector(_Tree &__tree) : m_tree(__tree), m_size(__tree.m_vertexNum), m_depth(__tree.m_vertexNum), m_heavySon(__tree.m_vertexNum), m_pos(__tree.m_vertexNum), m_sequence(__tree.m_vertexNum), m_belong(__tree.m_vertexNum), m_linkTop(__tree.m_vertexNum) {
            auto dfs = [&](auto self, uint32_t i, uint32_t d) -> void {
                m_size[i] = 1;
                m_depth[i] = d++;
                m_heavySon[i] = -1;
                uint32_t heavy = 0;
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i]) {
                        self(self, to, d);
                        m_size[i] += m_size[to];
                        if (chmax(heavy, m_size[to])) m_heavySon[i] = to;
                    }
            };
            dfs(dfs, m_tree.m_root, 0);
            std::vector<bool> visit(m_tree.m_vertexNum, false);
            uint32_t cursor = 0;
            m_linkCount = 0;
            auto dfs2 = [&](auto self, uint32_t i) -> void {
                visit[i] = true;
                m_sequence[cursor] = i;
                m_pos[i] = cursor++;
                m_belong[i] = m_linkCount;
                if (!~m_heavySon[i]) {
                    m_linkCount++;
                    return;
                }
                self(self, m_heavySon[i]);
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; !visit[to]) self(self, m_linkTop[m_linkCount] = to);
            };
            dfs2(dfs2, m_linkTop[m_linkCount] = m_tree.m_root);
        }
        std::basic_string_view<uint32_t> getSequence() const { return std::basic_string_view<uint32_t>(m_sequence.begin(), m_sequence.begin()); }
        uint32_t getAncestor(uint32_t __a, uint32_t __n) const {
            if (__n > m_depth[__a]) return -1;
            uint32_t depth = m_depth[__a], targetDepth = m_depth[__a] - __n, top;
            while (true) {
                top = m_linkTop[m_belong[__a]];
                if (m_depth[top] > targetDepth) {
                    depth = m_depth[top] - 1;
                    __a = m_tree.m_parent[top];
                } else
                    break;
            }
            return m_sequence[m_pos[__a] - (depth - targetDepth)];
        }
        auto getLinks(uint32_t __a, uint32_t __b) const {
            struct _range {
                uint32_t start, end;
            };
            std::vector<_range> res;
            while (true) {
                uint32_t atop = m_linkTop[m_belong[__a]], btop = m_linkTop[m_belong[__b]];
                if (atop == btop) break;
                if (m_depth[atop] < m_depth[btop]) {
                    res.push_back({m_pos[btop], m_pos[__b]});
                    __b = m_tree.m_parent[btop];
                } else {
                    res.push_back({m_pos[atop], m_pos[__a]});
                    __a = m_tree.m_parent[atop];
                }
            }
            if (m_depth[__a] > m_depth[__b]) std::swap(__a, __b);
            res.push_back({m_pos[__a], m_pos[__b]});
            return res;
        }
        auto getSubtree(uint32_t __a) const {
            struct _range {
                uint32_t start, end;
            };
            return _range{m_pos[__a], m_pos[__a] + m_size[__a] - 1};
        }
        uint32_t lca(uint32_t __a, uint32_t __b) const {
            while (true) {
                uint32_t atop = m_linkTop[m_belong[__a]], btop = m_linkTop[m_belong[__b]];
                if (atop == btop) break;
                if (m_depth[atop] < m_depth[btop])
                    __b = m_tree.m_parent[btop];
                else
                    __a = m_tree.m_parent[atop];
            }
            return m_depth[__a] < m_depth[__b] ? __a : __b;
        }
    };
}

#endif