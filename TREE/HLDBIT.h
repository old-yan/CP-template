/*
最后修改:
20240613
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

#include "../DS/BIT_ex.h"
#include "HeavyLightDecomposition.h"

namespace OY {
    namespace HLDBIT {
        using size_type = uint32_t;
        template <typename Tree, typename Tp, template <typename> typename BufferType = BITEX::VectorBuffer>
        struct TreeBIT {
            using table_type = BITEX::Tree<Tp, BufferType>;
            Tree *m_rooted_tree;
            HLD::Table<Tree> m_hld;
            table_type m_bit;
            TreeBIT(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            template <typename InitMapping>
            TreeBIT(Tree *rooted_tree, InitMapping mapping) { reset(rooted_tree, mapping); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_hld.reset(rooted_tree);
                m_bit.resize(m_rooted_tree->vertex_cnt());
            }
            template <typename InitMapping>
            void reset(Tree *rooted_tree, InitMapping mapping) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_hld.reset(rooted_tree);
                m_bit.resize(m_rooted_tree->vertex_cnt(), [&](size_type i) { return mapping(m_hld.m_seq[i]); });
            }
            void add(size_type i, Tp inc) {
                m_hld.do_for_vertex(i, [&](size_type pos) { m_bit.add(pos, inc); });
            }
            template <bool LCA>
            void add_path(size_type a, size_type b, Tp inc) {
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) { m_bit.add(l, r, inc); });
            }
            void add_subtree(size_type root, Tp inc) {
                m_hld.do_for_subtree(root, [&](size_type l, size_type r) { m_bit.add(l, r, inc); });
            }
            Tp query(size_type i) const {
                return m_hld.do_for_vertex(i, [&](size_type pos) { return m_bit.query(pos); });
            }
            template <bool LCA>
            Tp query_path(size_type a, size_type b) const {
                Tp res = 0;
                m_hld.template do_for_path<LCA>(a, b, [&](size_type l, size_type r) { res += m_bit.query(l, r); });
                return res;
            }
            Tp query_subtree(size_type root) const {
                return m_hld.do_for_subtree(root, [&](size_type l, size_type r) { return m_bit.query(l, r); });
            }
            Tp query_all() const { return m_bit.query_all(); }
        };
        template <typename Ostream, typename Tree, typename Tp, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const TreeBIT<Tree, Tp, BufferType> &x) { // http://mshang.ca/syntree/
            x.m_rooted_tree->tree_dp_vertex(
                ~x.m_rooted_tree->m_root ? x.m_rooted_tree->m_root : 0, [&](size_type a, size_type) { out << '[' << x.query(a); }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
    template <typename Tree, template <typename> typename BufferType = BITEX::VectorBuffer>
    using HLDBIT32 = HLDBIT::TreeBIT<Tree, int32_t, BufferType>;
    template <typename Tree, template <typename> typename BufferType = BITEX::VectorBuffer>
    using HLDBIT64 = HLDBIT::TreeBIT<Tree, int64_t, BufferType>;
}

#endif