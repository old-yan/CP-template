#ifndef __OY_ISAP__
#define __OY_ISAP__

#include <algorithm>
#include <cstdint>
#include <limits>
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct ISAP {
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
        ISAP(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
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
            uint32_t queue[m_vertexNum], depth[m_vertexNum], head = 0, tail = 0, num[m_vertexNum + 1], point[m_vertexNum], prev[m_vertexNum], len = 0, it[m_vertexNum], end[m_vertexNum];
            _Tp res = 0;
            std::fill(depth, depth + m_vertexNum, m_vertexNum);
            depth[__target] = 0;
            queue[tail++] = __target;
            while (head < tail)
                for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                    if (auto &[to, rev, cap] = m_edges[cur]; m_edges[rev].cap && chmin(depth[to], depth[from] + 1)) queue[tail++] = to;
            if (depth[__source] == m_vertexNum) return res;
            std::fill(num, num + m_vertexNum + 1, 0);
            for (uint32_t i = 0; i < m_vertexNum; i++) num[depth[i]]++;
            for (uint32_t i = 0; i < m_vertexNum; i++) it[i] = m_starts[i];
            for (uint32_t i = 0; i < m_vertexNum; i++) end[i] = m_starts[i + 1];
            point[len++] = __source;
            while (depth[__source] < m_vertexNum)
                if (uint32_t x = point[len - 1]; x == __target) {
                    _Tp flow = __infiniteCap;
                    for (uint32_t i = 1; i < len; i++) chmin(flow, m_edges[prev[i]].cap);
                    for (uint32_t i = 1; i < len; i++) {
                        auto &[to, rev, cap] = m_edges[prev[i]];
                        cap -= flow;
                        m_edges[rev].cap += flow;
                    }
                    res += flow;
                    len = 1;
                } else {
                    for (uint32_t &cur = it[x]; cur != end[x]; cur++)
                        if (auto &[to, rev, cap] = m_edges[cur]; cap && depth[x] == depth[to] + 1) {
                            point[len] = to;
                            prev[len++] = cur;
                            break;
                        }
                    if (x == point[len - 1]) {
                        if (!--num[depth[x]]) break;
                        depth[x] = m_vertexNum;
                        for (uint32_t cur = m_starts[x], end = m_starts[x + 1]; cur < end; cur++)
                            if (auto &[to, rev, cap] = m_edges[cur]; cap) chmin(depth[x], depth[to] + 1);
                        num[depth[x]]++;
                        it[x] = m_starts[x];
                        if (len > 1) len--;
                    }
                }
            return res;
        }
    };
}

#endif