/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HLDZKW__
#define __OY_HLDZKW__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../DS/ZkwTree.h"
#include "HeavyLightDecomposition.h"

namespace OY {
    namespace HLDZkw {
        using size_type = uint32_t;
        template <typename Tree, typename Node, size_type MAX_VERTEX = 1 << 20>
        struct Table {
            using table_type = Zkw::Tree<Node, MAX_VERTEX << 2>;
            using value_type = typename table_type::value_type;
            using modify_type = typename table_type::modify_type;
            using node = typename table_type::node;
            Tree *m_rooted_tree;
            HLD::Table<Tree, MAX_VERTEX> m_hld;
            table_type m_zkw;
            template <typename InitMapping = Zkw::Ignore>
            Table(Tree *rooted_tree = nullptr, InitMapping mapping = InitMapping()) { reset(rooted_tree, mapping); }
            template <typename InitMapping = Zkw::Ignore>
            void reset(Tree *rooted_tree, InitMapping mapping = InitMapping()) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_hld.reset(rooted_tree);
                if constexpr (!std::is_same<InitMapping, Zkw::Ignore>::value)
                    m_zkw.resize(m_rooted_tree->vertex_cnt(), [&](size_type i) { return mapping(m_hld.m_seq[i]); });
                else
                    m_zkw.resize(m_rooted_tree->vertex_cnt());
            }
            void modify(size_type i, const value_type &val) { m_zkw.modify(m_hld.m_info[i].m_dfn, val); }
            void add(size_type i, const modify_type &modify) { m_zkw.add(m_hld.m_info[i].m_dfn, modify); }
            template <bool LCA>
            void add_path(size_type a, size_type b, const modify_type &modify) {
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) { m_zkw.add(l, r, modify); });
            }
            void add_subtree(size_type root, const modify_type &modify) {
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) { m_zkw.add(l, r, modify); });
            }
            value_type query(size_type i) const { return m_zkw.query(m_hld.m_info[i].m_dfn); }
            template <bool LCA>
            value_type query_path(size_type a, size_type b) const {
                value_type res;
                bool first = true;
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) {
                    if (first)
                        res = m_zkw.query(l, r), first = false;
                    else
                        res = node::op(res, m_zkw.query(l, r));
                });
                return res;
            }
            value_type query_subtree(size_type root) const {
                value_type res;
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) { res = m_zkw.query(l, r); });
                return res;
            }
            value_type query_all() const { return m_zkw.query_all(); }
        };
        template <typename Ostream, typename Tree, typename Node, size_type MAX_VERTEX>
        Ostream &operator<<(Ostream &out, const Table<Tree, Node, MAX_VERTEX> &x) { // http://mshang.ca/syntree/
            x.m_rooted_tree->tree_dp_vertex(
                ~x.m_rooted_tree->m_root ? x.m_rooted_tree->m_root : 0, [&](size_type a, size_type) { out << '[' << x.query(a); }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
    template <typename Tp, HLDZkw::size_type MAX_VERTEX = 1 << 20, typename Tree, typename Operation, typename InitMapping = Zkw::Ignore, typename TableType = HLDZkw::Table<Tree, Zkw::CustomNode<Tp, Operation>, MAX_VERTEX>>
    auto make_HLDZkw(Tree *rooted_tree, Operation op, InitMapping mapping = InitMapping()) -> TableType { return TableType(rooted_tree, mapping); }
    template <typename Tp, HLDZkw::size_type MAX_VERTEX = 1 << 20, typename Tree, typename InitMapping = Zkw::Ignore, typename TableType = HLDZkw::Table<Tree, Zkw::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_VERTEX>>
    auto make_HLDZkw(Tree *rooted_tree, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TableType { return TableType::node::s_op = op, TableType(rooted_tree, mapping); }
    template <typename Tp, HLDZkw::size_type MAX_VERTEX = 1 << 20, typename Tree, typename InitMapping = Zkw::Ignore, typename TableType = HLDZkw::Table<Tree, Zkw::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_VERTEX>>
    auto make_HLDZkw(Tree *rooted_tree, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TableType { return TableType::node::s_op = op, TableType(rooted_tree, mapping); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, HLDZkw::size_type MAX_VERTEX = 1 << 20, typename Tree, typename InitMapping, typename Operation, typename Mapping, typename Composition, typename TableType = HLDZkw::Table<Tree, Zkw::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>, MAX_VERTEX>>
    auto make_lazy_HLDZkw(Tree *rooted_tree, InitMapping mapping, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TableType { return TableType::node::s_default_modify = default_modify, TableType(rooted_tree, mapping); }
    template <typename Tree, HLDZkw::size_type MAX_VERTEX = 1 << 20>
    using HLDZkwLazySumTree = HLDZkw::Table<Tree, Zkw::LazyNode<int64_t, int64_t>, MAX_VERTEX>;
}

#endif