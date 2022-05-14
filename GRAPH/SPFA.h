#ifndef __OY_SPFA__
#define __OY_SPFA__

#include <queue>
#include "Graph.h"

namespace OY {
    template <typename _Graph, typename _Tp = typename _Graph::value_type>
    struct SPFA {
        _Graph &m_graph;
        std::vector<_Tp> m_distance;
        std::vector<uint32_t> m_from;
        std::queue<uint32_t> m_queue;
        std::vector<bool> m_inQueue;
        SPFA(_Graph &__graph, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) : m_graph(__graph), m_distance(__graph.m_vertexNum, __initDistance), m_from(__graph.m_vertexNum, -1), m_inQueue(__graph.m_vertexNum, false) {}
        void push(uint32_t __i) {
            m_queue.push(__i);
            m_inQueue[__i] = true;
        }
        uint32_t pop() {
            uint32_t i = m_queue.front();
            m_queue.pop();
            m_inQueue[i] = false;
            return i;
        }
        void init(uint32_t __i, _Tp __distance = 0) {
            m_distance[__i] = __distance;
            push(__i);
        }
        void calc() {
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                for (uint32_t len = m_queue.size(); len--;)
                    for (auto [index, from, to, value] : m_graph.getEdgesInfoOf(pop()))
                        if (chmin(m_distance[to], m_distance[from] + value)) {
                            m_from[to] = index;
                            if (!m_inQueue[to]) push(to);
                        }
        }
        bool hasNegativeCycle() const { return m_queue.size(); }
        std::vector<uint32_t> getPath_edge(uint32_t __target) const {
            std::vector<uint32_t> path;
            for (uint32_t cur = __target; ~m_from[cur]; cur = m_graph.getEdge(m_from[cur]).from) path.push_back(m_from[cur]);
            std::reverse(path.begin(), path.end());
            return path;
        }
        std::vector<uint32_t> getPath_vertex(uint32_t __target) const {
            std::vector<uint32_t> path;
            path.push_back(__target);
            for (uint32_t cur = __target; ~m_from[cur];) path.push_back(cur = m_graph.getEdge(m_from[cur]).from);
            std::reverse(path.begin(), path.end());
            return path;
        }
    };
}

#endif