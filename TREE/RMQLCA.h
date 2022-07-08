#ifndef __OY_RMQLCA__
#define __OY_RMQLCA__

#include "../DS/STTable.h"
#include "Tree.h"

namespace OY {
    template <typename _Tree, template <typename...> typename _Solver = STTable>
    struct RMQLCA {
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        _Tree &m_tree;
        uint32_t m_sequence[_MAXN * 2 - 1];
        uint32_t m_minIndexSequence[_MAXN * 2 - 1];
        uint32_t m_minIndex[_MAXN];
        uint32_t m_maxIndex[_MAXN];
        _Solver<uint32_t, const uint32_t &(*)(const uint32_t &, const uint32_t &)> m_sol;
        RMQLCA(_Tree &__tree) : m_tree(__tree), m_sol(0, std::min<uint32_t>) {
            uint32_t cursor = 0;
            auto dfs = [&](auto self, uint32_t i) -> void {
                m_sequence[cursor++] = i;
                for (uint32_t cur = m_tree.m_starts[i] + (i != m_tree.m_root), end = m_tree.m_starts[i + 1]; cur != end; cur++) {
                    self(self, m_tree.m_to[cur]);
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
            m_sol.reset(m_minIndexSequence, m_minIndexSequence + cursor);
        }
        uint32_t calc(uint32_t __a, uint32_t __b) { return m_sequence[m_sol.query(std::min(m_minIndex[__a], m_minIndex[__b]), std::max(m_maxIndex[__a], m_maxIndex[__b]))]; }
    };
}

#endif