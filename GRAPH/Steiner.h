#ifndef __OY_STEINER__
#define __OY_STEINER__

#include "Dijkstra_heap.h"
#include "Graph.h"
#include "SPFA.h"

namespace OY {
    template <typename _UDG, typename _Tp = typename _UDG::value_type, template <typename...> typename _Solver = Dijkstra_heap>
    struct Steiner {
        static constexpr uint32_t shift = 10;
        _UDG &m_graph;
        std::vector<uint32_t> &m_keys;
        std::vector<std::vector<_Tp>> m_trees;
        std::vector<std::vector<uint32_t>> m_from;
        _Tp m_minCost;
        Steiner(_UDG &__graph, std::vector<uint32_t> &__keys, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) : m_graph(__graph), m_keys(__keys) {
            m_trees.reserve(1 << m_keys.size());
            m_from.reserve(1 << m_keys.size());
            for (uint32_t mask = 0; mask < 1 << m_keys.size(); mask++) {
                _Solver sol(m_graph, __initDistance);
                if (uint32_t i = std::__countr_zero(mask); std::__popcount(mask) == 1) sol.m_distance[m_keys[i]] = 0;
                if (uint32_t sub = (mask - 1) & mask, half = mask >> 1; sub) do
                        for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                            if (chmin(sol.m_distance[i], m_trees[sub][i] + m_trees[mask - sub][i])) sol.m_from[i] = sub << shift;
                    while ((sub = (sub - 1) & mask) >= half);
                for (uint32_t i = 0; i < m_graph.m_vertexNum; i++) sol.push(i);
                sol.calc();
                m_trees.push_back(std::move(sol.m_distance));
                m_from.push_back(std::move(sol.m_from));
            }
            m_minCost = *std::min_element(m_trees.back().begin(), m_trees.back().end());
        }
        std::vector<uint32_t> getEdges() const {
            std::vector<uint32_t> path;
            struct node {
                uint32_t mask, root;
            };
            std::queue<node> Q;
            Q.push({uint32_t((1 << m_keys.size()) - 1), uint32_t(std::min_element(m_trees.back().begin(), m_trees.back().end()) - m_trees.back().begin())});
            while (Q.size()) {
                auto [mask, root] = Q.front();
                Q.pop();
                if (uint32_t state = m_from[mask][root]; state == -1)
                    continue;
                else if (state < 1 << shift) {
                    path.push_back(state);
                    Q.push({mask, m_graph.getEdge(state).from});
                } else {
                    Q.push({state >> shift, root});
                    Q.push({mask - (state >> shift), root});
                }
            }
            return path;
        }
    };
};

#endif