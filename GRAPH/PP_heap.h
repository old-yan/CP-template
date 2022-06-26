#ifndef __OY_PP_HEAP__
#define __OY_PP_HEAP__

#include <limits>
#include "../DS/SiftHeap.h"
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct PP_heap {
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
        PP_heap(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
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
                    m_edges[cursor[from]] = {to, cursor[to], cap};
                    m_edges[cursor[to]++] = {from, cursor[from]++, 0};
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
            uint32_t depth[m_vertexNum];
            _Tp ex[m_vertexNum];
            auto self = [](uint32_t x) { return x; };
            auto comp = [&](uint32_t x, uint32_t y) { return ex[y] > 0 && (ex[x] <= 0 || depth[x] < depth[y]); };
            SiftHeap heap(m_vertexNum, self, comp);
            auto ex_add = [&](uint32_t i, _Tp f) {
                ex[i] += f;
                if (ex[i] == f) heap.siftUp(i);
            };
            auto ex_remove = [&](uint32_t i, _Tp f) {
                ex[i] -= f;
                if (!ex[i]) heap.siftDown(i);
            };
            auto relabel = [&](uint32_t i) {
                uint32_t old = depth[i];
                depth[i] = m_vertexNum;
                for (uint32_t cur = m_starts[i], end = m_starts[i + 1]; cur < end; cur++)
                    if (auto &[to, rev, cap] = m_edges[cur]; cap) chmin(depth[i], depth[to] + 1);
                if (depth[i] == m_vertexNum) {
                    ex[i] = 0;
                    heap.siftDown(i);
                } else if (depth[i] < old)
                    heap.siftDown(i);
                else
                    heap.siftUp(i);
            };
            std::fill(depth, depth + m_vertexNum, 0);
            depth[__source] = m_vertexNum - 1;
            std::fill(ex, ex + m_vertexNum, 0);
            ex[__source] = __infiniteCap;
            ex[__target] = -__infiniteCap;
            for (uint32_t i = 0; i < m_vertexNum; i++) heap.m_heap.push_back(heap.m_pos[i] = i);
            heap.siftUp(__source);
            heap.siftDown(__target);
            while (ex[heap.top()] > 0) {
                uint32_t i = heap.top();
                _Tp &rest = ex[i], f;
                for (uint32_t cur = m_starts[i], end = m_starts[i + 1]; cur < end; cur++)
                    if (auto &[to, rev, cap] = m_edges[cur]; depth[i] == depth[to] + 1 && (f = std::min(rest, cap))) {
                        ex_remove(i, f);
                        ex_add(to, f);
                        cap -= f;
                        m_edges[rev].cap += f;
                    }
                if (rest > 0) relabel(i);
            }
            return ex[__target] + __infiniteCap;
        }
    };
}

#endif