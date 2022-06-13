#ifndef __OY_KRUSKAL__
#define __OY_KRUSKAL__

#include "../DS/UnionFind.h"
#include <cstdint>

namespace OY {
    template <typename _Tp>
    struct Kruskal {
        struct _Edge {
            uint32_t index, from, to;
            _Tp cost;
        };
        std::vector<_Edge> m_edges;
        std::vector<bool> m_used;
        UnionFind m_union;
        uint32_t m_vertexNum;
        _Tp m_totalCost;
        Kruskal(uint32_t __vertexNum, uint32_t __edgeNum) : m_union(__vertexNum), m_vertexNum(__vertexNum), m_totalCost(0) { m_edges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { m_edges.push_back({uint32_t(m_edges.size()), __a, __b, __cost}); }
        bool calc() {
            m_used.resize(m_edges.size(), false);
            std::sort(m_edges.begin(), m_edges.end(), [](const _Edge &x, const _Edge &y) { return x.cost < y.cost; });
            for (auto [index, from, to, cost] : m_edges)
                if (m_union.uniteBySize(from, to)) {
                    m_used[index] = true;
                    m_totalCost += cost;
                    if (m_union.count() == 1) break;
                }
            return m_union.count() == 1;
        }
        _Tp totalCost() const { return m_totalCost; }
    };
}

#endif