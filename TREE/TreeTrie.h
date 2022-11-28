#ifndef __OY_TREETRIE__
#define __OY_TREETRIE__

#include <algorithm>
#include <cstdint>
#include <map>

#include "Tree.h"

namespace OY {
    struct _TreeTrieNode {
        uint32_t m_id;
        std::map<uint32_t, _TreeTrieNode *> m_sons;
        _TreeTrieNode(uint32_t __id) : m_id(__id) {}
    };
    _TreeTrieNode g_treeTrie(-1);
    uint32_t g_treeTrieIdCount = -1;
    template <typename _Tree>
    std::vector<uint32_t> getTreeTrieId(_Tree &__tree, uint32_t __root) {
        __tree.setRoot(__root);
        std::vector<uint32_t> res(__tree.m_vertexNum);
        uint32_t deg[__tree.m_vertexNum], queue[__tree.m_vertexNum], head = 0, tail = 0;
        std::adjacent_difference(__tree.m_starts + 1, __tree.m_starts + __tree.m_vertexNum + 1, deg);
        for (uint32_t i = 0; i < __tree.m_vertexNum; i++) deg[i]--;
        deg[__tree.m_root]++;
        for (uint32_t i = 0; i < __tree.m_vertexNum; i++)
            if (!deg[i]) queue[tail++] = i;
        while (head < tail) {
            uint32_t i = queue[head++], childNum = __tree.m_starts[i + 1] - __tree.m_starts[i] - (i != __tree.m_root), childId[childNum];
            for (uint32_t cur = __tree.m_starts[i] + (i != __tree.m_root), index = 0; index < childNum; cur++) childId[index++] = res[__tree.m_to[cur]];
            std::sort(childId, childId + childNum);
            _TreeTrieNode *cur = &g_treeTrie;
            for (uint32_t a : childId) {
                auto it = cur->m_sons.try_emplace(a, nullptr).first;
                if (!it->second) it->second = new _TreeTrieNode(-1);
                cur = it->second;
            }
            if (!~cur->m_id) cur->m_id = ++g_treeTrieIdCount;
            res[i] = cur->m_id;
            if (i != __tree.m_root)
                if (uint32_t p = __tree.m_to[__tree.m_starts[i]]; !--deg[p]) queue[tail++] = p;
        }
        return res;
    }
    template <typename _Tree>
    std::pair<uint32_t, uint32_t> getTreeTrieId(_Tree &__tree) {
        auto centroid = __tree.getCentroid();
        auto &[r1, r2] = centroid;
        r1 = getTreeTrieId(__tree, r1)[r1];
        if (~r2) r2 = getTreeTrieId(__tree, r2)[r2];
        if (r1 > r2) std::swap(r1, r2);
        return centroid;
    }
}

#endif