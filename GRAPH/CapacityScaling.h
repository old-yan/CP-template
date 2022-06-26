#ifndef __OY_CAPACITYSCALING__
#define __OY_CAPACITYSCALING__

#include <limits>
#include "Graph.h"

namespace OY {
    template <typename _Tp, typename _Fp>
    struct CapacityScaling_mcmf {
        struct _RawEdge {
            uint32_t from, to;
            _Tp cap;
            _Fp cost;
        };
        struct _Edge {
            uint32_t to, rev;
            _Tp cap, flow;
            _Fp cost;
            bool operator>(const _Edge &other) const { return cap > other.cap; }
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts;
        uint32_t m_vertexNum;
        uint32_t m_source;
        uint32_t m_target;
        _Tp m_infiniteCap;
        _Fp m_infiniteCost;
        CapacityScaling_mcmf(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum + 1); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cap, _Fp __cost) { m_rawEdges.push_back({__a, __b, __cap, __cost}); }
        void setting(uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2, _Fp __infiniteCost = std::numeric_limits<_Fp>::max() / 2) {
            m_source = __source;
            m_target = __target;
            m_infiniteCap = __infiniteCap;
            m_infiniteCost = __infiniteCost;
        }
        void prepare() {
            m_rawEdges.push_back({m_target, m_source, m_infiniteCap, -m_infiniteCost});
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
                    m_edges[cursor[from]] = _Edge{to, cursor[to], cap, 0, cost};
                    m_edges[cursor[to]++] = _Edge{from, cursor[from]++, 0, 0, -cost};
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
        std::pair<_Tp, _Fp> calc() {
            uint32_t queue[m_vertexNum + 1], fromEdge[m_vertexNum], prev[m_vertexNum], head = 0, tail = 0;
            std::vector<bool> inQueue(m_vertexNum, false);
            _Tp totalFlow = 0;
            _Fp costs[m_vertexNum], totalCost = 0;
            auto spfa = [&](uint32_t i) {
                std::fill(costs, costs + m_vertexNum, m_infiniteCost);
                costs[i] = 0;
                queue[tail++] = i;
                if (tail == m_vertexNum + 1) tail = 0;
                inQueue[i] = true;
                while (head != tail) {
                    uint32_t from = queue[head++];
                    if (head == m_vertexNum + 1) head = 0;
                    inQueue[from] = false;
                    for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[to, rev, cap, flow, cost] = m_edges[cur]; flow && chmin(costs[to], costs[from] + cost)) {
                            fromEdge[to] = cur;
                            prev[to] = from;
                            if (!inQueue[to]) {
                                queue[tail++] = to;
                                if (tail == m_vertexNum + 1) tail = 0;
                                inQueue[to] = true;
                            }
                        }
                }
            };
            for (uint32_t i = std::__countr_zero(std::__bit_floor(m_infiniteCap)); ~i; i--) {
                for (auto &[to, rev, cap, flow, cost] : m_edges) flow <<= 1;
                for (uint32_t from = 0; from < m_vertexNum; from++)
                    for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[to, rev, cap, flow, cost] = m_edges[cur]; cap >> i & 1) {
                            if (flow) {
                                flow++;
                                continue;
                            }
                            spfa(to);
                            if (costs[from] < m_infiniteCost && costs[from] + cost < 0) {
                                m_edges[rev].flow++;
                                for (uint32_t cur = from; cur != to; cur = prev[cur]) {
                                    auto &[to, rev, cap, flow, cost] = m_edges[fromEdge[cur]];
                                    flow--;
                                    m_edges[rev].flow++;
                                }
                            } else
                                flow++;
                        }
            }
            for (uint32_t from = 0; from < m_vertexNum; from++)
                for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                    if (auto &[to, rev, cap, flow, cost] = m_edges[cur]; cap)
                        if (cap == m_infiniteCap)
                            totalFlow = m_edges[rev].flow;
                        else
                            totalCost += m_edges[rev].flow * cost;
            return {totalFlow, totalCost};
        }
    };
}

#endif