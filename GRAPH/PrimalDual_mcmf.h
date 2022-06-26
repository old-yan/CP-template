#ifndef __OY_PRIMALDUALMCMF__
#define __OY_PRIMALDUALMCMF__

#include <limits>
#include "../DS/SiftHeap.h"
#include "Graph.h"

namespace OY {
    template <typename _Tp, typename _Fp>
    struct PrimalDual_mcmf {
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
        PrimalDual_mcmf(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
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
            uint32_t spfaQueue[m_vertexNum + 1], fromEdge[m_vertexNum], prev[m_vertexNum], head = 0, tail = 0;
            std::vector<bool> spfaInQueue(m_vertexNum, false);
            _Tp flow[m_vertexNum], totalFlow = 0;
            _Fp costs[m_vertexNum], spfaCosts[m_vertexNum], f, totalCost = 0;
            std::fill(spfaCosts, spfaCosts + m_vertexNum, __infiniteCost);
            spfaCosts[__source] = 0;
            spfaQueue[tail++] = __source;
            spfaInQueue[__source] = true;
            while (head != tail) {
                uint32_t from = spfaQueue[head++];
                if (head == m_vertexNum + 1) head = 0;
                spfaInQueue[from] = false;
                for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                    if (auto &[to, rev, cap, cost] = m_edges[cur]; cap && chmin(spfaCosts[to], spfaCosts[from] + cost) && !spfaInQueue[to]) {
                        spfaQueue[tail++] = to;
                        if (tail == m_vertexNum + 1) tail = 0;
                        spfaInQueue[to] = true;
                    }
            }
            auto mapping = [&](uint32_t i) { return costs[i]; };
            SiftHeap heap(m_vertexNum, mapping, std::greater<_Fp>{});
            while (true) {
                std::fill(costs, costs + m_vertexNum, __infiniteCost);
                costs[__source] = 0;
                flow[__source] = __infiniteCap;
                heap.push(__source);
                while (heap.size()) {
                    uint32_t from = heap.top();
                    heap.pop();
                    for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[to, rev, cap, cost] = m_edges[cur]; cap && chmin(costs[to], costs[from] + cost + spfaCosts[from] - spfaCosts[to])) {
                            flow[to] = std::min(flow[from], cap);
                            fromEdge[to] = cur;
                            prev[to] = from;
                            heap.push(to);
                        }
                }
                if (costs[__target] == __infiniteCost) break;
                for (uint32_t i = 0; i < m_vertexNum; i++) spfaCosts[i] += costs[i];
                totalFlow += f = flow[__target];
                totalCost += f * spfaCosts[__target];
                for (uint32_t cur = __target; cur != __source; cur = prev[cur]) {
                    auto &[to, rev, cap, cost] = m_edges[fromEdge[cur]];
                    cap -= f;
                    m_edges[rev].cap += f;
                }
            }
            return {totalFlow, totalCost};
        }
    };
}

#endif