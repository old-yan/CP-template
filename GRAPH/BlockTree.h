#ifndef __OY_BLOCKTREE__
#define __OY_BLOCKTREE__

#include "../DS/SiftHeap.h"
#include "../TREE/VectorTree.h"
#include "Tarjan_cut.h"

namespace OY {
    template <typename _Tp = bool>
    struct BlockTree {
        using distance_type = std::conditional_t<std::is_same_v<_Tp, bool>, uint32_t, _Tp>;
        Tarjan_cut<_Tp> m_tarjan;
        VectorTree<_Tp> m_tree;
        std::vector<distance_type> m_cactusCycleLength;
        std::vector<distance_type> m_cactusPosition;
        BlockTree(uint32_t __vertexNum, uint32_t __edgeNum) : m_tarjan(__vertexNum, __edgeNum) {}
        void addEdge(uint32_t __a, uint32_t __b) { m_tarjan.addEdge(__a, __b); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __distance) { m_tarjan.addEdge(__a, __b, __distance); }
        template <bool _IsCactus = false>
        void calc(uint32_t __root = 0, distance_type __infiniteDistance = std::numeric_limits<distance_type>::max() / 2) {
            m_tarjan.template calc<true>();
            m_tree.resize(m_tarjan.m_vertexNum + m_tarjan.m_vbcc.size());
            if constexpr (std::is_same_v<_Tp, bool>)
                for (uint32_t i = 0; i < m_tarjan.m_vbcc.size(); i++)
                    for (uint32_t a : m_tarjan.m_vbcc[i]) m_tree.addEdge(a, m_tarjan.m_vertexNum + i);
            else if constexpr (_IsCactus) {
                std::tie(m_cactusCycleLength, m_cactusPosition) = m_tarjan.getCactusCyclePos();
                for (uint32_t i = 0; i < m_tarjan.m_vbcc.size(); i++)
                    for (uint32_t a : m_tarjan.m_vbcc[i]) m_tree.addEdge(a, m_tarjan.m_vertexNum + i, queryCactusCycleDistance(m_tarjan.m_vbcc[i][0], a, i));
            } else {
                std::vector<distance_type> dis(m_tarjan.m_vertexNum, __infiniteDistance);
                SiftHeap<SiftGetter<std::vector<distance_type>>, std::greater<distance_type>> heap(m_tarjan.m_vertexNum, dis, std::greater<distance_type>());
                dis[__root] = 0;
                heap.push(__root);
                while (heap.size()) {
                    uint32_t from = heap.top();
                    distance_type curDistance = dis[from];
                    heap.pop();
                    for (uint32_t cur = m_tarjan.m_starts[from], end = m_tarjan.m_starts[from + 1]; cur != end; cur++)
                        if (auto &[index, to] = m_tarjan.m_edges[cur]; chmin(dis[to], curDistance + m_tarjan.m_rawEdges[index].distance)) heap.push(to);
                }
                for (uint32_t i = 0; i < m_tarjan.m_vbcc.size(); i++) {
                    uint32_t highest;
                    distance_type highest_dis = __infiniteDistance;
                    for (uint32_t a : m_tarjan.m_vbcc[i])
                        if (chmin(highest_dis, dis[a])) highest = a;
                    for (uint32_t a : m_tarjan.m_vbcc[i]) m_tree.addEdge(a, m_tarjan.m_vertexNum + i, dis[a] - highest_dis);
                }
            }
            m_tree.setRoot(__root);
        }
        distance_type queryCactusCycleDistance(uint32_t __a, uint32_t __b, uint32_t __groupId) const {
            distance_type cycle = m_cactusCycleLength[__groupId], d1 = m_cactusPosition[__a], d2 = m_cactusPosition[__b], dif = d1 > d2 ? d1 - d2 : d2 - d1;
            return std::min(dif, cycle - dif);
        }
    };
    BlockTree(uint32_t, uint32_t)->BlockTree<bool>;
}

#endif