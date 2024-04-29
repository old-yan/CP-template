/*
最后修改:
20240429
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LONGSHORTDECOMPOSITION__
#define __OY_LONGSHORTDECOMPOSITION__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace LSD {
        using size_type = uint32_t;
        template <typename Tree, size_type MAX_LEVEL = 32>
        struct Table {
            struct node {
                size_type m_dfn, m_dep, m_height, m_heavy;
            };
            Tree *m_rooted_tree;
            std::vector<node> m_info;
            std::vector<size_type> m_seq, m_fa[MAX_LEVEL];
            size_type m_level;
            void _tree_dfs1(size_type a, size_type p) {
                m_info[a].m_height = 1, m_info[a].m_heavy = -1, m_fa[0][a] = p;
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) {
                    if (to != p) {
                        _tree_dfs1(to, a);
                        size_type to_height = m_info[to].m_height;
                        m_info[a].m_height = std::max(m_info[a].m_height, to_height + 1);
                        if (!~m_info[a].m_heavy || to_height > m_info[m_info[a].m_heavy].m_height) m_info[a].m_heavy = to;
                    }
                });
            }
            void _tree_dfs2(size_type a, size_type p, size_type &cursor, size_type height) {
                if (m_info[a].m_height == height) {
                    cursor += height;
                    for (size_type i = cursor - 1, j = height, x = a; j-- && ~x;) m_seq[i--] = x = m_fa[0][x];
                }
                m_seq[cursor] = a;
                if (~p) m_info[a].m_dep = m_info[p].m_dep + 1;
                m_info[a].m_dfn = cursor++;
                size_type heavy = m_info[a].m_heavy;
                if (~heavy) {
                    _tree_dfs2(heavy, a, cursor, height);
                    m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) { if (to != p && to != heavy) _tree_dfs2(to, a, cursor,m_info[to].m_height); });
                }
            }
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_info.resize(m_rooted_tree->vertex_cnt()), m_seq.resize(m_rooted_tree->vertex_cnt() * 2);
                m_fa[0].resize(m_rooted_tree->vertex_cnt());
                _tree_dfs1(m_rooted_tree->m_root, -1);
                m_level = std::bit_width(m_info[m_rooted_tree->m_root].m_height - 1);
                for (size_type j = 1; j != m_level; j++) {
                    m_fa[j].resize(m_rooted_tree->vertex_cnt());
                    size_type *prev = m_fa[j - 1].data(), *cur = m_fa[j].data();
                    for (size_type i = 0; i != m_rooted_tree->vertex_cnt(); i++) cur[i] = ~prev[i] ? prev[prev[i]] : -1;
                }
                size_type cursor = 0;
                _tree_dfs2(m_rooted_tree->m_root, -1, cursor, m_info[m_rooted_tree->m_root].m_height);
                for (size_type i = 0; i != m_rooted_tree->vertex_cnt(); i++)
                    if (~m_fa[0][i]) m_fa[0][i] = m_info[m_fa[0][i]].m_dfn + 1;
                for (size_type j = 1; j != m_level; j++) {
                    size_type *cur = m_fa[j].data();
                    for (size_type i = 0; i != m_rooted_tree->vertex_cnt(); i++)
                        if (~cur[i]) cur[i] = m_info[cur[i]].m_dfn + (1 << j);
                }
            }
            size_type get_ancestor(size_type a, size_type n) const {
                if (n > m_info[a].m_dep) return -1;
                if (!n) return a;
                return m_seq[m_fa[std::bit_width(n) - 1][a] - n];
            }
            size_type find_parent(size_type a) const { return m_seq[m_fa[0][a] - 1]; }
            size_type find_son(size_type a, size_type b) const { return get_ancestor(b, m_info[b].m_dep - m_info[a].m_dep - 1); }
            size_type get_depth(size_type a) const { return m_info[a].m_dep; }
            size_type calc(size_type a, size_type b) const {
                if (m_info[a].m_dep > m_info[b].m_dep) std::swap(a, b);
                size_type low = 0, high = m_info[a].m_dep, d = m_info[b].m_dep - m_info[a].m_dep, best;
                while (low < high) {
                    size_type mid = (low + high) / 2;
                    if (get_ancestor(a, mid) == get_ancestor(b, mid + d))
                        high = mid;
                    else
                        low = mid + 1;
                }
                return get_ancestor(a, low);
            }
        };
    }
}

#endif