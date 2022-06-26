#ifndef __OY_DINICMCMF__
#define __OY_DINICMCMF__

#include <limits>
#include "Graph.h"

namespace OY {
    template <typename _Tp, typename _Fp>
    struct Dinic_mcmf {
        struct _RawEdge {
            uint32_t from, to;
            _Tp cap;
            _Fp cost;
        };
        struct _Edge {
            uint32_t to, rev;
            _Tp cap;
            _Fp cost;
            bool operator>(const _Edge &other) const { return cap > other.cap; }
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts;
        uint32_t m_vertexNum;
        Dinic_mcmf(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cap, _Fp __cost) { m_rawEdges.push_back({__a, __b, __cap, __cost}); }
        void prepare() {
            for (auto &[from, to, cap, cost] : m_rawEdges)
                if (from != to) {
                    m_starts[from + 1]++;
                    m_starts[to + 1]++;
                }
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (auto &[from, to, cap, cost] : m_rawEdges)
                if (from != to) {
                    m_edges[cursor[from]] = _Edge{to, cursor[to], cap, cost};
                    m_edges[cursor[to]++] = _Edge{from, cursor[from]++, 0, -cost};
                }
        }
        template <typename _Compare = std::greater<_Edge>>
        void prepareSorted(_Compare __comp = _Compare()) {
            prepare();
            for (uint32_t i = 0; i < m_vertexNum; i++) {
                uint32_t start = m_starts[i], end = m_starts[i + 1];
                std::sort(m_edges.begin() + start, m_edges.begin() + end, __comp);
                for (uint32_t j = start; j < end; j++) m_edges[m_edges[j].rev].rev = j;
            }
        }
        std::pair<_Tp, _Fp> calc(uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2, _Fp __infiniteCost = std::numeric_limits<_Fp>::max() / 2) {
            uint32_t queue[m_vertexNum + 1], it[m_vertexNum], end[m_vertexNum], head = 0, tail = 0;
            _Tp totalFlow = 0;
            _Fp costs[m_vertexNum], totalCost = 0;
            std::vector<bool> inQueue(m_vertexNum, false);
            for (uint32_t i = 0; i < m_vertexNum; i++) end[i] = m_starts[i + 1];
            auto dfs = [&](auto self, uint32_t i, _Tp _cap) {
                if (i == __target || !_cap) return _cap;
                inQueue[i] = true;
                _Tp flow = 0, f;
                for (uint32_t &cur = it[i]; cur != end[i]; cur++)
                    if (auto &[to, rev, cap, cost] = m_edges[cur]; !inQueue[to] && costs[i] + cost == costs[to] && (f = self(self, to, std::min(_cap, cap))))
                        if (flow += f, _cap -= f, cap -= f, m_edges[rev].cap += f; !_cap) break;
                inQueue[i] = false;
                return flow;
            };
            while (true) {
                std::fill(costs, costs + m_vertexNum, __infiniteCost);
                costs[__source] = 0;
                queue[tail++] = __source;
                if (tail == m_vertexNum + 1) tail = 0;
                inQueue[__source] = true;
                while (head != tail) {
                    uint32_t from = queue[head++];
                    if (head == m_vertexNum + 1) head = 0;
                    inQueue[from] = false;
                    for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[to, rev, cap, cost] = m_edges[cur]; cap && chmin(costs[to], costs[from] + cost) && !inQueue[to]) {
                            queue[tail++] = to;
                            if (tail == m_vertexNum + 1) tail = 0;
                            inQueue[to] = true;
                        }
                }
                if (costs[__target] == __infiniteCost) break;
                for (uint32_t i = 0; i < m_vertexNum; i++) it[i] = m_starts[i];
                _Tp flow = 0;
                while (_Tp f = dfs(dfs, __source, __infiniteCap)) flow += f;
                totalFlow += flow;
                totalCost += flow * costs[__target];
            }
            return {totalFlow, totalCost};
        }
    };
}

#endif