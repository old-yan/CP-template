#ifndef __OY_TREEADJACENTDIFFERENCE__
#define __OY_TREEADJACENTDIFFERENCE__

#include "HeavyLightDecompositionLCA.h"

namespace OY {
    template <typename _Tree, typename _Tp, template <typename...> typename _Solver = HeavyLightDecompositionLCA>
    struct TreeAdjacentDifference {
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        _Tree &m_tree;
        _Solver<_Tree> m_sol;
        _Tp m_dif[_MAXN];
        uint32_t m_queue[_MAXN];
        TreeAdjacentDifference(_Tree &__tree) : m_tree(__tree), m_sol(__tree) {
            std::fill(m_dif, m_dif + m_tree.m_vertexNum, 0);
            uint32_t deg[m_tree.m_vertexNum], head = 0, tail = 0;
            std::adjacent_difference(m_tree.m_starts + 1, m_tree.m_starts + m_tree.m_vertexNum + 1, deg);
            for (uint32_t i = 0; i < m_tree.m_vertexNum; i++) deg[i]--;
            deg[m_tree.m_root]++;
            for (uint32_t i = 0; i < m_tree.m_vertexNum; i++)
                if (!deg[i]) m_queue[tail++] = i;
            while (tail < m_tree.m_vertexNum - 1)
                if (uint32_t p = m_tree.m_to[m_tree.m_starts[m_queue[head++]]]; !--deg[p]) m_queue[tail++] = p;
        }
        void addPathValue(uint32_t __a, uint32_t __b, _Tp __inc) {
            uint32_t p = m_sol.calc(__a, __b);
            m_dif[__a] += __inc;
            m_dif[__b] += __inc;
            m_dif[p] -= __inc;
            if (p != m_tree.m_root) m_dif[m_tree.m_to[m_tree.m_starts[p]]] -= __inc;
        }
        void partialSum() {
            for (uint32_t i = 0; i < m_tree.m_vertexNum - 1; i++) m_dif[m_tree.m_to[m_tree.m_starts[m_queue[i]]]] += m_dif[m_queue[i]];
        }
        void adjacentDifference() {
            for (uint32_t i = m_tree.m_vertexNum - 2; ~i; i--) m_dif[m_tree.m_to[m_tree.m_starts[m_queue[i]]]] -= m_dif[m_queue[i]];
        }
    };
}

#endif