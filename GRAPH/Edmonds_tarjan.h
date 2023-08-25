#ifndef __OY_EDMONDS_TARJAN__
#define __OY_EDMONDS_TARJAN__

#include <cstdint>
#include <limits>
#include "../DS/UnionFind.h"

namespace OY {
    template <typename _Tp>
    struct Edmonds_tarjan {
        struct _Edge {
            uint32_t from, to;
            _Tp cost;
        };
        std::vector<_Edge> m_edges;
        uint32_t m_vertexNum;
        _Tp m_totalCost;
        std::vector<int> m_useCount;
        Edmonds_tarjan(uint32_t __vertexNum, uint32_t __edgeNum) : m_vertexNum(__vertexNum), m_totalCost(0) { m_edges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { m_edges.push_back({__a, __b, __cost}); }
        template <bool _GetPath = false>
        bool calc(uint32_t __root, _Tp __infiniteCost = std::numeric_limits<_Tp>::max() / 2) {
            struct _edge {
                _Tp cost;
                uint32_t index;
                bool operator>(const _edge &other) const { return cost > other.cost; }
            };
            struct _heap {
                std::vector<_edge> in_edges;
                _Tp inc;
                void push(const _edge &e) {
                    in_edges.push_back(e);
                    std::push_heap(in_edges.begin(), in_edges.end(), std::greater<_edge>());
                }
                void pop() {
                    std::pop_heap(in_edges.begin(), in_edges.end(), std::greater<_edge>());
                    in_edges.pop_back();
                }
                void join(_heap &other) {
                    if (in_edges.size() < other.in_edges.size()) {
                        std::swap(in_edges, other.in_edges);
                        std::swap(inc, other.inc);
                    }
                    for (auto &[cost, from] : other.in_edges) push({cost + other.inc - inc, from});
                }
            } heap[m_vertexNum * 2];
            UnionFind u(m_vertexNum * 2);
            uint32_t parent[m_vertexNum * 2], visit[m_vertexNum * 2], fromEdge[m_vertexNum * 2], cnt;
            for (uint32_t i = 0; i < m_vertexNum * 2; i++) heap[i].inc = 0;
            for (uint32_t index = 0; index < m_edges.size(); index++) {
                auto &[from, to, cost] = m_edges[index];
                heap[to].push({cost, index});
            }
            std::fill(visit, visit + m_vertexNum * 2, -1);
            if constexpr (_GetPath) {
                std::iota(parent, parent + m_vertexNum * 2, 0);
                std::fill(fromEdge, fromEdge + m_vertexNum * 2, -1);
            }
            visit[__root] = __root;
            cnt = m_vertexNum;
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (uint32_t cur = i; !~visit[cur])
                    while (true) {
                        do {
                            while (heap[cur].in_edges.size() && u.find(m_edges[heap[cur].in_edges.front().index].from) == cur) heap[cur].pop();
                            if (heap[cur].in_edges.empty()) return false;
                            visit[cur] = i;
                            auto &[cost, index] = heap[cur].in_edges.front();
                            m_totalCost += cost + heap[cur].inc;
                            if constexpr (_GetPath) fromEdge[cur] = index;
                            cur = u.find(m_edges[index].from);
                        } while (!~visit[cur]);
                        if (visit[cur] != i) break;
                        uint32_t it = cur;
                        do {
                            auto [cost, index] = heap[it].in_edges.front();
                            heap[it].pop();
                            heap[it].inc = -cost;
                            heap[cnt].join(heap[it]);
                            u.unite_to(it, cnt);
                            if constexpr (_GetPath) parent[it] = cnt;
                            it = u.find(m_edges[index].from);
                        } while (it != cnt);
                        cur = cnt++;
                    }
            if constexpr (_GetPath) {
                m_useCount.resize(m_edges.size(), false);
                for (uint32_t i = cnt - 1; ~i; i--)
                    if (visit[i] != __root) {
                        m_useCount[fromEdge[i]] = true;
                        for (uint32_t to = m_edges[fromEdge[i]].to; visit[to] != __root; to = parent[to]) visit[to] = __root;
                    }
            }
            return true;
        }
    };
}

#endif