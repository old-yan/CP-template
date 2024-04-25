/*
最后修改:
20231203
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HAMELXORBASETREE__
#define __OY_HAMELXORBASETREE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"
#include "HamelXorBase.h"

namespace OY {
    namespace HAMELTREE {
        using size_type = uint32_t;
        template <typename Tp, size_type MAX_WIDTH>
        struct HamelTreeNodes {
            Tp m_val[MAX_WIDTH], m_mask;
            size_type m_pos[MAX_WIDTH];
            size_type count(size_type left) const {
                return std::count_if(m_pos, m_pos + MAX_WIDTH, [&](size_type pos) { return pos > left; });
            }
        };
        template <typename Tp>
        struct HamelTreeNodes<Tp, 0> {
            static size_type s_width;
            std::vector<Tp> m_val = std::vector<Tp>(s_width);
            std::vector<size_type> m_pos = std::vector<size_type>(s_width);
            Tp m_mask{};
            size_type count(size_type left) const {
                return std::count_if(m_val.begin(), m_val.end(), [&](size_type pos) { return pos > left; });
            }
        };
        template <typename Tp>
        size_type HamelTreeNodes<Tp, 0>::s_width = sizeof(Tp) << 3;
        template <typename Tp, size_type MAX_WIDTH>
        struct HamelXorBaseTree {
            using base_type = HAMEL::HamelXorBase<Tp, MAX_WIDTH>;
            std::vector<HamelTreeNodes<Tp, MAX_WIDTH>> m_masks;
            std::vector<size_type> m_depth;
            static void set_width(size_type w) {
                static_assert(!MAX_WIDTH, "MAX_WIDTH Must Be 0");
                HamelTreeNodes<Tp, MAX_WIDTH>::s_width = w;
                base_type::set_width(w);
            }
            static size_type width() {
                if constexpr (MAX_WIDTH)
                    return MAX_WIDTH;
                else
                    return HamelTreeNodes<Tp, MAX_WIDTH>::s_width;
            }
            template <typename Tree, typename InitMapping>
            void _dfs(size_type a, size_type p, size_type dep, Tree *rooted_tree, InitMapping &&mapping) {
                m_masks[a] = m_masks[p], m_depth[a - 1] = dep++;
                Tp mask = mapping(a - 1);
                size_type pos = dep;
                for (size_type j = width() - 1; ~j; j--)
                    if (mask >> j & 1) {
                        if (!m_masks[a].m_pos[j]) {
                            m_masks[a].m_val[j] = mask, m_masks[a].m_pos[j] = pos, m_masks[a].m_mask |= Tp(1) << j;
                            break;
                        }
                        if (m_masks[a].m_pos[j] <= pos) std::swap(mask, m_masks[a].m_val[j]), std::swap(pos, m_masks[a].m_pos[j]);
                        mask ^= m_masks[a].m_val[j];
                    }
                rooted_tree->do_for_each_adj_vertex(a - 1, [&](size_type to) { if (to != p - 1) _dfs(to + 1, a, dep, rooted_tree, mapping); });
            }
            HamelXorBaseTree() = default;
            template <typename Tree, typename InitMapping>
            HamelXorBaseTree(Tree *rooted_tree, InitMapping mapping) { reset(rooted_tree, mapping); }
            template <typename Tree, typename InitMapping>
            void reset(Tree *rooted_tree, InitMapping mapping) {
                m_masks.resize(rooted_tree->vertex_cnt() + 1);
                m_depth.resize(rooted_tree->vertex_cnt());
                for (size_type j = 0; j != width(); j++) m_masks[0].m_val[j] = m_masks[0].m_pos[j] = 0;
                _dfs(rooted_tree->m_root + 1, 0, 0, rooted_tree, mapping);
            }
            template <typename Callback>
            void enumerate_base(size_type up, size_type down, Callback &&call) const {
                auto &node = m_masks[down + 1];
                size_type d = m_depth[up];
                for (Tp mask = node.m_mask; mask;) {
                    size_type i = std::countr_zero(mask);
                    mask -= Tp(1) << i;
                    if (node.m_pos[i] > d) call(node.m_val[i]);
                }
            }
            base_type to_base_type(size_type up, size_type down) const {
                base_type res{};
                enumerate_base(up, down, [&](Tp mask) { res.insert(mask); });
                return res;
            }
            base_type to_base_type(size_type a, size_type b, size_type lca) const {
                if (m_depth[a] > m_depth[b]) std::swap(a, b);
                base_type res{};
                enumerate_base(lca, b, [&](Tp mask) { res.insert(mask); });
                if (a != lca) enumerate_base(lca, a, [&](Tp mask) { res.insert(mask); });
                return res;
            }
            Tp query_max_bitxor(size_type up, size_type down, Tp base = 0) const {
                Tp ans = base;
                auto &node = m_masks[down + 1];
                size_type d = m_depth[up];
                for (size_type i = width() - 1; ~i; i--)
                    if (node.m_pos[i] > d && (ans ^ node.m_val[i]) > ans) ans ^= node.m_val[i];
                return ans;
            }
        };
    }
    template <HAMELTREE::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBaseTree32 = HAMELTREE::HamelXorBaseTree<uint32_t, MAX_WIDTH>;
    template <HAMELTREE::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBaseTree64 = HAMELTREE::HamelXorBaseTree<uint64_t, MAX_WIDTH>;
    using DynamicHamelXorBaseTree32 = HAMELTREE::HamelXorBaseTree<uint32_t, 0>;
    using DynamicHamelXorBaseTree64 = HAMELTREE::HamelXorBaseTree<uint64_t, 0>;
}

#endif