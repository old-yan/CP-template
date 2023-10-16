/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HLDMASKRMQ__
#define __OY_HLDMASKRMQ__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../DS/MaskRMQ.h"
#include "HeavyLightDecomposition.h"

namespace OY {
    namespace HLDMaskRMQ {
        using size_type = uint32_t;
        template <typename Tree, typename Node, typename MaskType, size_type MAX_VERTEX>
        struct IndexTable {
            using table_type = MaskRMQ::IndexTable<Node, MaskType, MAX_VERTEX>;
            using value_type = typename table_type::value_type;
            using node = typename table_type::node;
            Tree *m_rooted_tree;
            HLD::Table<Tree, MAX_VERTEX> m_hld;
            table_type m_rmq;
            template <typename InitMapping = MaskRMQ::Ignore>
            IndexTable(Tree *rooted_tree = nullptr, InitMapping mapping = InitMapping()) { reset(rooted_tree, mapping); }
            template <typename InitMapping = MaskRMQ::Ignore>
            void reset(Tree *rooted_tree, InitMapping mapping = InitMapping()) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_hld.reset(rooted_tree);
                if constexpr (!std::is_same<InitMapping, MaskRMQ::Ignore>::value)
                    m_rmq.resize(m_rooted_tree->vertex_cnt(), [&](size_type i) { return mapping(m_hld.m_seq[i]); });
                else
                    m_rmq.resize(m_rooted_tree->vertex_cnt());
            }
            void modify(size_type i, const value_type &inc) { m_rmq.add(m_hld.m_info[i].m_dfn, inc); }
            size_type query(size_type i) const { return i; }
            template <bool LCA>
            size_type query_path(size_type a, size_type b) const {
                size_type res;
                bool first = true;
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) {
                    if (first)
                        res = m_rmq.query(l, r), first = false;
                    else
                        res = m_rmq._choose(res, m_rmq.query(l, r));
                });
                return m_hld.m_seq[res];
            }
            size_type query_subtree(size_type root) const {
                size_type res;
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) { res = m_rmq.query(l, r); });
                return m_hld.m_seq[res];
            }
            size_type query_all() const { return m_hld.m_seq[m_rmq.query_all()]; }
            const value_type &get(size_type i) const { return m_rmq.get(m_hld.m_info[i].m_dfn); }
        };
        template <typename Tree, typename Node, typename MaskType, size_type MAX_VERTEX>
        struct ValueTable {
            using table_type = MaskRMQ::ValueTable<Node, MaskType, MAX_VERTEX>;
            using value_type = typename table_type::value_type;
            using node = typename table_type::node;
            Tree *m_rooted_tree;
            HLD::Table<Tree, MAX_VERTEX> m_hld;
            table_type m_rmq;
            template <typename InitMapping = MaskRMQ::Ignore>
            ValueTable(Tree *rooted_tree = nullptr, InitMapping mapping = InitMapping()) { reset(rooted_tree, mapping); }
            template <typename InitMapping = MaskRMQ::Ignore>
            void reset(Tree *rooted_tree, InitMapping mapping = InitMapping()) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_hld.reset(rooted_tree);
                m_rmq.resize(m_rooted_tree->vertex_cnt(), [&](size_type i) { return mapping(m_hld.m_seq[i]); });
            }
            void modify(size_type i, const value_type &inc) { m_rmq.add(m_hld.m_info[i].m_dfn, inc); }
            value_type query(size_type i) const { return m_rmq.query(m_hld.m_info[i].m_dfn); }
            template <bool LCA>
            value_type query_path(size_type a, size_type b) const {
                value_type res;
                bool first = true;
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) {
                    if (first)
                        res = m_rmq.query(l, r), first = false;
                    else
                        res = m_rmq._choose(res, m_rmq.query(l, r));
                });
                return res;
            }
            value_type query_subtree(size_type root) const {
                value_type res;
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) { res = m_rmq.query(l, r); });
                return res;
            }
            value_type query_all() const { return m_rmq.query_all(); }
        };
        template <typename Ostream, typename Tree, typename Node, typename MaskType, size_type MAX_VERTEX>
        Ostream &operator<<(Ostream &out, const IndexTable<Tree, Node, MaskType, MAX_VERTEX> &x) { // http://mshang.ca/syntree/
            x.m_rooted_tree->tree_dp_vertex(
                ~x.m_rooted_tree->m_root ? x.m_rooted_tree->m_root : 0, [&](size_type a, size_type) { out << '[' << x.get(x.query(a)); }, {}, [&](size_type) { out << ']'; });
            return out;
        }
        template <typename Ostream, typename Tree, typename Node, typename MaskType, size_type MAX_VERTEX>
        Ostream &operator<<(Ostream &out, const ValueTable<Tree, Node, MaskType, MAX_VERTEX> &x) { // http://mshang.ca/syntree/
            x.m_rooted_tree->tree_dp_vertex(
                ~x.m_rooted_tree->m_root ? x.m_rooted_tree->m_root : 0, [&](size_type a, size_type) { out << '[' << x.query(a); }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
    template <typename Tp, typename MaskType, HLDMaskRMQ::size_type MAX_VERTEX, typename Tree, typename Compare = std::less<Tp>, typename InitMapping = MaskRMQ::Ignore, typename TableType = HLDMaskRMQ::IndexTable<Tree, MaskRMQ::BaseNode<Tp, Compare>, MaskType, MAX_VERTEX>>
    auto make_HLDMaskRMQ_IndexTable(Tree *rooted_tree, Compare comp = Compare(), InitMapping mapping = InitMapping()) -> TableType { return TableType(rooted_tree, mapping); }
    template <typename Tp, typename MaskType, HLDMaskRMQ::size_type MAX_VERTEX, typename Tree, typename Compare = std::less<Tp>, typename InitMapping = MaskRMQ::Ignore, typename TableType = HLDMaskRMQ::ValueTable<Tree, MaskRMQ::BaseNode<Tp, Compare>, MaskType, MAX_VERTEX>>
    auto make_HLDMaskRMQ_ValueTable(Tree *rooted_tree, Compare comp = Compare(), InitMapping mapping = InitMapping()) -> TableType { return TableType(rooted_tree, mapping); }
    template <typename Tree, typename Tp, typename MaskType, HLDMaskRMQ::size_type MAX_VERTEX>
    using HLDMaskRMQMaxTable = HLDMaskRMQ::ValueTable<Tree, MaskRMQ::BaseNode<Tp, std::less<Tp>>, MaskType, MAX_VERTEX>;
    template <typename Tree, typename Tp, typename MaskType, HLDMaskRMQ::size_type MAX_VERTEX>
    using HLDMaskRMQMinTable = HLDMaskRMQ::ValueTable<Tree, MaskRMQ::BaseNode<Tp, std::greater<Tp>>, MaskType, MAX_VERTEX>;
}

#endif