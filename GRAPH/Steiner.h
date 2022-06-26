#ifndef __OY_STEINER__
#define __OY_STEINER__

#include <limits>
#include <queue>
#include "../DS/SiftHeap.h"
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct Steiner {
        struct _RawEdge {
            uint32_t from, to;
            _Tp cost;
        };
        struct _Edge {
            uint32_t index, to;
            _Tp cost;
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts;
        std::vector<std::vector<_Tp>> m_trees;
        std::vector<std::vector<uint32_t>> m_from;
        uint32_t m_vertexNum;
        _Tp m_infiniteCost;
        _Tp m_minCost;
        Steiner(uint32_t __vertexNum, uint32_t __edgeNum, _Tp __infiniteCost = std::numeric_limits<_Tp>::max() / 2) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum), m_infiniteCost(__infiniteCost) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { m_rawEdges.push_back({__a, __b, __cost}); }
        void prepare() {
            for (auto &[from, to, cost] : m_rawEdges)
                if (from != to) {
                    m_starts[from + 1]++;
                    m_starts[to + 1]++;
                }
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++)
                if (auto &[from, to, cost] = m_rawEdges[index]; from != to) {
                    m_edges[cursor[from]++] = _Edge{index, to, cost};
                    m_edges[cursor[to]++] = _Edge{index, from, cost};
                }
        }
        template <bool _GetPath = false>
        bool calc_dijk(const std::vector<uint32_t> &__keys) {
            m_trees.reserve(1 << __keys.size());
            std::vector<_Tp> costs(m_vertexNum);
            std::vector<uint32_t> froms;
            if constexpr (_GetPath) {
                m_from.reserve(1 << __keys.size());
                froms.resize(m_vertexNum);
            }
            SiftHeap dijkstraHeap(m_vertexNum, costs, std::greater<_Tp>{});
            for (uint32_t mask = 0; mask < 1 << __keys.size(); mask++) {
                std::fill(costs.begin(), costs.end(), m_infiniteCost);
                if constexpr (_GetPath) std::fill(froms.begin(), froms.end(), -1);
                if (uint32_t i = std::__countr_zero(mask); std::__popcount(mask) == 1) costs[__keys[i]] = 0;
                if (uint32_t sub = (mask - 1) & mask, half = mask >> 1; sub) do
                        for (uint32_t i = 0; i < m_vertexNum; i++)
                            if (chmin(costs[i], m_trees[sub][i] + m_trees[mask - sub][i])) {
                                if constexpr (_GetPath) froms[i] = 0x80000000 | sub;
                            }
                    while ((sub = (sub - 1) & mask) >= half);
                for (uint32_t i = 0; i < m_vertexNum; i++)
                    if (costs[i] < m_infiniteCost) dijkstraHeap.push(i);
                while (dijkstraHeap.size()) {
                    uint32_t from = dijkstraHeap.top();
                    _Tp curCost = costs[from];
                    dijkstraHeap.pop();
                    for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[index, to, cost] = m_edges[cur]; chmin(costs[to], curCost + cost)) {
                            dijkstraHeap.push(to);
                            if constexpr (_GetPath) froms[to] = index;
                        }
                }
                m_trees.push_back(costs);
                if constexpr (_GetPath) m_from.push_back(froms);
            }
            m_minCost = *std::min_element(m_trees.back().begin(), m_trees.back().end());
            return m_minCost < m_infiniteCost;
        }
        template <bool _GetPath = false>
        bool calc_spfa(const std::vector<uint32_t> &__keys) {
            m_trees.reserve(1 << __keys.size());
            std::vector<_Tp> costs(m_vertexNum);
            std::vector<uint32_t> froms;
            if constexpr (_GetPath) {
                m_from.reserve(1 << __keys.size());
                froms.resize(m_vertexNum);
            }
            std::queue<uint32_t> spfaQueue;
            std::vector<bool> spfaInQueue(m_vertexNum, false);
            for (uint32_t mask = 0; mask < 1 << __keys.size(); mask++) {
                std::fill(costs.begin(), costs.end(), m_infiniteCost);
                if constexpr (_GetPath) std::fill(froms.begin(), froms.end(), -1);
                if (uint32_t i = std::__countr_zero(mask); std::__popcount(mask) == 1) costs[__keys[i]] = 0;
                if (uint32_t sub = (mask - 1) & mask, half = mask >> 1; sub) do
                        for (uint32_t i = 0; i < m_vertexNum; i++)
                            if (chmin(costs[i], m_trees[sub][i] + m_trees[mask - sub][i])) {
                                if constexpr (_GetPath) froms[i] = 0x80000000 | sub;
                            }
                    while ((sub = (sub - 1) & mask) >= half);
                for (uint32_t i = 0; i < m_vertexNum; i++)
                    if (costs[i] < m_infiniteCost) {
                        spfaQueue.push(i);
                        spfaInQueue[i] = true;
                    }
                while (spfaQueue.size()) {
                    uint32_t from = spfaQueue.front();
                    spfaQueue.pop();
                    spfaInQueue[from] = false;
                    for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[index, to, cost] = m_edges[cur]; chmin(costs[to], costs[from] + cost)) {
                            if constexpr (_GetPath) froms[to] = index;
                            if (!spfaInQueue[to]) {
                                spfaQueue.push(to);
                                spfaInQueue[to] = true;
                            }
                        }
                }
                m_trees.push_back(costs);
                if constexpr (_GetPath) m_from.push_back(froms);
            }
            m_minCost = *std::min_element(m_trees.back().begin(), m_trees.back().end());
            return m_minCost < m_infiniteCost;
        }
        std::vector<uint32_t> getEdges() const {
            std::vector<uint32_t> path;
            struct node {
                uint32_t mask, root;
            };
            std::queue<node> Q;
            Q.push({uint32_t(m_trees.size() - 1), uint32_t(std::min_element(m_trees.back().begin(), m_trees.back().end()) - m_trees.back().begin())});
            while (Q.size()) {
                auto [mask, root] = Q.front();
                Q.pop();
                if (uint32_t state = m_from[mask][root]; state == -1)
                    continue;
                else if (!(state & 0x80000000)) {
                    path.push_back(state);
                    Q.push({mask, m_rawEdges[state].from ^ m_rawEdges[state].to ^ root});
                } else {
                    Q.push({state ^ 0x80000000, root});
                    Q.push({mask - (state ^ 0x80000000), root});
                }
            }
            return path;
        }
    };
};

#endif