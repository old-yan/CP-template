#ifndef __OY_EDMONDSKARP__
#define __OY_EDMONDSKARP__

#include <algorithm>
#include <cstdint>
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct EdmondsKarp {
        struct _RawEdge {
            uint32_t from, to;
            _Tp cap;
        };
        struct _Edge {
            uint32_t to, rev;
            _Tp cap;
            bool operator>(const _Edge &other) const { return cap > other.cap; }
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts;
        uint32_t m_vertexNum;
        EdmondsKarp(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cap) { m_rawEdges.push_back({__a, __b, __cap}); }
        void build() {
            for (auto &[from, to, cap] : m_rawEdges)
                if (from != to) {
                    m_starts[from + 1]++;
                    m_starts[to + 1]++;
                }
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (auto &[from, to, cap] : m_rawEdges)
                if (from != to) {
                    m_edges[cursor[from]] = _Edge{to, cursor[to], cap};
                    m_edges[cursor[to]++] = _Edge{from, cursor[from]++, 0};
                }
        }
        template <typename _Compare = std::greater<_Edge>>
        void buildSorted(_Compare __comp = _Compare()) {
            build();
            for (uint32_t i = 0; i < m_vertexNum; i++) {
                uint32_t start = m_starts[i], end = m_starts[i + 1];
                std::sort(m_edges.begin() + start, m_edges.begin() + end, __comp);
                for (uint32_t j = start; j < end; j++) m_edges[m_edges[j].rev].rev = j;
            }
        }
        _Tp calc(uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2) {
            uint32_t queue[m_vertexNum], fromEdge[m_vertexNum], prev[m_vertexNum];
            _Tp res = 0, flow[m_vertexNum], f;
            while (true) {
                std::fill(flow, flow + m_vertexNum, 0);
                uint32_t head = 0, tail = 0;
                flow[__source] = __infiniteCap;
                queue[tail++] = __source;
                while (head < tail)
                    for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[to, rev, cap] = m_edges[cur]; cap && !flow[to]) {
                            flow[to] = std::min(flow[from], cap);
                            fromEdge[to] = cur;
                            prev[to] = from;
                            queue[tail++] = to;
                        }
                if (!flow[__target]) break;
                res += f = flow[__target];
                for (uint32_t cur = __target; cur != __source; cur = prev[cur]) {
                    auto &[to, rev, cap] = m_edges[fromEdge[cur]];
                    cap -= f;
                    m_edges[rev].cap += f;
                }
            }
            return res;
        }
    };
}

#endif