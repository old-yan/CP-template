#ifndef __OY_TREEADJACENTDIFFERENCE_VECTOR__
#define __OY_TREEADJACENTDIFFERENCE_VECTOR__

#include "HeavyLightDecompositionLCA_vector.h"

namespace OY {
    template <typename _Tree, typename _Tp = int64_t, typename _Operation = std::plus<_Tp>, typename _Inverse = std::negate<_Tp>>
    struct TreeAdjacentDifference_vector {
        _Tree &m_tree;
        _Operation m_op;
        _Inverse m_inv;
        _Tp m_defaultValue;
        std::vector<_Tp> m_values, m_incrementUpward, m_incrementDownward;
        std::vector<uint32_t> m_queue;
        TreeAdjacentDifference_vector(_Tree &__tree, _Operation __op = _Operation(), _Inverse __inv = _Inverse(), _Tp __defaultValue = _Tp()) : m_tree(__tree), m_op(__op), m_inv(__inv), m_defaultValue(__defaultValue), m_values(__tree.m_vertexNum, __defaultValue), m_incrementUpward(__tree.m_vertexNum, __defaultValue), m_incrementDownward(__tree.m_vertexNum, __defaultValue), m_queue(__tree.m_vertexNum) {
            uint32_t head = 0, tail = 0;
            m_queue[tail++] = m_tree.m_root;
            while (head != tail) {
                uint32_t i = m_queue[head++];
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i]) m_queue[tail++] = to;
            }
        }
        void addPathValue(uint32_t __a, uint32_t __b, uint32_t __lca, _Tp __inc) {
            m_incrementUpward[__a] = m_op(__inc, m_incrementUpward[__a]);
            m_incrementUpward[__b] = m_op(__inc, m_incrementUpward[__b]);
            m_incrementUpward[__lca] = m_op(m_inv(__inc), m_incrementUpward[__lca]);
            if (uint32_t p = m_tree.m_parent[__lca]; ~p) m_incrementUpward[p] = m_op(m_inv(__inc), m_incrementUpward[p]);
        }
        void addSubtreeValue(uint32_t __a, _Tp __inc) { m_incrementDownward[__a] = m_op(__inc, m_incrementDownward[__a]); }
        void addVertexValue(uint32_t __a, _Tp __inc) { m_values[__a] = m_op(__inc, m_values[__a]); }
        void partialSum() {
            for (uint32_t idx = 0; idx < m_tree.m_vertexNum; idx++) {
                uint32_t i = m_queue[idx];
                m_values[i] = m_op(m_incrementDownward[i], m_values[i]);
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; to != m_tree.m_parent[i]) m_incrementDownward[to] = m_op(m_incrementDownward[i], m_incrementDownward[to]);
                m_incrementDownward[i] = m_defaultValue;
            }
            for (uint32_t idx = m_tree.m_vertexNum - 1; ~idx; idx--) {
                uint32_t i = m_queue[idx];
                m_values[i] = m_op(m_incrementUpward[i], m_values[i]);
                if (uint32_t p = m_tree.m_parent[i]; ~p) m_incrementUpward[p] = m_op(m_incrementUpward[i], m_incrementUpward[p]);
                m_incrementUpward[i] = m_defaultValue;
            }
        }
    };
}

#endif