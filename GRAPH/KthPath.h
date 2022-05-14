#ifndef __OY_KTHPATH__
#define __OY_KTHPATH__

#include "Dijkstra_heap.h"

namespace OY {
    template <typename _DG, typename _Tp = typename _DG::value_type, template <typename...> typename _Solver = Dijkstra_heap>
    struct KthPath {
        struct node {
            _Tp distance;
            uint32_t i;
            bool operator<(const node &other) const { return distance > other.distance; }
        };
        _DG m_graph;
        _Tp m_initDistance;
        uint32_t m_target;
        std::vector<_Tp> m_targetDistance;
        std::priority_queue<node> m_queue;
        std::vector<uint32_t> m_visit;
        KthPath(_DG &__graph, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) : m_graph(__graph), m_initDistance(__initDistance), m_visit(m_graph.m_vertexNum, 0) {}
        void setTarget(uint32_t __target) {
            m_target = __target;
            _DG reverseGraph(m_graph.m_vertexNum, m_graph.m_edgeNum);
            for (uint32_t i = 0; i < m_graph.m_edgeNum; i++) {
                auto &edge = m_graph.getEdge(i);
                auto [to, value] = edge.data;
                reverseGraph.addEdge(to, edge.from, value);
            }
            reverseGraph.prepare();
            OY::Dijkstra_heap dijk(reverseGraph, m_initDistance);
            dijk.init(__target);
            dijk.calc();
            m_targetDistance = std::move(dijk.m_distance);
        }
        void init(uint32_t __i, _Tp __distance = 0) { m_queue.push({__distance + m_targetDistance[__i], __i}); }
        template <typename _Operation>
        void calc(_Operation __op) {
            bool stop = false;
            while (m_queue.size()) {
                auto [curDistance, cur] = m_queue.top();
                m_queue.pop();
                __op(curDistance, cur, m_visit[cur]++, stop);
                if (stop) break;
                if (cur == m_target) continue;
                curDistance -= m_targetDistance[cur];
                auto edges = m_graph.getEdgesOf(cur);
                for (auto &[to, value] : edges) {
                    m_queue.push({curDistance + value + m_targetDistance[to], to});
                }
            }
        }
    };
}

#endif