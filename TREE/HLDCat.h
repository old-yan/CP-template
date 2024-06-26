/*
最后修改:
20240429
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HLDCAT__
#define __OY_HLDCAT__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../DS/CatTree.h"
#include "HeavyLightDecomposition.h"

namespace OY {
    namespace HLDCAT {
        using size_type = uint32_t;
        template <typename Tree, typename Node>
        struct TreeCat {
            using table_type = CAT::Table<Node>;
            using value_type = typename table_type::value_type;
            using node = typename table_type::node;
            Tree *m_rooted_tree;
            HLD::Table<Tree> m_hld;
            table_type m_cat;
            template <typename InitMapping = CAT::Ignore>
            TreeCat(Tree *rooted_tree = nullptr, InitMapping mapping = InitMapping()) { reset(rooted_tree, mapping); }
            template <typename InitMapping = CAT::Ignore>
            void reset(Tree *rooted_tree, InitMapping mapping = InitMapping()) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_hld.reset(rooted_tree);
                if constexpr (!std::is_same<InitMapping, CAT::Ignore>::value)
                    m_cat.resize(m_rooted_tree->vertex_cnt(), [&](size_type i) { return mapping(m_hld.m_seq[i]); });
                else
                    m_cat.resize(m_rooted_tree->vertex_cnt());
            }
            void add(size_type i, const value_type &inc) {
                m_hld.do_for_vertex(i, [&](size_type pos) { m_cat.add(pos, inc); });
            }
            template <bool LCA>
            void add_path(size_type a, size_type b, const value_type &inc) {
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) {for (size_type i = l; i <= r; i++)m_cat.add(i, inc); });
            }
            void add_subtree(size_type root, const value_type &inc) {
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) {for (size_type i = l; i <= r; i++)m_cat.add(i, inc); });
            }
            value_type query(size_type i) const {
                return m_hld.do_for_vertex(i, [&](size_type pos) { return m_cat.query(pos); });
            }
            template <bool LCA>
            value_type query_path(size_type a, size_type b) const {
                value_type res;
                bool first = true;
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) {
                    if (first)
                        res = m_cat.query(l, r), first = false;
                    else
                        res = node::op(res, m_cat.query(l, r));
                });
                return res;
            }
            value_type query_subtree(size_type root) const {
                return m_hld.do_for_subtree(root, [&](size_type l, size_type r) { return m_cat.query(l, r); });
            }
            value_type query_all() const { return m_cat.query_all(); }
        };
        template <typename Ostream, typename Tree, typename Node>
        Ostream &operator<<(Ostream &out, const TreeCat<Tree, Node> &x) { // http://mshang.ca/syntree/
            x.m_rooted_tree->tree_dp_vertex(
                ~x.m_rooted_tree->m_root ? x.m_rooted_tree->m_root : 0, [&](size_type a, size_type) { out << '[' << x.query(a); }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
    template <typename Tp, typename Tree, typename Operation, typename InitMapping = CAT::Ignore, typename TableType = HLDCAT::TreeCat<Tree, CAT::CustomNode<Tp, Operation>>>
    auto make_HLDCat(Tree *rooted_tree, Operation op, InitMapping mapping = InitMapping()) -> TableType { return TableType(rooted_tree, mapping); }
    template <typename Tp, typename Tree, typename InitMapping = CAT::Ignore, typename TableType = HLDCAT::TreeCat<Tree, CAT::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>>>
    auto make_HLDCat(Tree *rooted_tree, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TableType { return TableType::node::s_op = op, TableType(rooted_tree, mapping); }
    template <typename Tp, typename Tree, typename InitMapping = CAT::Ignore, typename TableType = HLDCAT::TreeCat<Tree, CAT::CustomNode<Tp, Tp (*)(Tp, Tp)>>>
    auto make_HLDCat(Tree *rooted_tree, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TableType { return TableType::node::s_op = op, TableType(rooted_tree, mapping); }
    template <typename Tree, typename Tp>
    using HLDCatMaxTable = HLDCAT::TreeCat<Tree, CAT::BaseNode<Tp, std::less<Tp>>>;
    template <typename Tree, typename Tp>
    using HLDCatMinTable = HLDCAT::TreeCat<Tree, CAT::BaseNode<Tp, std::greater<Tp>>>;
    template <typename Tree, typename Tp>
    using HLDCatSumTable = HLDCAT::TreeCat<Tree, CAT::CustomNode<Tp, std::plus<Tp>>>;
}

#endif