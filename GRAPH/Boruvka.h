#ifndef __OY_BORUVKA__
#define __OY_BORUVKA__

#include "../DS/UnionFind.h"

namespace OY {
    template <typename _Tp>
    struct Boruvka {
        struct _Edge {
            uint32_t from, to;
            _Tp cost;
        };
        std::vector<_Edge> m_edges;
        std::vector<bool> m_used;
        UnionFind m_union;
        uint32_t m_vertexNum;
        Boruvka(uint32_t __vertexNum, uint32_t __edgeNum) : m_union(__vertexNum), m_vertexNum(__vertexNum) { m_edges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { m_edges.push_back({__a, __b, __cost}); }
        bool calc() {
            m_used.resize(m_edges.size(), false);
            uint32_t closest[m_vertexNum];
            std::fill(closest, closest + m_vertexNum, -1);
            while (m_union.count() > 1) {
                bool flag = false;
                for (uint32_t index = 0; index < m_edges.size(); index++) {
                    auto [from, to, cost] = m_edges[index];
                    if (from = m_union.find(from), to = m_union.find(to); from != to) {
                        flag = true;
                        if (!~closest[from] || m_edges[closest[from]].cost > cost) closest[from] = index;
                        if (!~closest[to] || m_edges[closest[to]].cost > cost) closest[to] = index;
                    }
                }
                if (!flag) break;
                for (uint32_t &d : closest)
                    if (~d && m_union.uniteBySize(m_edges[d].from, m_edges[d].to)) m_used[d] = true;
                std::fill(closest, closest + m_vertexNum, -1);
            }
            return m_union.count() == 1;
        }
        _Tp totalCost() const {
            _Tp res = 0;
            for (uint32_t index = 0; index < m_used.size(); index++)
                if (m_used[index]) res += m_edges[index].cost;
            return res;
        }
    };
}

#endif