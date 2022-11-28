#ifndef __OY_VIRTUALTREE_VECTOR__
#define __OY_VIRTUALTREE_VECTOR__

#include <algorithm>
#include <cstdint>

#include "../DS/Discretizer.h"
#include "HeavyLightDecomposition_vector.h"

namespace OY {
    template <typename _Tree>
    struct VirtualTreeFactory_vector {
        struct _VirtualEdge {
            uint32_t from, to;
        };
        HeavyLightDecomposition_vector<_Tree> m_hld;
        _Tree &m_tree;
        VirtualTreeFactory_vector(_Tree &__tree) : m_hld(__tree), m_tree(__tree) {}
        std::vector<_VirtualEdge> getVirtualEdges(const std::vector<uint32_t> &__keyPoints) const {
            const uint32_t n = __keyPoints.size();
            uint32_t vertexes[n], stack[n], stackLen = 0;
            std::vector<_VirtualEdge> res;
            std::copy(__keyPoints.begin(), __keyPoints.end(), vertexes);
            std::sort(vertexes, vertexes + n, [&](uint32_t x, uint32_t y) { return m_hld.m_pos[x] < m_hld.m_pos[y]; });
            stack[stackLen++] = m_tree.m_root;
            for (uint32_t i = 0; i < n; i++) {
                if (vertexes[i] == stack[stackLen - 1]) continue;
                uint32_t lca = m_hld.lca(vertexes[i], stack[stackLen - 1]), d = m_hld.m_depth[lca], d1 = m_hld.m_depth[stack[stackLen - 1]];
                while (d1 > d)
                    if (uint32_t d2 = m_hld.m_depth[stack[stackLen - 2]]; d2 >= d)
                        res.push_back({stack[stackLen - 1], stack[stackLen - 2]}), stackLen--, d1 = d2;
                    else
                        res.push_back({stack[stackLen - 1], lca}), stack[stackLen - 1] = lca, d1 = d;
                stack[stackLen++] = vertexes[i];
            }
            while (stackLen > 1) res.push_back({stack[stackLen - 1], stack[stackLen - 2]}), stackLen--;
            return res;
        }
        void fillVirtualTree(VectorTree<bool> &__tree, Discretizer<uint32_t> &__dis, const std::vector<uint32_t> &__keyPoints) const {
            __dis.clear();
            std::vector<_VirtualEdge> edges = getVirtualEdges(__keyPoints);
            for (auto &[a, b] : edges) __dis << a << b;
            __dis.prepare();
            __tree.resize(__dis.size());
            for (auto &[a, b] : edges) __tree.addEdge(__dis.rank(a), __dis.rank(b));
            __tree.setRoot(m_tree.m_root);
        }
    };
}

#endif