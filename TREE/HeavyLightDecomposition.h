/*
最后修改:
20240429
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HEAVYLIGHTDECOMPOSITION__
#define __OY_HEAVYLIGHTDECOMPOSITION__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace HLD {
        using size_type = uint32_t;
        template <typename Tree>
        struct Table {
            struct node {
                size_type m_top_dfn, m_top_dep, m_parent, m_dfn, m_dep, m_size, m_heavy;
            };
            Tree *m_rooted_tree;
            std::vector<node> m_info;
            std::vector<size_type> m_seq;
            void _tree_dfs1(size_type a, size_type p) {
                m_info[a].m_size = 1, m_info[a].m_heavy = -1;
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) {
                    if (to != p) {
                        _tree_dfs1(to, a);
                        size_type to_size = m_info[to].m_size;
                        m_info[a].m_size += to_size;
                        if (!~m_info[a].m_heavy || to_size > m_info[m_info[a].m_heavy].m_size) m_info[a].m_heavy = to;
                    }
                });
            }
            void _tree_dfs2(size_type a, size_type p, size_type &cursor) {
                m_seq[cursor] = a;
                if (~p) m_info[a].m_dep = m_info[p].m_dep + 1;
                m_info[a].m_dfn = cursor++;
                bool is_top = !~p || a != m_info[p].m_heavy;
                m_info[a].m_top_dfn = is_top ? m_info[a].m_dfn : m_info[p].m_top_dfn;
                m_info[a].m_top_dep = is_top ? m_info[a].m_dep : m_info[p].m_top_dep;
                m_info[a].m_parent = is_top ? p : m_info[p].m_parent;
                size_type heavy = m_info[a].m_heavy;
                if (~heavy) _tree_dfs2(heavy, a, cursor);
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) { if (to != p && to != heavy) _tree_dfs2(to, a, cursor); });
            }
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_info.resize(m_rooted_tree->vertex_cnt()), m_seq.resize(m_rooted_tree->vertex_cnt());
                _tree_dfs1(m_rooted_tree->m_root, -1);
                size_type cursor = 0;
                _tree_dfs2(m_rooted_tree->m_root, -1, cursor);
            }
            size_type get_ancestor(size_type a, size_type n) const {
                auto info = m_info.data();
                if (n > info[a].m_dep) return -1;
                size_type dep = info[a].m_dep, target_dep = dep - n;
                while (target_dep < info[a].m_top_dep) dep = info[a].m_top_dep - 1, a = info[a].m_parent;
                return m_seq[info[a].m_dfn - dep + target_dep];
            }
            size_type find_parent(size_type a) const { return m_info[a].m_top_dep == m_info[a].m_dep ? m_info[a].m_parent : (m_info[a].m_dfn ? m_seq[m_info[a].m_dfn - 1] : -1); }
            size_type find_son(size_type a, size_type b) const { return get_ancestor(b, m_info[b].m_dep - m_info[a].m_dep - 1); }
            size_type get_depth(size_type a) const { return m_info[a].m_dep; }
            template <typename Callback>
            auto do_for_vertex(size_type a, Callback &&call) const -> decltype(call(0)) { return call(m_info[a].m_dfn); }
            template <bool LCA, typename Callback>
            void do_for_path(size_type a, size_type b, Callback &&call) const {
                auto info = m_info.data();
                while (info[a].m_top_dfn != info[b].m_top_dfn)
                    if (info[a].m_top_dep < info[b].m_top_dep)
                        call(info[b].m_top_dfn, info[b].m_dfn), b = info[b].m_parent;
                    else
                        call(info[a].m_top_dfn, info[a].m_dfn), a = info[a].m_parent;
                if (info[a].m_dep > info[b].m_dep) std::swap(a, b);
                if constexpr (LCA)
                    call(info[a].m_dfn, info[b].m_dfn);
                else if (a != b)
                    call(info[a].m_dfn + 1, info[b].m_dfn);
            }
            template <typename Callback>
            void do_for_directed_path(size_type from, size_type to, Callback &&call) const {
                auto info = m_info.data();
                while (info[from].m_top_dfn != info[to].m_top_dfn)
                    if (info[from].m_top_dep < info[to].m_top_dep) {
                        do_for_directed_path(from, info[to].m_parent, call);
                        call(info[to].m_top_dfn, info[to].m_dfn);
                        return;
                    } else
                        call(info[from].m_dfn, info[from].m_top_dfn), from = info[from].m_parent;
                call(info[from].m_dfn, info[to].m_dfn);
            }
            template <typename Callback>
            auto do_for_subtree(size_type a, Callback &&call) const -> decltype(call(0, 0)) { return call(m_info[a].m_dfn, m_info[a].m_dfn + m_info[a].m_size - 1); }
            size_type calc(size_type a, size_type b) const {
                auto info = m_info.data();
                while (info[a].m_top_dfn != info[b].m_top_dfn)
                    if (info[a].m_top_dep < info[b].m_top_dep)
                        b = info[b].m_parent;
                    else
                        a = info[a].m_parent;
                return info[a].m_dep < info[b].m_dep ? a : b;
            }
        };
    }
}

#endif