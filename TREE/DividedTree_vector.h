#ifndef __OY_DIVIDEDTREE_VECTOR__
#define __OY_DIVIDEDTREE_VECTOR__

#include "VectorTree.h"

namespace OY {
    template <typename _Tree>
    auto getDivideTree_vector(_Tree &__tree) {
        VectorTree res(__tree.m_vertexNum);
        std::vector<bool> blocked(__tree.m_vertexNum, false);
        uint32_t queue[__tree.m_vertexNum];
        auto dfs = [&](auto self, uint32_t i) -> uint32_t {
            uint32_t head = 0, tail = 0;
            blocked[i] = true;
            queue[tail++] = i;
            while (head < tail)
                for (auto &adj : __tree.m_adj[queue[head++]])
                    if (uint32_t to = adj.to; !blocked[to]) blocked[to] = true, queue[tail++] = to;
            for (uint32_t i = 0; i < tail; i++) blocked[queue[i]] = false;
            uint32_t centroid;
            auto traverse = [&](auto self, uint32_t i, uint32_t p) -> uint32_t {
                uint32_t size = 1, maxAdj = 0;
                for (auto &adj : __tree.m_adj[i])
                    if (uint32_t to = adj.to; to != p && !blocked[to]) {
                        uint32_t subSize = self(self, to, i);
                        size += subSize;
                        chmax(maxAdj, subSize);
                    }
                chmax(maxAdj, tail - size);
                if (maxAdj <= tail / 2) centroid = i;
                return size;
            };
            traverse(traverse, i, -1);
            blocked[centroid] = true;
            for (auto &adj : __tree.m_adj[centroid])
                if (uint32_t to = adj.to; !blocked[to]) res.addEdge(centroid, self(self, to));
            blocked[centroid] = false;
            return centroid;
        };
        uint32_t root = dfs(dfs, 0);
        res.setRoot(root);
        return res;
    };
}

#endif