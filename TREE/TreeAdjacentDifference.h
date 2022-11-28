#ifndef __OY_TREEADJACENTDIFFERENCE__
#define __OY_TREEADJACENTDIFFERENCE__

#include "HeavyLightDecompositionLCA.h"

namespace OY {
    template <typename _Tree, typename _Tp = int64_t, typename _Operation = std::plus<_Tp>, typename _Inverse = std::negate<_Tp>>
    struct TreeAdjacentDifference {
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        _Tree &m_tree;
        _Operation m_op;
        _Inverse m_inv;
        _Tp m_values[_MAXN], m_incrementUpward[_MAXN], m_incrementDownward[_MAXN], m_defaultValue;
        uint32_t m_queue[_MAXN];
        TreeAdjacentDifference(_Tree &__tree, _Operation __op = _Operation(), _Inverse __inv = _Inverse(), _Tp __defaultValue = _Tp()) : m_tree(__tree), m_op(__op), m_inv(__inv), m_defaultValue(__defaultValue) {
            std::fill_n(m_values, m_tree.m_vertexNum, __defaultValue);
            std::fill_n(m_incrementUpward, m_tree.m_vertexNum, __defaultValue);
            std::fill_n(m_incrementDownward, m_tree.m_vertexNum, __defaultValue);
            uint32_t head = 0, tail = 0;
            m_queue[tail++] = m_tree.m_root;
            while (head != tail)
                for (uint32_t i = m_queue[head++], cur = m_tree.m_starts[i] + (i != m_tree.m_root), end = m_tree.m_starts[i + 1]; cur != end; cur++) m_queue[tail++] = m_tree.m_to[cur];
        }
        void addPathValue(uint32_t __a, uint32_t __b, uint32_t __lca, _Tp __inc) {
            m_incrementUpward[__a] = m_op(__inc, m_incrementUpward[__a]);
            m_incrementUpward[__b] = m_op(__inc, m_incrementUpward[__b]);
            m_incrementUpward[__lca] = m_op(m_inv(__inc), m_incrementUpward[__lca]);
            if (uint32_t p = m_tree.getParent(__lca); ~p) m_incrementUpward[p] = m_op(m_inv(__inc), m_incrementUpward[p]);
        }
        void addSubtreeValue(uint32_t __a, _Tp __inc) { m_incrementDownward[__a] = m_op(__inc, m_incrementDownward[__a]); }
        void addVertexValue(uint32_t __a, _Tp __inc) { m_values[__a] = m_op(__inc, m_values[__a]); }
        void partialSum() {
            for (uint32_t idx = 0; idx < m_tree.m_vertexNum; idx++) {
                uint32_t i = m_queue[idx];
                m_values[i] = m_op(m_incrementDownward[i], m_values[i]);
                for (uint32_t cur = m_tree.m_starts[i] + (i != m_tree.m_root), end = m_tree.m_starts[i + 1]; cur != end; cur++) {
                    uint32_t to = m_tree.m_to[cur];
                    m_incrementDownward[to] = m_op(m_incrementDownward[i], m_incrementDownward[to]);
                }
                m_incrementDownward[i] = m_defaultValue;
            }
            for (uint32_t idx = m_tree.m_vertexNum - 1; ~idx; idx--) {
                uint32_t i = m_queue[idx];
                m_values[i] = m_op(m_incrementUpward[i], m_values[i]);
                if (uint32_t p = m_tree.getParent(i); ~p) m_incrementUpward[p] = m_op(m_incrementUpward[i], m_incrementUpward[p]);
                m_incrementUpward[i] = m_defaultValue;
            }
        }
    };
}

#endif