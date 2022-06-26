#ifndef __OY_HLPP__
#define __OY_HLPP__

#include <algorithm>
#include <cstdint>
#include <limits>
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct HLPP {
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
        HLPP(uint32_t __vertexNum, uint32_t __edgeNum) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum) { m_rawEdges.reserve(__edgeNum); }
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
            uint32_t queue[m_vertexNum], height[m_vertexNum], ex_next[m_vertexNum * 2], gap_prev[m_vertexNum * 2], gap_next[m_vertexNum * 2], ex_highest = 0, gap_highest = 0, discharge_count, it[m_vertexNum], end[m_vertexNum];
            _Tp ex[m_vertexNum];
            auto ex_insert = [&](uint32_t i, uint32_t h) {
                ex_next[i] = ex_next[m_vertexNum + h];
                ex_next[m_vertexNum + h] = i;
                chmax(ex_highest, h);
            };
            auto gap_insert = [&](uint32_t i, uint32_t h) {
                gap_prev[i] = m_vertexNum + h;
                gap_next[i] = gap_next[m_vertexNum + h];
                gap_prev[gap_next[i]] = gap_next[gap_prev[i]] = i;
                chmax(gap_highest, h);
            };
            auto gap_erase = [&](uint32_t i) {
                gap_next[gap_prev[i]] = gap_next[i];
                gap_prev[gap_next[i]] = gap_prev[i];
            };
            auto ex_add = [&](uint32_t i, _Tp f) {
                ex[i] += f;
                if (ex[i] == f) ex_insert(i, height[i]);
            };
            auto ex_remove = [&](uint32_t i, _Tp f) { ex[i] -= f; };
            auto update_height = [&](uint32_t i, uint32_t h) {
                if (~height[i]) gap_erase(i);
                height[i] = h;
                if (~h) {
                    gap_insert(i, h);
                    if (ex[i] > 0) ex_insert(i, h);
                }
            };
            auto global_relabel = [&] {
                discharge_count = 0;
                std::iota(ex_next + m_vertexNum, ex_next + m_vertexNum * 2, m_vertexNum);
                std::iota(gap_prev + m_vertexNum, gap_prev + m_vertexNum * 2, m_vertexNum);
                std::iota(gap_next + m_vertexNum, gap_next + m_vertexNum * 2, m_vertexNum);
                std::fill(height, height + m_vertexNum, -1);
                height[__target] = 0;
                uint32_t head = 0, tail = 0;
                queue[tail++] = __target;
                while (head < tail)
                    for (uint32_t from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[to, rev, cap] = m_edges[cur]; m_edges[rev].cap && height[to] > height[from] + 1) {
                            update_height(to, height[from] + 1);
                            queue[tail++] = to;
                        }
            };
            auto push = [&](uint32_t from, uint32_t to, uint32_t rev, _Tp &cap, _Tp f) {
                ex_remove(from, f);
                ex_add(to, f);
                cap -= f;
                m_edges[rev].cap += f;
            };
            auto discharge = [&](uint32_t i) {
                uint32_t h = m_vertexNum;
                uint32_t pos = it[i];
                for (uint32_t &cur = it[i]; cur < end[i]; cur++)
                    if (auto &[to, rev, cap] = m_edges[cur]; cap) {
                        if (height[i] == height[to] + 1) {
                            push(i, to, rev, cap, std::min(ex[i], cap));
                            if (!ex[i]) return;
                        } else
                            chmin(h, height[to]);
                    }
                it[i] = m_starts[i];
                for (uint32_t &cur = it[i]; cur < pos; cur++)
                    if (auto &[to, rev, cap] = m_edges[cur]; cap) {
                        if (height[i] == height[to] + 1) {
                            push(i, to, rev, cap, std::min(ex[i], cap));
                            if (!ex[i]) return;
                        } else
                            chmin(h, height[to]);
                    }
                discharge_count++;
                if (gap_next[gap_next[m_vertexNum + height[i]]] < m_vertexNum)
                    update_height(i, h == m_vertexNum ? -1 : h + 1);
                else {
                    uint32_t oldh = height[i];
                    for (h = oldh; h <= gap_highest; h++)
                        while (gap_next[m_vertexNum + h] < m_vertexNum) {
                            uint32_t j = gap_next[m_vertexNum + h];
                            height[j] = -1;
                            gap_erase(j);
                        }
                    gap_highest = oldh - 1;
                }
            };
            for (uint32_t i = 0; i < m_vertexNum; i++) it[i] = m_starts[i];
            for (uint32_t i = 0; i < m_vertexNum; i++) end[i] = m_starts[i + 1];
            std::fill(ex, ex + m_vertexNum, 0);
            global_relabel();
            ex_add(__source, __infiniteCap);
            ex_remove(__target, __infiniteCap);
            while (~ex_highest) {
                while (true) {
                    uint32_t i = ex_next[m_vertexNum + ex_highest];
                    if (i >= m_vertexNum) break;
                    ex_next[m_vertexNum + ex_highest] = ex_next[i];
                    if (height[i] != ex_highest) continue;
                    discharge(i);
                    if (discharge_count >= 4 * m_vertexNum) global_relabel();
                }
                ex_highest--;
            }
            return ex[__target] + __infiniteCap;
        }
    };
}

#endif