#ifndef __OY_LIMITEDRMQLCA_VECTOR__
#define __OY_LIMITEDRMQLCA_VECTOR__

#include "../DS/LimitedRMQ.h"
#include "VectorTree.h"

namespace OY {
    template <typename _Tree>
    struct LimitedRMQLCA_vector {
        _Tree &m_tree;
        std::vector<uint32_t> m_sequence, m_depthSequence, m_minIndex, m_maxIndex;
        LimitedRMQ<_LimitedRMQPicker<std::vector<uint32_t>, std::greater<uint32_t>>> m_rmq;
        LimitedRMQLCA_vector(_Tree &__tree) : m_tree(__tree), m_sequence(__tree.m_vertexNum * 2 - 1), m_depthSequence(__tree.m_vertexNum * 2 - 1), m_minIndex(__tree.m_vertexNum), m_maxIndex(__tree.m_vertexNum), m_rmq(m_depthSequence) {
            uint32_t cursor = 0;
            auto dfs = [&](auto self, uint32_t i, uint32_t d) -> void {
                m_sequence[cursor] = i;
                m_depthSequence[cursor++] = d;
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i]) {
                        self(self, to, d + 1);
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
            m_rmq.reset(m_depthSequence.begin(), m_depthSequence.end());
        }
        uint32_t calc(uint32_t __a, uint32_t __b) { return m_sequence[m_rmq.query(std::min(m_minIndex[__a], m_minIndex[__b]), std::max(m_maxIndex[__a], m_maxIndex[__b]))]; }
    };
}

#endif