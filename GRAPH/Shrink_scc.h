#ifndef __SHRINK_SCC__
#define __SHRINK_SCC__

#include "Tarjan_scc.h"

namespace OY {
    template <typename _DG, template <typename...> typename _Solver = Tarjan_scc>
    struct Shrink_scc {
        _Solver<_DG> m_sol;
        _DG m_newGraph;
        Shrink_scc(_DG &__graph) : m_sol(__graph), m_newGraph(__graph.m_vertexNum, __graph.m_edgeNum) {
            for (auto [index, from, to, value] : __graph.getEdgesInfo()) m_newGraph.addEdge(m_sol.m_id[from], m_sol.m_id[to], value);
            m_newGraph.prepare();
        }
    };
}

#endif