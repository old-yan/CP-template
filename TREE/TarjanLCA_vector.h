#ifndef __OY_TARJANLCA_VECTOR__
#define __OY_TARJANLCA_VECTOR__

#include "VectorTree.h"

namespace OY {
    template <typename _Tree>
    struct TarjanLCA_vector {
        _Tree &m_tree;
        struct _QueryEdge {
            uint32_t to, index;
        };
        uint32_t m_cursor;
        std::vector<uint32_t> m_answer, m_find;
        std::vector<std::vector<_QueryEdge>> m_queries;
        TarjanLCA_vector(_Tree &__tree) : m_cursor(0), m_tree(__tree), m_queries(__tree.m_vertexNum) {}
        void addQuery(uint32_t __a, uint32_t __b) { m_queries[__a].push_back({__b, m_cursor}), m_queries[__b].push_back({__a, m_cursor++}); }
        void calc() {
            m_answer.resize(m_cursor);
            m_find.resize(m_cursor);
            std::iota(m_find.begin(), m_find.end(), 0);
            std::vector<bool> visit(m_tree.m_vertexNum, false);
            auto dfs = [&](auto self, uint32_t i) -> void {
                m_find[i] = i;
                visit[i] = true;
                for (auto &adj : m_tree.m_adj[i])
                    if (uint32_t to = adj.to; !visit[to]) {
                        self(self, to);
                        m_find[to] = i;
                    }
                for (auto [to, index] : m_queries[i])
                    if (visit[to]) m_answer[index] = Find(to);
            };
            dfs(dfs, m_tree.m_root);
        }
        uint32_t Find(uint32_t __i) { return m_find[__i] == __i ? __i : m_find[__i] = Find(m_find[__i]); }
    };
}

#endif