#ifndef __OY_TREEHASH__
#define __OY_TREEHASH__

#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include "../DS/MemoryPool.h"
#include "Tree.h"

namespace OY {
    struct _TreeHashTrieNode : MemoryPool<_TreeHashTrieNode> {
        uint32_t m_id;
        std::unordered_map<uint32_t, _TreeHashTrieNode *> m_sons;
    };
    _TreeHashTrieNode g_treeHashTrie{{}, 0, {}};
    uint32_t g_treeHashCount = 1;
    template <typename _Tree>
    std::vector<uint32_t> getTreeHash(_Tree &__tree, uint32_t __root) {
        __tree.setRoot(__root);
        std::vector<uint32_t> res(__tree.m_vertexNum);
        uint32_t deg[__tree.m_vertexNum], queue[__tree.m_vertexNum], head = 0, tail = 0;
        std::fill(deg, deg + __tree.m_vertexNum, 0);
        std::adjacent_difference(__tree.m_starts + 1, __tree.m_starts + __tree.m_vertexNum + 1, deg);
        for (uint32_t i = 0; i < __tree.m_vertexNum; i++) deg[i]--;
        deg[__tree.m_root]++;
        for (uint32_t i = 0; i < __tree.m_vertexNum; i++)
            if (!deg[i]) queue[tail++] = i;
        while (head < tail) {
            uint32_t i = queue[head++], childNum = __tree.m_starts[i + 1] - __tree.m_starts[i] - (i != __tree.m_root);
            uint32_t childHash[childNum];
            for (uint32_t cur = __tree.m_starts[i] + (i != __tree.m_root), index = 0; index < childNum; cur++) childHash[index++] = res[__tree.m_to[cur]];
            std::sort(childHash, childHash + childNum);
            _TreeHashTrieNode *cur = &g_treeHashTrie;
            for (uint32_t a : childHash) {
                auto find = cur->m_sons.find(a);
                if (find == cur->m_sons.end()) find = cur->m_sons.emplace(a, new _TreeHashTrieNode{{}, uint32_t(-1), {}}).first;
                cur = find->second;
            }
            if (!~cur->m_id) cur->m_id = g_treeHashCount++;
            res[i] = cur->m_id;
            if (i != __tree.m_root)
                if (uint32_t p = __tree.m_to[__tree.m_starts[i]]; !--deg[p]) queue[tail++] = p;
        }
        return res;
    }
    template <typename _Tree>
    std::pair<uint32_t, uint32_t> getTreeHash(_Tree &__tree) {
        auto centroid = __tree.getCentroid();
        auto &[r1, r2] = centroid;
        r1 = OY::getTreeHash(__tree, r1)[r1];
        if (~r2) r2 = OY::getTreeHash(__tree, r2)[r2];
        if (r1 > r2) std::swap(r1, r2);
        return centroid;
    }
}

#endif