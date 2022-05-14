#ifndef __OY_TWOSAT__
#define __OY_TWOSAT__

#include "Tarjan_scc.h"

namespace OY {
    template <template <typename...> typename _DG = DirectedGraph, template <typename...> typename _Solver = Tarjan_scc>
    struct TwoSat {
        _DG<uint32_t> m_graph;
        std::vector<bool> m_choose;
        TwoSat(uint32_t __n, uint32_t __m) : m_graph(__n * 2, __m * 2), m_choose(__n) {}
        void addClause(uint32_t __i, bool __iVal, uint32_t __j, bool __jVal) {
            m_graph.addEdge(__i * 2 + !__iVal, __j * 2 + __jVal, 0);
            m_graph.addEdge(__j * 2 + !__jVal, __i * 2 + __iVal, 0);
        }
        bool calc() {
            m_graph.prepare();
            _Solver<_DG<uint32_t>> sol(m_graph);
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i += 2) {
                if (sol.m_id[i] == sol.m_id[i + 1]) return false;
                m_choose[i >> 1] = sol.m_id[i] < sol.m_id[i + 1];
            }
            return true;
        }
    };
};

#endif