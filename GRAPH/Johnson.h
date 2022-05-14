#ifndef __OY_JOHNSON__
#define __OY_JOHNSON__

#include "Dijkstra_heap.h"
#include "SPFA.h"

namespace OY {
    template <typename _Graph, typename _Tp = typename _Graph::value_type, template <typename...> typename _Solver = Dijkstra_heap>
    struct Johnson {
        std::vector<std::vector<_Tp>> m_distance;
        Johnson(_Graph &__graph, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) {
            SPFA spfa(__graph, __initDistance);
            for (uint32_t i = 0; i < __graph.m_vertexNum; i++) spfa.init(i);
            spfa.calc();
            if (spfa.hasNegativeCycle()) return;
            for (auto &[from, to, value] : __graph.getEdgesData()) value += spfa.m_distance[from] - spfa.m_distance[to];
            m_distance.reserve(__graph.m_vertexNum);
            for (uint32_t i = 0; i < __graph.m_vertexNum; i++) {
                _Solver sol(__graph, __initDistance);
                sol.init(i);
                sol.calc();
                m_distance.push_back(std::move(sol.m_distance));
                for (uint32_t j = 0; j < __graph.m_vertexNum; j++)
                    if (m_distance[i][j] < __initDistance) m_distance[i][j] -= spfa.m_distance[i] - spfa.m_distance[j];
            }
        }
        bool hasNegativeCycle() const { return m_distance.empty(); }
    };
}

#endif