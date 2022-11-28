#ifndef __OY_RMQLCA_VECTOR__
#define __OY_RMQLCA_VECTOR__

#include "../DS/STTable.h"
#include "VectorTree.h"

namespace OY {
    template <typename _Tree, template <typename...> typename _Solver = STTable>
    struct RMQLCA_vector {
        _Tree &m_tree;
        std::vector<uint32_t> m_sequence, m_minIndexSequence, m_minIndex, m_maxIndex;
        _Solver<uint32_t, const uint32_t &(*)(const uint32_t &, const uint32_t &)> m_sol;
        RMQLCA_vector(_Tree &__tree) : m_tree(__tree), m_sequence(__tree.m_vertexNum * 2 - 1), m_minIndexSequence(__tree.m_vertexNum * 2 - 1), m_minIndex(__tree.m_vertexNum), m_maxIndex(__tree.m_vertexNum), m_sol(0, std::min<uint32_t>) {
            uint32_t cursor = 0;
            auto dfs = [&](auto self, uint32_t i) -> void {
                m_sequence[cursor++] = i;
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i]) {
                        self(self, to);
                        m_sequence[cursor++] = i;
                    }
            };
            dfs(dfs, m_tree.m_root);
            for (uint32_t i = 0; i < m_tree.m_vertexNum; i++) {
                m_minIndex[i] = cursor;
                m_maxIndex[i] = 0;
            }
            for (uint32_t i = 0; i < cursor; i++) {
                chmin(m_minIndex[m_sequence[i]], i);
                chmax(m_maxIndex[m_sequence[i]], i);
            }
            for (uint32_t i = 0; i < cursor; i++) m_minIndexSequence[i] = m_minIndex[m_sequence[i]];
            m_sol.reset(m_minIndexSequence.begin(), m_minIndexSequence.end());
        }
        uint32_t calc(uint32_t __a, uint32_t __b) { return m_sequence[m_sol.query(std::min(m_minIndex[__a], m_minIndex[__b]), std::max(m_maxIndex[__a], m_maxIndex[__b]))]; }
    };
}

#endif