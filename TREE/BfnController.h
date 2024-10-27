/*
最后修改:
20241026
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BFNCONTROLLER__
#define __OY_BFNCONTROLLER__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace BFN {
        using size_type = uint32_t;
        template <typename Tree>
        struct Table {
            struct node {
                size_type m_bfn, m_dfn, m_dep;
            };
            Tree *m_rooted_tree;
            std::vector<node> m_info;
            std::vector<size_type> m_seq, m_bfs_dfn_seq, m_level_start;
            void _tree_dfs(size_type a, size_type p, size_type &cursor) {
                m_info[a].m_dep = ~p ? m_info[p].m_dep + 1 : 0;
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) {
                    if (to != p) _tree_dfs(to, a, cursor);
                });
                m_info[a].m_dfn = cursor++;
            }
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                size_type n = m_rooted_tree->vertex_cnt(), cursor{}, maxd{};
                m_info.resize(n);
                _tree_dfs(m_rooted_tree->m_root, -1, cursor);
                for (size_type i = 0; i != n; i++) maxd = std::max(maxd, m_info[i].m_dep);
                m_level_start.resize(maxd + 2), m_seq.resize(n), m_bfs_dfn_seq.resize(n), cursor = 0;
                size_type *p = m_seq.data(), *q = m_bfs_dfn_seq.data();
                auto push = [&](size_type x) { m_info[x].m_bfn = cursor, p[cursor] = x, q[cursor] = m_info[x].m_dfn, cursor++; };
                push(m_rooted_tree->m_root);
                for (size_type d = 1; d <= maxd; d++)
                    for (size_type cur = m_level_start[d - 1], end = (m_level_start[d] = cursor); cur != end;) {
                        size_type x = p[cur++];
                        m_rooted_tree->do_for_each_adj_vertex(x, [&](size_type to) {
                            if (m_info[to].m_dep > m_info[x].m_dep) push(to);
                        });
                    }
                m_level_start[maxd + 1] = cursor;
            }
            size_type get_depth(size_type a) const { return m_info[a].m_dep; }
            size_type get_ancestor(size_type a, size_type n) const {
                size_type d = m_info[a].m_dep;
                if (n > d) return -1;
                const size_type *p = m_bfs_dfn_seq.data(), *first = p + m_level_start[d - n], *last = p + m_level_start[d - n + 1];
                return m_seq[std::lower_bound(first, last, m_info[a].m_dfn) - p];
            }
            size_type calc(size_type a, size_type b) const {
                size_type a_dfn = m_info[a].m_dfn, b_dfn = m_info[b].m_dfn;
                if (a_dfn > b_dfn) std::swap(a_dfn, b_dfn);
                const size_type *p = m_bfs_dfn_seq.data();
                size_type low = 0, high = std::min(m_info[a].m_dep, m_info[b].m_dep);
                while (low != high) {
                    size_type mid = (low + high + 1) / 2;
                    *std::lower_bound(p + m_level_start[mid], p + m_level_start[mid + 1], a_dfn) >= b_dfn ? (low = mid) : (high = mid - 1);
                }
                return m_seq[std::lower_bound(p + m_level_start[low], p + m_level_start[low + 1], a_dfn) - p];
            }
            template <typename BfnCallback>
            auto do_for_descendants(size_type a, size_type n, BfnCallback &&call) const -> decltype(call(0, 0)) {
                size_type d = m_info[a].m_dep;
                if (d + n + 1 >= m_level_start.size()) return call(m_seq.size(), m_seq.size() - 1);
                const size_type *p = m_bfs_dfn_seq.data(), *first = p + m_level_start[d + n], *last = p + m_level_start[d + n + 1];
                size_type l = std::lower_bound(first, last, m_info[a].m_bfn == m_level_start[d] ? 0 : m_bfs_dfn_seq[m_info[a].m_bfn - 1] + 1) - p, r = std::lower_bound(first, last, m_info[a].m_dfn + 1) - p;
                return call(l, r - 1);
            }
            template <typename BfnCallback>
            auto do_for_vertex(size_type a, BfnCallback &&call) const -> decltype(call(0)) { return call(m_info[a].m_bfn); }
        };
    }
}

#endif