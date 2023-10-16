/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HLDBIT__
#define __OY_HLDBIT__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../DS/BIT.h"
#include "HeavyLightDecomposition.h"

namespace OY {
    namespace HLDBIT {
        using size_type = uint32_t;
        template <typename Tree, typename Tp, size_type MAX_VERTEX = 1 << 20>
        struct TreeBIT {
            using table_type = BIT::Tree<Tp, true, MAX_VERTEX << 1>;
            Tree *m_rooted_tree;
            HLD::Table<Tree, MAX_VERTEX> m_hld;
            table_type m_bit;
            template <typename InitMapping = BIT::Ignore>
            TreeBIT(Tree *rooted_tree = nullptr, InitMapping mapping = InitMapping()) { reset(rooted_tree, mapping); }
            template <typename InitMapping = BIT::Ignore>
            void reset(Tree *rooted_tree, InitMapping mapping = InitMapping()) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_hld.reset(rooted_tree);
                if constexpr (!std::is_same<InitMapping, BIT::Ignore>::value)
                    m_bit.resize(m_rooted_tree->vertex_cnt(), [&](size_type i) { return mapping(m_hld.m_seq[i]); });
                else
                    m_bit.resize(m_rooted_tree->vertex_cnt());
            }
            void add(size_type i, const Tp &inc) { m_bit.add(m_hld.m_info[i].m_dfn, inc); }
            template <bool LCA>
            void add_path(size_type a, size_type b, const Tp &inc) {
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) { m_bit.add(l, r, inc); });
            }
            void add_subtree(size_type root, const Tp &inc) {
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) { m_bit.add(l, r, inc); });
            }
            Tp query(size_type i) const { return m_bit.query(m_hld.m_info[i].m_dfn); }
            template <bool LCA>
            Tp query_path(size_type a, size_type b) const {
                Tp res = 0;
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) { res += m_bit.query(l, r); });
                return res;
            }
            Tp query_subtree(size_type root) const {
                Tp res = 0;
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) { res += m_bit.query(l, r); });
                return res;
            }
            Tp query_all() const { return m_bit.query_all(); }
        };
        template <typename Ostream, typename Tree, typename Tp, size_type MAX_VER>
        Ostream &operator<<(Ostream &out, const TreeBIT<Tree, Tp, MAX_VER> &x) { // http://mshang.ca/syntree/
            x.m_rooted_tree->tree_dp_vertex(
                ~x.m_rooted_tree->m_root ? x.m_rooted_tree->m_root : 0, [&](size_type a, size_type) { out << '[' << x.query(a); }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
    template <typename Tree, HLDBIT::size_type MAX_VERTEX = 1 << 20>
    using HLDBIT32 = HLDBIT::TreeBIT<Tree, int32_t, MAX_VERTEX>;
    template <typename Tree, HLDBIT::size_type MAX_VERTEX = 1 << 20>
    using HLDBIT64 = HLDBIT::TreeBIT<Tree, int64_t, MAX_VERTEX>;
}

#endif