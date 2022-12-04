#ifndef __OY_KTHPATH__
#define __OY_KTHPATH__

#include <queue>

#include "Dijkstra_heap.h"

namespace OY {
    template <typename _Tp, bool _PassTarget = false>
    struct KthPath {
        struct _Edge {
            uint32_t index, to;
            _Tp cost;
        };
        struct _ExtraCost {
            uint32_t to, s, lchild, rchild;
            _Tp cost;
            _ExtraCost(uint32_t _to, _Tp _cost) : to(_to), lchild(-1), rchild(-1), cost(_cost) {}
        };
        struct _CostNode {
            uint32_t to, it;
            _Tp cost;
            bool operator<(const _CostNode &_other) const { return cost > _other.cost; }
        };
        uint32_t m_source, m_target;
        _Tp m_infiniteDistance;
        Dijkstra_heap<_Tp> m_reverseGraph;
        std::vector<std::vector<_Edge>> m_adj;
        std::vector<uint32_t> m_vertexes, m_extraCosts;
        std::vector<_ExtraCost> m_extraCostBuffer;
        std::priority_queue<_CostNode> m_queue;
        uint32_t raw_merge(uint32_t __a, uint32_t __b) {
            if (!~__b) return __a;
            if (m_extraCostBuffer[__a].cost > m_extraCostBuffer[__b].cost) std::swap(__a, __b);
            m_extraCostBuffer[__a].rchild = raw_merge(__b, m_extraCostBuffer[__a].rchild);
            if (!~m_extraCostBuffer[__a].lchild || m_extraCostBuffer[m_extraCostBuffer[__a].rchild].s > m_extraCostBuffer[m_extraCostBuffer[__a].lchild].s) std::swap(m_extraCostBuffer[__a].lchild, m_extraCostBuffer[__a].rchild);
            m_extraCostBuffer[__a].s = m_extraCostBuffer[m_extraCostBuffer[__a].lchild].s + 1;
            return __a;
        }
        uint32_t safe_merge(uint32_t __a, uint32_t __b) {
            if (!~__b) return __a;
            if (m_extraCostBuffer[__a].cost > m_extraCostBuffer[__b].cost) std::swap(__a, __b);
            uint32_t p = m_extraCostBuffer.size();
            m_extraCostBuffer.push_back(m_extraCostBuffer[__a]);
            m_extraCostBuffer[p].rchild = safe_merge(__b, m_extraCostBuffer[p].rchild);
            if (!~m_extraCostBuffer[p].lchild || m_extraCostBuffer[m_extraCostBuffer[p].rchild].s > m_extraCostBuffer[m_extraCostBuffer[p].lchild].s) std::swap(m_extraCostBuffer[p].lchild, m_extraCostBuffer[p].rchild);
            m_extraCostBuffer[p].s = m_extraCostBuffer[m_extraCostBuffer[p].lchild].s + 1;
            return p;
        }
        KthPath(uint32_t __vertexNum, uint32_t __edgeNum, _Tp __infiniteDistance = std::numeric_limits<_Tp>::max() / 2) : m_infiniteDistance(__infiniteDistance), m_reverseGraph(__vertexNum, __edgeNum, __infiniteDistance), m_adj(__vertexNum), m_vertexes(__vertexNum), m_extraCosts(__vertexNum, -1) {}
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { m_reverseGraph.addEdge(__b, __a, __cost); }
        void setting(uint32_t __source, uint32_t __target) { m_source = __source, m_target = __target; }
        void prepare() {
            m_reverseGraph.prepare();
            m_reverseGraph.setDistance(m_target, _Tp(0));
            m_reverseGraph.template calc<true>();
            if (m_reverseGraph.m_distances[m_source] == m_infiniteDistance) return;
            uint32_t deg[m_reverseGraph.m_vertexNum];
            std::fill_n(deg, m_reverseGraph.m_vertexNum, 0);
            for (auto &[b, a, cost] : m_reverseGraph.m_rawEdges) deg[a]++;
            for (uint32_t i = 0; i < m_adj.size(); i++) m_adj[i].reserve(deg[i]);
            for (uint32_t i = 0; i < m_reverseGraph.m_rawEdges.size(); i++) {
                auto &[b, a, cost] = m_reverseGraph.m_rawEdges[i];
                m_adj[a].push_back({i, b, cost});
            }
            for (uint32_t i = 0; i < m_vertexes.size(); i++) m_vertexes[i] = i;
            std::sort(m_vertexes.begin(), m_vertexes.end(), [&](uint32_t x, uint32_t y) { return m_reverseGraph.m_distances[x] < m_reverseGraph.m_distances[y]; });
            m_extraCostBuffer.reserve(m_reverseGraph.m_vertexNum + m_reverseGraph.m_rawEdges.size());
            for (uint32_t i = !_PassTarget; i < m_vertexes.size(); i++) {
                uint32_t x = m_vertexes[i], root = -1;
                for (auto &[index, to, cost] : m_adj[x])
                    if (index != m_reverseGraph.m_from[x]) m_extraCostBuffer.emplace_back(to, m_reverseGraph.m_distances[to] + cost - m_reverseGraph.m_distances[x]), root = raw_merge(m_extraCostBuffer.size() - 1, root);
                if (~m_reverseGraph.m_from[x])
                    if (uint32_t from = m_reverseGraph.m_rawEdges[m_reverseGraph.m_from[x]].from; ~m_extraCosts[from]) root = safe_merge(m_extraCosts[from], root);
                m_extraCosts[x] = root;
            }
            m_queue.push({uint32_t(-1), uint32_t(-1), m_reverseGraph.m_distances[m_source]});
            if (uint32_t root = m_extraCosts[m_source]; ~root) m_queue.push({m_extraCostBuffer[root].to, root, m_reverseGraph.m_distances[m_source] + m_extraCostBuffer[root].cost});
        }
        _Tp next() {
            if (m_queue.empty()) return m_infiniteDistance;
            auto [to, it, cost] = m_queue.top();
            m_queue.pop();
            if (!~it) return cost;
            if (uint32_t it2 = m_extraCostBuffer[it].lchild; ~it2) m_queue.push({m_extraCostBuffer[it2].to, it2, cost - m_extraCostBuffer[it].cost + m_extraCostBuffer[it2].cost});
            if (uint32_t it2 = m_extraCostBuffer[it].rchild; ~it2) m_queue.push({m_extraCostBuffer[it2].to, it2, cost - m_extraCostBuffer[it].cost + m_extraCostBuffer[it2].cost});
            if (uint32_t it2 = m_extraCosts[m_extraCostBuffer[it].to]; ~it2) m_queue.push({m_extraCostBuffer[it2].to, it2, cost + m_extraCostBuffer[it2].cost});
            return cost;
        }
    };
}

#endif