#ifndef __OY_LIMITEDRMQLCA__
#define __OY_LIMITEDRMQLCA__

#include "../DS/LimitedRMQ.h"
#include "Tree.h"

namespace OY {
    template <typename _Tree>
    struct LimitedRMQLCA {
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        _Tree &m_tree;
        uint32_t m_sequence[_MAXN * 2 - 1];
        uint32_t m_depthSequence[_MAXN * 2 - 1];
        uint32_t m_minIndex[_MAXN];
        uint32_t m_maxIndex[_MAXN];
        LimitedRMQ<_LimitedRMQPicker<uint32_t[_MAXN * 2 - 1], std::greater<uint32_t>>> m_rmq;
        LimitedRMQLCA(_Tree &__tree) : m_tree(__tree), m_rmq(m_depthSequence) {
            uint32_t cursor = 0;
            auto dfs = [&](auto self, uint32_t i, uint32_t d) -> void {
                m_sequence[cursor] = i;
                m_depthSequence[cursor++] = d;
                for (uint32_t cur = m_tree.m_starts[i] + (i != m_tree.m_root), end = m_tree.m_starts[i + 1]; cur != end; cur++) {
                    self(self, m_tree.m_to[cur], d + 1);
                    m_sequence[cursor] = i;
                    m_depthSequence[cursor++] = d;
                }
            };
            dfs(dfs, m_tree.m_root, 0);
            for (uint32_t i = 0; i < m_tree.m_vertexNum; i++) {
                m_minIndex[i] = cursor;
                m_maxIndex[i] = 0;
            }
            for (uint32_t i = 0; i < cursor; i++) {
                chmin(m_minIndex[m_sequence[i]], i);
                chmax(m_maxIndex[m_sequence[i]], i);
            }
            m_rmq.reset(m_depthSequence, m_depthSequence + cursor);
        }
        uint32_t calc(uint32_t __a, uint32_t __b) { return m_sequence[m_rmq.query(std::min(m_minIndex[__a], m_minIndex[__b]), std::max(m_maxIndex[__a], m_maxIndex[__b]))]; }
    };
}

#endif