/*
最后修改:
20231026
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STOERWAGNER__
#define __OY_STOERWAGNER__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace StoerWagner {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_NODE>
        struct Graph {
            static mask_type s_buffer[MAX_NODE];
            static Tp s_cost_buffer[MAX_NODE];
            static size_type s_use_count, s_cost_use_count;
            Tp *m_cost;
            mask_type *m_behalf, *m_chosen;
            size_type m_vertex_cnt, m_width;
            static void _set_true(mask_type *b, size_type i) { b[i >> 6] |= mask_type(1) << (i & 63); }
            static void _set_false(mask_type *b, size_type i) { b[i >> 6] &= -(mask_type(1) << (i & 63)) - 1; }
            static size_type _find_first(mask_type *b) {
                size_type i = 0;
                while (!b[i]) i++;
                return (i << 6) | std::countr_zero(b[i]);
            }
            static size_type _find_next(mask_type *b, size_type j) {
                size_type i = j >> 6;
                mask_type x = b[i] & -(mask_type(1) << (j & 63));
                if (x) return (i << 6) | std::countr_zero(x);
                while (!b[++i]) {}
                return (i << 6) | std::countr_zero(b[i]);
            }
            void _bit_or(mask_type *a, const mask_type *b) {
                for (size_type i = 0; i < m_vertex_cnt; i += 64) a[i] |= b[i];
            }
            Graph(size_type vertex_cnt) { m_vertex_cnt = vertex_cnt, m_width = (m_vertex_cnt >> 6) + 1, m_behalf = s_buffer + s_use_count, m_cost = s_cost_buffer + s_cost_use_count, s_use_count += m_width * m_vertex_cnt, s_cost_use_count += m_vertex_cnt * m_vertex_cnt; }
            void add_edge(size_type a, size_type b, const Tp &cost) { m_cost[m_vertex_cnt * a + b] += cost, m_cost[m_vertex_cnt * b + a] += cost; }
            template <bool GetPath>
            Tp calc(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                if constexpr (GetPath)for (size_type i = 0; i != m_vertex_cnt; i++) _set_true(m_behalf + m_width * i, i);
                std::vector<mask_type> can_choose(m_width);
                std::vector<Tp> w(m_vertex_cnt);
                Tp ans = infinite;
                size_type can_choose_cnt = m_vertex_cnt;
                _set_true(can_choose.data(), m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++) _set_true(can_choose.data(), i);
                while (can_choose_cnt > 1) {
                    std::vector<mask_type> can_visit(can_choose);
                    size_type source = -1, target = _find_first(can_visit.data()), can_visit_count = can_choose_cnt;
                    std::fill_n(w.begin(), m_vertex_cnt, 0);
                    while (can_visit_count > 1) {
                        _set_false(can_visit.data(), target), can_visit_count--, source = target;
                        Tp max_cost = 0;
                        for (size_type to = _find_first(can_visit.data()); to != m_vertex_cnt; to = _find_next(can_visit.data(), to + 1)) {
                            w[to] += m_cost[m_vertex_cnt * source + to];
                            if (w[to] > max_cost) max_cost = w[to], target = to;
                        }
                    }
                    if (w[target] < ans) {
                        ans = w[target];
                        if constexpr (GetPath) m_chosen = m_behalf + m_width * target;
                    }
                    _set_false(can_choose.data(), target), can_choose_cnt--;
                    for (size_type to = _find_first(can_choose.data()); to != m_vertex_cnt; to = _find_next(can_choose.data(), to + 1))
                        if (m_cost[m_vertex_cnt * target + to]) add_edge(source, to, m_cost[m_vertex_cnt * target + to]);
                    if constexpr (GetPath) _bit_or(m_behalf + m_width * source, m_behalf + m_width * target);
                }
                return ans;
            }
            bool is_chosen(size_type i) const { return m_chosen[i >> 6] >> (i & 63) & 1; }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_NODE>
        mask_type Graph<Tp, MAX_VERTEX, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_NODE>
        Tp Graph<Tp, MAX_VERTEX, MAX_NODE>::s_cost_buffer[MAX_NODE];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_NODE>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_NODE>::s_cost_use_count;
    }
}

#endif