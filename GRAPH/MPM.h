#ifndef __OY_MPM__
#define __OY_MPM__

#include <algorithm>
#include <cstdint>
#include "../DS/SiftHeap.h"
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct MPM {
        struct _RawEdge {
            uint32_t from, to;
            _Tp cap;
        };
        struct _Edge {
            uint32_t from, to, rev;
            _Tp cap;
            bool operator>(const _Edge &other) const { return cap > other.cap; }
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts;
        uint32_t m_vertexNum;
        MPM(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
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
                    m_edges[cursor[from]] = _Edge{from, to, cursor[to], cap};
                    m_edges[cursor[to]++] = _Edge{to, from, cursor[from]++, 0};
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
            _Tp res = 0, in_flow[m_vertexNum], out_flow[m_vertexNum], flow[m_vertexNum], ex[m_vertexNum];
            uint32_t queue[m_vertexNum], depth[m_vertexNum], head, tail, in_prev[m_edges.size() + m_vertexNum], in_next[m_edges.size() + m_vertexNum], out_prev[m_edges.size() + m_vertexNum], out_next[m_edges.size() + m_vertexNum];
            auto mapping = [&](uint32_t i) { return flow[i]; };
            SiftHeap heap(m_vertexNum, mapping, std::greater<_Tp>{});
            for (uint32_t i = 0; i < m_vertexNum; i++) heap.m_heap.push_back(heap.m_pos[i] = i);
            auto insert = [&](uint32_t e, uint32_t i, uint32_t prev[], uint32_t next[]) {
                prev[e] = m_edges.size() + i;
                next[e] = next[m_edges.size() + i];
                prev[next[e]] = next[prev[e]] = e;
            };
            auto erase = [&](uint32_t e, uint32_t prev[], uint32_t next[]) {
                next[prev[e]] = next[e];
                prev[next[e]] = prev[e];
            };
            auto update_flow = [&](uint32_t i) {if (chmin(flow[i], std::min(in_flow[i], out_flow[i]))) heap.siftUp(i); };
            auto remove_node = [&](uint32_t i) {
                for (uint32_t index = in_next[m_edges.size() + i]; index < m_edges.size(); index = in_next[index]) {
                    auto &[from, to, rev, cap] = m_edges[index];
                    erase(index, out_prev, out_next);
                    out_flow[from] -= cap;
                    update_flow(from);
                }
                for (uint32_t index = out_next[m_edges.size() + i]; index < m_edges.size(); index = out_next[index]) {
                    auto &[from, to, rev, cap] = m_edges[index];
                    erase(index, in_prev, in_next);
                    in_flow[to] -= cap;
                    update_flow(to);
                }
            };
            auto push = [&](uint32_t i, _Tp f, uint32_t end, bool forward) {
                std::fill(ex, ex + m_vertexNum, 0);
                ex[i] = f;
                head = tail = 0;
                queue[tail++] = i;
                while (head < tail) {
                    uint32_t cur = queue[head++];
                    if (cur == end) break;
                    auto &index = forward ? out_next[m_edges.size() + cur] : in_next[m_edges.size() + cur];
                    for (_Tp must = ex[cur]; must;) {
                        auto &[from, to, rev, cap] = m_edges[index];
                        _Tp pushed = std::min(must, cap);
                        out_flow[from] -= pushed;
                        in_flow[to] -= pushed;
                        update_flow(from);
                        update_flow(to);
                        uint32_t other = forward ? to : from;
                        if (!ex[other]) queue[tail++] = other;
                        ex[other] += pushed;
                        cap -= pushed;
                        m_edges[rev].cap += pushed;
                        must -= pushed;
                        if (cap) break;
                        if (forward) {
                            erase(index, in_prev, in_next);
                            erase(index, out_prev, out_next);
                        } else {
                            erase(index, out_prev, out_next);
                            erase(index, in_prev, in_next);
                        }
                    }
                }
            };
            while (true) {
                std::fill(depth, depth + m_vertexNum, -1);
                head = tail = 0;
                depth[__source] = 0;
                queue[tail++] = __source;
                while (head < tail && !~depth[__target])
                    for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[from, to, rev, cap] = m_edges[cur]; cap && chmin(depth[to], depth[from] + 1)) queue[tail++] = to;
                if (!~depth[__target]) break;
                std::fill(in_flow, in_flow + m_vertexNum, 0);
                std::fill(out_flow, out_flow + m_vertexNum, 0);
                for (uint32_t i = 0; i < m_vertexNum; i++) in_prev[m_edges.size() + i] = in_next[m_edges.size() + i] = out_prev[m_edges.size() + i] = out_next[m_edges.size() + i] = m_edges.size() + i;
                for (uint32_t index = 0; index < m_edges.size(); index++)
                    if (auto &[from, to, rev, cap] = m_edges[index]; cap && depth[from] + 1 == depth[to] && (depth[to] < depth[__target] || to == __target)) {
                        insert(index, to, in_prev, in_next);
                        insert(index, from, out_prev, out_next);
                        in_flow[to] += cap;
                        out_flow[from] += cap;
                    }
                in_flow[__source] = out_flow[__target] = __infiniteCap;
                std::fill(flow, flow + m_vertexNum, __infiniteCap);
                for (uint32_t i = 0; i < m_vertexNum; i++) update_flow(i);
                while (true) {
                    uint32_t critical = heap.top();
                    if (flow[critical] == __infiniteCap) break;
                    if (_Tp f = flow[critical]) {
                        res += f;
                        push(critical, f, __source, false);
                        push(critical, f, __target, true);
                    }
                    flow[critical] = __infiniteCap;
                    heap.siftDown(critical);
                    remove_node(critical);
                }
            }
            return res;
        }
    };
}

#endif