/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CUSTOMTREE__
#define __OY_CUSTOMTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace CustomTree {
        using size_type = uint32_t;
        struct Ignore {
            template <typename... Args>
            void operator()(Args... args) const {}
        };
        template <typename Tp, typename AdjCallback>
        struct Tree {
            size_type m_vertex_cnt, m_root;
            AdjCallback m_adj_call;
            template <typename PreWork, typename Report, typename AfterWork>
            void _tree_dp_vertex(size_type a, size_type p, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const {
                pre_work(a, p);
                do_for_each_adj_vertex(a, [&](size_type to) {if (to != p) _tree_dp_vertex(to, a, pre_work, report, after_work), report(a, to); });
                after_work(a);
            }
            template <typename PreWork, typename Report, typename AfterWork>
            void _tree_dp_edge(size_type a, size_type p, Tp up_dis, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const {
                pre_work(a, p, up_dis);
                do_for_each_adj_edge(a, [&](size_type to, Tp dis) { if (to != p) _tree_dp_edge(to, a, dis, pre_work, report, after_work), report(a, to, dis); });
                after_work(a, up_dis);
            }
            template <typename Callback = AdjCallback>
            Tree(size_type vertex_cnt, Callback &&adj_call) : m_vertex_cnt(vertex_cnt), m_root(-1), m_adj_call(adj_call) {}
            size_type vertex_cnt() const { return m_vertex_cnt; }
            void set_root(size_type root) { m_root = root; }
            template <typename Callback>
            void do_for_each_adj_vertex(size_type a, Callback &&call) const {
                m_adj_call(a, [&](size_type to, Tp dis) { call(to); });
            }
            template <typename Callback>
            void do_for_each_adj_edge(size_type a, Callback &&call) const {
                m_adj_call(a, [&](size_type to, Tp dis) { call(to, dis); });
            }
            template <typename PreWork = Ignore, typename Report = Ignore, typename AfterWork = Ignore>
            void tree_dp_vertex(size_type a, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const { _tree_dp_vertex(a, -1, pre_work, report, after_work); }
            template <typename PreWork = Ignore, typename Report = Ignore, typename AfterWork = Ignore>
            void tree_dp_edge(size_type a, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const { _tree_dp_edge(a, -1, pre_work, report, after_work); }
        };
        template <typename Ostream, typename Tp, typename AdjCallback>
        Ostream &operator<<(Ostream &out, const Tree<Tp, AdjCallback> &tree) { // http://mshang.ca/syntree/
            tree.tree_dp_vertex(
                ~tree.m_root ? tree.m_root : 0, [&](size_type a, size_type) { out << '[' << a; }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
}

#endif