/*
最后修改:
20240515
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MULTIDIMSEGTREE__
#define __OY_MULTIDIMSEGTREE__

#include "WaveLet.h"

namespace OY {
    namespace MDSEG {
        using size_type = uint32_t;
        template <typename SizeType, typename ElemType, size_type DIM, bool HasModify>
        struct Point {
            ElemType m_w;
            SizeType m_x[DIM];
            size_type m_index;
            ElemType weight() const { return m_w; }
        };
        template <typename SizeType, size_type DIM>
        struct Point<SizeType, bool, DIM, true> {
            SizeType m_x[DIM];
            size_type m_index;
            static constexpr bool weight() { return 1; }
        };
        template <typename SizeType, typename ElemType, size_type DIM>
        struct Point<SizeType, ElemType, DIM, false> {
            ElemType m_w;
            SizeType m_x[DIM];
            ElemType weight() const { return m_w; }
        };
        template <typename SizeType, size_type DIM>
        struct Point<SizeType, bool, DIM, false> {
            SizeType m_x[DIM];
            static constexpr bool weight() { return 1; }
        };
        template <size_type DIM>
        struct TempAlpha {
            static size_type s_val[DIM];
        };
        template <size_type DIM>
        size_type TempAlpha<DIM>::s_val[DIM];
        template <typename ValueType, typename BaseTable, size_type DIM, size_type BIAS>
        struct Adapter {
            using sub_adapter = Adapter<ValueType, BaseTable, DIM - 1, BIAS + 1>;
            WaveLet::Table<size_type, sub_adapter> m_table;
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_table.resize_mapping_with_index(
                    length, [&](size_type i) -> size_type { return mapping(i).m_x[2 + BIAS]; }, TempAlpha<DIM + BIAS>::s_val[2 + BIAS], [&](size_type, size_type i) { return mapping(i); });
            }
            template <typename Callback, typename... Args, typename = typename std::enable_if<sizeof...(Args) == DIM * 2 - 6>::type>
            void query(size_type left, size_type right, Callback &&call, size_type x2_min, size_type x2_max, Args... args) const {
                auto q = [&](Callback &&call, const sub_adapter &tr, size_type left, size_type right) { tr.query(left, right, call, args...); };
                m_table.do_for_value_range(left, right, x2_min, x2_max, call, q);
            }
            template <typename Callback>
            void do_in_table(size_type pos, Callback &&call) {
                m_table.do_in_table(pos, [&](sub_adapter &tr, size_type pos) { tr.do_in_table(pos, call); });
            }
        };
        template <typename ValueType, typename BaseTable, size_type BIAS>
        struct Adapter<ValueType, BaseTable, 2, BIAS> {
            BaseTable m_table;
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_table.resize(length, [&](size_type i) { return mapping(i).weight(); });
            }
            template <typename Callback>
            void query(size_type left, size_type right, Callback &&call) const { call(m_table.query(left, right)); }
            template <typename Callback>
            void do_in_table(size_type pos, Callback &&call) { call(m_table, pos); }
        };
        template <typename Tp>
        struct AdjTable {
            std::vector<Tp> m_sum;
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_sum.resize(length + 1);
                for (size_type i = 0; i != length; i++) m_sum[i + 1] = m_sum[i] + mapping(i);
            }
            Tp query(size_type left, size_type right) const { return m_sum[right + 1] - m_sum[left]; }
        };
        template <typename Tp>
        struct SimpleBIT {
            std::vector<Tp> m_sum;
            static size_type _lowbit(size_type x) { return x & -x; }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_sum.resize(length);
                for (size_type i = 0; i != length; i++) m_sum[i] = mapping(i);
                for (size_type i = 0; i != length; i++) {
                    size_type j = i + _lowbit(i + 1);
                    if (j < length) m_sum[j] += m_sum[i];
                }
            }
            void add(size_type i, Tp inc) {
                while (i < m_sum.size()) m_sum[i] += inc, i += _lowbit(i + 1);
            }
            Tp presum(size_type i) const {
                Tp res{};
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_sum[j];
                return res;
            }
            Tp query(size_type left, size_type right) const { return presum(right) - presum(left - 1); }
        };
        template <typename SizeType, typename ElemType, typename BaseTable, size_type DIM = 3, bool HasModify = false>
        struct Tree {
            using value_type = typename std::conditional<std::is_same<ElemType, bool>::value, size_type, ElemType>::type;
            using alpha_table = TempAlpha<DIM>;
            using point = Point<SizeType, ElemType, DIM, HasModify>;
            using adapter = Adapter<value_type, BaseTable, DIM, 0>;
            std::vector<point> m_points;
            std::vector<SizeType> m_sorted_xs[DIM];
            std::vector<size_type> m_pos;
            WaveLet::Table<size_type, adapter> m_table;
            template <size_type I>
            void _prepare(size_type n, size_type *ids) {
                m_sorted_xs[I].reserve(n);
                std::sort(ids, ids + n, [&](size_type x, size_type y) { return m_points[x].m_x[I] < m_points[y].m_x[I]; });
                SizeType pre;
                for (size_type i = 0; i != n; i++) {
                    if (!i || m_points[ids[i]].m_x[I] != pre) m_sorted_xs[I].push_back(m_points[ids[i]].m_x[I]);
                    pre = m_points[ids[i]].m_x[I], m_points[ids[i]].m_x[I] = m_sorted_xs[I].size() - 1;
                }
                alpha_table::s_val[I] = std::max<size_type>(1, std::bit_width(m_sorted_xs[I].size()));
                if constexpr (I + 1 != DIM) _prepare<I + 1>(n, ids);
            }
            template <typename Operation, size_type I, typename... Args>
            value_type _query(value_type init_val, size_type l0, size_type r0, size_type l1, size_type r1, SizeType min, SizeType max, Args... args) const {
                if constexpr (I == DIM) {
                    value_type val = init_val;
                    auto call = [&val](value_type v) { val = Operation()(val, v); };
                    auto q = [&](decltype(call) &call2, const adapter &tr, size_type left, size_type right) { tr.query(left, right, call2, min, max, args...); };
                    m_table.do_for_value_range(l0, r0, l1, r1, call, q);
                    return val;
                } else {
                    size_type l = std::lower_bound(m_sorted_xs[I].begin(), m_sorted_xs[I].end(), min) - m_sorted_xs[I].begin();
                    size_type r = std::upper_bound(m_sorted_xs[I].begin(), m_sorted_xs[I].end(), max) - m_sorted_xs[I].begin() - 1;
                    return l == r + 1 ? 0 : _query<Operation, I + 1>(init_val, l0, r0, l1, r1, args..., l, r);
                }
            }
            Tree(size_type point_cnt = 0) { m_points.reserve(point_cnt); }
            template <typename... Args, typename = typename std::enable_if<!std::is_same<ElemType, bool>::value && sizeof...(Args) == DIM>::type>
            void add_point(ElemType w, Args... args) {
                if constexpr (HasModify)
                    m_points.push_back({w, args..., size_type(m_points.size())});
                else
                    m_points.push_back({w, args...});
            }
            template <typename... Args, typename = typename std::enable_if<std::is_same<ElemType, bool>::value && sizeof...(Args) == DIM>::type>
            void add_point(Args... args) {
                if constexpr (HasModify)
                    m_points.push_back({args..., size_type(m_points.size())});
                else
                    m_points.push_back({args...});
            }
            void prepare() {
                std::sort(m_points.begin(), m_points.end(), [](point &x, point &y) { return x.m_x[0] < y.m_x[0]; });
                m_sorted_xs[0].resize(m_points.size());
                for (size_type i = 0; i != m_points.size(); i++) m_sorted_xs[0][i] = m_points[i].m_x[0];
                std::vector<size_type> ids(m_points.size());
                std::iota(ids.begin(), ids.end(), 0);
                _prepare<1>(m_points.size(), ids.data());
                m_table.resize_mapping_with_index(
                    m_points.size(), [&](size_type i) -> size_type { return m_points[i].m_x[1]; }, alpha_table::s_val[1], [&](SizeType, size_type i) -> const point & { return m_points[i]; });
                if constexpr (HasModify) {
                    m_pos.resize(m_points.size());
                    for (size_type i = 0; i != m_points.size(); i++) m_pos[m_points[i].m_index] = i;
                }
            }
            template <typename Operation = std::plus<value_type>, typename... Args, typename = typename std::enable_if<sizeof...(Args) == DIM * 2 - 4>::type>
            value_type query(value_type init_val, SizeType x0_min, SizeType x0_max, SizeType x1_min, SizeType x1_max, Args... args) const {
                size_type l0 = std::lower_bound(m_sorted_xs[0].begin(), m_sorted_xs[0].end(), x0_min) - m_sorted_xs[0].begin();
                size_type r0 = std::upper_bound(m_sorted_xs[0].begin(), m_sorted_xs[0].end(), x0_max) - m_sorted_xs[0].begin() - 1;
                if (l0 == r0 + 1) return init_val;
                size_type l1 = std::lower_bound(m_sorted_xs[1].begin(), m_sorted_xs[1].end(), x1_min) - m_sorted_xs[1].begin();
                size_type r1 = std::upper_bound(m_sorted_xs[1].begin(), m_sorted_xs[1].end(), x1_max) - m_sorted_xs[1].begin() - 1;
                if (l1 == r1 + 1) return init_val;
                if constexpr (DIM == 2) {
                    value_type val = init_val;
                    auto call = [&val](value_type v) { val = Operation()(val, v); };
                    auto q = [](decltype(call) &call2, const adapter &tr, size_type left, size_type right) { tr.query(left, right, call2); };
                    m_table.do_for_value_range(l0, r0, l1, r1, call, q);
                    return val;
                } else
                    return _query<Operation, 2>(init_val, l0, r0, l1, r1, args...);
            }
            template <typename Callback>
            void do_in_table(size_type point_id, Callback &&call) {
                static_assert(HasModify, "HasModify Must Be True");
                m_table.do_in_table(m_pos[point_id], [&](adapter &tr, size_type pos) { tr.do_in_table(pos, call); });
            }
        };
    }
    template <typename SizeType, typename ElemType, typename BaseTable, bool HasModify = false>
    using Segtree2D = MDSEG::Tree<SizeType, ElemType, BaseTable, 2, HasModify>;
    template <typename SizeType, typename ElemType, typename BaseTable, bool HasModify = false>
    using Segtree3D = MDSEG::Tree<SizeType, ElemType, BaseTable, 3, HasModify>;
}

#endif