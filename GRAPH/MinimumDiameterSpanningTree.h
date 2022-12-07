#ifndef __OY_MINIMUMDIAMETERSPANNINGTREE__
#define __OY_MINIMUMDIAMETERSPANNINGTREE__

#include "Dijkstra_heap.h"
#include "Floyd.h"

namespace OY {
    template <typename _Tp>
    struct MinimumDiameterSpanningTree {
        struct _Edge {
            uint32_t from, to;
            _Tp distance;
        };
        uint32_t m_vertexNum;
        _Tp m_infiniteDistance;
        Floyd<_Tp> m_floyd;
        std::vector<_Edge> m_edges;
        MinimumDiameterSpanningTree(uint32_t __vertexNum, uint32_t __edgeNum, _Tp __infiniteDistance = std::numeric_limits<_Tp>::max() / 2) : m_vertexNum(__vertexNum), m_infiniteDistance(__infiniteDistance), m_floyd(__vertexNum, __infiniteDistance) { m_edges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __distance) { m_edges.push_back({__a, __b, __distance}); }
        void calc() {
            for (auto &[from, to, dis] : m_edges)
                m_floyd.addEdge(from, to, dis), m_floyd.addEdge(to, from, dis);
            m_floyd.calc();
        }
        auto getCenterOfVertex() const {
            struct _res {
                uint32_t id;
                _Tp dis;
            };
            _res res;
            res.dis = m_infiniteDistance;
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (chmin(res.dis, *std::max_element(m_floyd.m_distances[i].begin(), m_floyd.m_distances[i].end()))) res.id = i;
            return res;
        }
        auto getCenterOfEdge() const {
            struct _res {
                uint32_t id;
                _Tp double_bias, double_dis;
            };
            uint32_t farthest[m_vertexNum][m_vertexNum];
            for (uint32_t i = 0; i < m_vertexNum; i++) {
                for (uint32_t j = 0; j < m_vertexNum; j++) farthest[i][j] = j;
                std::sort(farthest[i], farthest[i + 1], [&](uint32_t x, uint32_t y) { return m_floyd.m_distances[i][x] > m_floyd.m_distances[i][y]; });
            }
            _res res;
            res.double_dis = m_infiniteDistance;
            for (uint32_t i = 0; i < m_edges.size(); i++) {
                auto &[from, to, dis] = m_edges[i];
                for (uint32_t p = farthest[from][0], j = 1; j < m_vertexNum; j++)
                    if (uint32_t q = farthest[from][j]; m_floyd.m_distances[to][q] > m_floyd.m_distances[to][p]) {
                        if (chmin(res.double_dis, m_floyd.m_distances[from][q] + m_floyd.m_distances[to][p] + dis)) res.id = i, res.double_bias = res.double_dis - m_floyd.m_distances[from][q] * 2;
                        p = q;
                    }
            }
            return res;
        }
        std::vector<uint32_t> getTreeEdgesOfVertexCenter(uint32_t __root) const {
            std::vector<uint32_t> res;
            res.reserve(m_vertexNum - 1);
            Dijkstra_heap<_Tp> dijk(m_vertexNum, m_edges.size(), m_infiniteDistance);
            for (auto &[from, to, dis] : m_edges) dijk.addEdge(from, to, dis), dijk.addEdge(to, from, dis);
            dijk.prepare();
            dijk.setDistance(__root, 0);
            dijk.template calc<true>();
            for (auto &from : dijk.m_from)
                if (~from) res.push_back(from / 2);
            return res;
        }
        std::vector<uint32_t> getTreeEdgesOfEdgeCenter(uint32_t __edgeId, _Tp __doubleBias) const {
            std::vector<uint32_t> res;
            res.reserve(m_vertexNum - 1);
            Dijkstra_heap<_Tp> dijk(m_vertexNum, m_edges.size(), m_infiniteDistance);
            for (auto &[from, to, dis] : m_edges) dijk.addEdge(from, to, dis), dijk.addEdge(to, from, dis);
            dijk.prepare();
            auto &[from, to, dis] = m_edges[__edgeId];
            if (__doubleBias <= dis)
                dijk.setDistance(from, 0), dijk.setDistance(to, dis - __doubleBias);
            else
                dijk.setDistance(to, 0), dijk.setDistance(from, __doubleBias - dis);
            dijk.template calc<true>();
            for (auto &from : dijk.m_from)
                if (~from) res.push_back(from / 2);
            res.push_back(__edgeId);
            return res;
        }
    };
}

#endif