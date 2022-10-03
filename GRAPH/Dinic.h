#ifndef __OY_DINIC__
#define __OY_DINIC__

#include "Graph.h"
#include <algorithm>
#include <cstdint>
#include <limits>

namespace OY {
    template <typename _Tp>
    struct Dinic {
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
        Dinic(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cap) { m_rawEdges.push_back({__a, __b, __cap}); }
        void prepare() {
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
        void prepareSorted(_Compare __comp = _Compare()) {
            prepare();
            for (uint32_t i = 0; i < m_vertexNum; i++) {
                uint32_t start = m_starts[i], end = m_starts[i + 1];
                std::sort(m_edges.begin() + start, m_edges.begin() + end, __comp);
                for (uint32_t j = start; j < end; j++) m_edges[m_edges[j].rev].rev = j;
            }
        }
        _Tp calc(uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2) {
            uint32_t queue[m_vertexNum], depth[m_vertexNum], it[m_vertexNum], end[m_vertexNum];
            _Tp res = 0;
            for (uint32_t i = 0; i < m_vertexNum; i++) end[i] = m_starts[i + 1];
            auto dfs = [&](auto self, uint32_t i, _Tp _cap) {
                if (i == __target || !_cap) return _cap;
                _Tp flow = 0, f;
                for (uint32_t &cur = it[i]; cur != end[i]; cur++)
                    if (auto &[to, rev, cap] = m_edges[cur]; depth[i] + 1 == depth[to] && (f = self(self, to, std::min(_cap, cap))))
                        if (flow += f, _cap -= f, cap -= f, m_edges[rev].cap += f; !_cap) break;
                return flow;
            };
            while (true) {
                std::fill(depth, depth + m_vertexNum, -1);
                uint32_t head = 0, tail = 0;
                depth[__source] = 0;
                queue[tail++] = __source;
                while (head < tail)
                    for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[to, rev, cap] = m_edges[cur]; cap && chmin(depth[to], depth[from] + 1)) queue[tail++] = to;
                if (!~depth[__target]) break;
                for (uint32_t i = 0; i < m_vertexNum; i++) it[i] = m_starts[i];
                while (_Tp flow = dfs(dfs, __source, __infiniteCap)) res += flow;
            }
            return res;
        }
    };
}

#endif