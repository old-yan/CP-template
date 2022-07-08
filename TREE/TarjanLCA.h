#ifndef __OY_TARJANLCA__
#define __OY_TARJANLCA__

#include <bitset>
#include "Tree.h"

namespace OY {
    template <typename _Tree, uint32_t _MAXM>
    struct TarjanLCA {
        static constexpr uint32_t _MAXN = sizeof(_Tree::m_edges) / sizeof(*_Tree::m_edges);
        _Tree &m_tree;
        struct _QueryEdge {
            uint32_t from, to;
        } m_queryEdges[_MAXM];
        uint32_t m_answer[_MAXM];
        uint32_t m_queryIndex[_MAXM * 2];
        uint32_t m_queryTo[_MAXM * 2];
        uint32_t m_queryStarts[_MAXN + 1];
        uint32_t m_find[_MAXN];
        uint32_t m_cursor;
        TarjanLCA(_Tree &__tree) : m_tree(__tree), m_cursor(0) {}
        void addQuery(uint32_t __a, uint32_t __b) { m_queryEdges[m_cursor++] = {__a, __b}; }
        void calc() {
            std::fill(m_queryStarts, m_queryStarts + m_tree.m_vertexNum + 1, 0);
            for (uint32_t i = 0; i < m_cursor; i++)
                if (auto &[from, to] = m_queryEdges[i]; from != to) {
                    m_queryStarts[from + 1]++;
                    m_queryStarts[to + 1]++;
                }
            std::partial_sum(m_queryStarts, m_queryStarts + m_tree.m_vertexNum + 1, m_queryStarts);
            uint32_t cursor[m_tree.m_vertexNum];
            std::copy(m_queryStarts, m_queryStarts + m_tree.m_vertexNum, cursor);
            for (uint32_t i = 0; i < m_cursor; i++)
                if (auto &[from, to] = m_queryEdges[i]; from != to) {
                    m_queryTo[cursor[from]] = to;
                    m_queryIndex[cursor[from]++] = i;
                    m_queryTo[cursor[to]] = from;
                    m_queryIndex[cursor[to]++] = i;
                } else
                    m_answer[i] = from;
            std::bitset<_MAXN> visit;
            std::iota(m_find, m_find + m_tree.m_vertexNum, 0);
            auto dfs = [&](auto self, uint32_t i) -> void {
                m_find[i] = i;
                visit.set(i);
                for (uint32_t cur = m_tree.m_starts[i], end = m_tree.m_starts[i + 1]; cur != end; cur++)
                    if (uint32_t to = m_tree.m_to[cur]; !visit[to]) {
                        self(self, to);
                        m_find[to] = i;
                    }
                for (uint32_t cur = m_queryStarts[i], end = m_queryStarts[i + 1]; cur != end; cur++)
                    if (uint32_t to = m_queryTo[cur]; visit[to]) m_answer[m_queryIndex[cur]] = Find(to);
            };
            dfs(dfs, m_tree.m_root);
        }
        uint32_t Find(uint32_t __i) {
            return m_find[__i] == __i ? __i : m_find[__i] = Find(m_find[__i]);
        }
    };
}

#endif