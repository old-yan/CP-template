#ifndef __OY_KINDMANAGER__
#define __OY_KINDMANAGER__

#include <map>
#include "PersistentSegTree.h"

namespace OY {
    template <typename _Tp, typename _Map = std::map<_Tp, uint32_t>>
    struct KindManager {
        PersistentSegTree<int> m_tree;
        _Map m_last;
        std::vector<uint32_t> m_realVersion;
        template <typename _Iterator>
        KindManager(_Iterator __first, _Iterator __last) : m_tree(__last - __first), m_last{} {
            m_realVersion.reserve(__last - __first);
            for (auto it = __first; it != __last; ++it) {
                if (uint32_t last = m_last[*it]) m_tree.add(-1, last - 1, -1);
                m_tree.add(-1, (m_last[*it] = it - __first + 1) - 1, 1);
                m_realVersion.push_back(m_tree.versionCount() - 1);
            }
        }
        uint32_t query(uint32_t __left, uint32_t __right) const { return m_tree.query(m_realVersion[__right], __left, __right); }
    };
}

#endif