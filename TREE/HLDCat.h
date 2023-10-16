/*
最后修改:
20230922
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

#include "../DS/CatTree.h"
#include "HeavyLightDecomposition.h"

namespace OY {
    namespace HLDCat {
        using size_type = uint32_t;
        template <typename Tree, typename Node, size_type MAX_VERTEX, size_type MAX_NODE>
        struct TreeCat {
            using table_type = Cat::Table<Node, MAX_NODE>;
            using value_type = typename table_type::value_type;
            using node = typename table_type::node;
            Tree *m_rooted_tree;
            HLD::Table<Tree, MAX_VERTEX> m_hld;
            table_type m_cat;
            template <typename InitMapping = Cat::Ignore>
            TreeCat(Tree *rooted_tree = nullptr, InitMapping mapping = InitMapping()) { reset(rooted_tree, mapping); }
            template <typename InitMapping = Cat::Ignore>
            void reset(Tree *rooted_tree, InitMapping mapping = InitMapping()) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_hld.reset(rooted_tree);
                if constexpr (!std::is_same<InitMapping, Cat::Ignore>::value)
                    m_cat.resize(m_rooted_tree->vertex_cnt(), [&](size_type i) { return mapping(m_hld.m_seq[i]); });
                else
                    m_cat.resize(m_rooted_tree->vertex_cnt());
            }
            void add(size_type i, const value_type &inc) { m_cat.add(m_hld.m_info[i].m_dfn, inc); }
            template <bool LCA>
            void add_path(size_type a, size_type b, const value_type &inc) {
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) {for (size_type i = l; i <= r; i++)m_cat.add(i, inc); });
            }
            void add_subtree(size_type root, const value_type &inc) {
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) {for (size_type i = l; i <= r; i++)m_cat.add(i, inc); });
            }
            value_type query(size_type i) const { return m_cat.query(m_hld.m_info[i].m_dfn); }
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
                value_type res;
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) { res = m_cat.query(l, r); });
                return res;
            }
            value_type query_all() const { return m_cat.query_all(); }
        };
        template <typename Ostream, typename Tree, typename Node, size_type MAX_VERTEX, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const TreeCat<Tree, Node, MAX_VERTEX, MAX_NODE> &x) { // http://mshang.ca/syntree/
            x.m_rooted_tree->tree_dp_vertex(
                ~x.m_rooted_tree->m_root ? x.m_rooted_tree->m_root : 0, [&](size_type a, size_type) { out << '[' << x.query(a); }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
    template <typename Tp, HLDCat::size_type MAX_VERTEX, HLDCat::size_type MAX_NODE, typename Tree, typename Operation, typename InitMapping = Cat::Ignore, typename TableType = HLDCat::TreeCat<Tree, Cat::CustomNode<Tp, Operation>, MAX_VERTEX, MAX_NODE>>
    auto make_HLDCat(Tree *rooted_tree, Operation op, InitMapping mapping = InitMapping()) -> TableType { return TableType(rooted_tree, mapping); }
    template <typename Tp, HLDCat::size_type MAX_VERTEX, HLDCat::size_type MAX_NODE, typename Tree, typename InitMapping = Cat::Ignore, typename TableType = HLDCat::TreeCat<Tree, Cat::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_VERTEX, MAX_NODE>>
    auto make_HLDCat(Tree *rooted_tree, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TableType { return TableType::node::s_op = op, TableType(rooted_tree, mapping); }
    template <typename Tp, HLDCat::size_type MAX_VERTEX, HLDCat::size_type MAX_NODE, typename Tree, typename InitMapping = Cat::Ignore, typename TableType = HLDCat::TreeCat<Tree, Cat::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_VERTEX, MAX_NODE>>
    auto make_HLDCat(Tree *rooted_tree, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TableType { return TableType::node::s_op = op, TableType(rooted_tree, mapping); }
    template <typename Tree, typename Tp, HLDCat::size_type MAX_VERTEX, HLDCat::size_type MAX_NODE>
    using HLDCatMaxTable = HLDCat::TreeCat<Tree, Cat::BaseNode<Tp, std::less<Tp>>, MAX_VERTEX, MAX_NODE>;
    template <typename Tree, typename Tp, HLDCat::size_type MAX_VERTEX, HLDCat::size_type MAX_NODE>
    using HLDCatMinTable = HLDCat::TreeCat<Tree, Cat::BaseNode<Tp, std::greater<Tp>>, MAX_VERTEX, MAX_NODE>;
    template <typename Tree, typename Tp, HLDCat::size_type MAX_VERTEX, HLDCat::size_type MAX_NODE>
    using HLDCatSumTable = HLDCat::TreeCat<Tree, Cat::CustomNode<Tp, std::plus<Tp>>, MAX_VERTEX, MAX_NODE>;
}

#endif