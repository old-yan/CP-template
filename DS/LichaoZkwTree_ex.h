/*
最后修改:
20240919
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LICHAOZKWTREEEX__
#define __OY_LICHAOZKWTREEEX__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace LCZKWEX {
        using size_type = uint32_t;
        template <typename Tp>
        struct BaseLine {
            Tp m_k, m_b;
            static BaseLine<Tp> make_constant_line(Tp val) { return BaseLine<Tp>(0, val); }
            BaseLine() = default;
            BaseLine(Tp k, Tp b) : m_k(k), m_b(b) {}
            void add(Tp inc) { m_b += inc; }
            Tp calc(Tp i) const { return m_k * i + m_b; }
            bool is_ascending() const { return m_k > 0; }
            bool is_horizontal() const { return m_k == 0; }
        };
        struct BaseLess {
            template <typename Tp>
            bool operator()(const Tp &x, const Tp &y) const { return x < y; }
        };
        template <typename Tp, Tp BackGroundValue>
        struct BaseJudger {
            template <typename Line>
            bool operator()(const Line &line) const { return line.is_horizontal() && std::abs(line.calc(0)) >= std::abs(BackGroundValue) / 2; }
        };
        template <typename Tp, typename Line, bool MaintainMax>
        struct Info {
            using line_type = Line;
            Line m_line;
            Tp m_max;
            void set(Tp val) { m_max = val; }
            Tp get() const { return m_max; }
            void update(Tp val) { m_max = std::max(m_max, val); }
            bool operator<(const Info &rhs) const { return m_max < rhs.m_max; }
        };
        template <typename Tp, typename Line>
        struct Info<Tp, Line, false> {
            using line_type = Line;
            Line m_line;
            Tp m_min;
            void set(Tp val) { m_min = val; }
            Tp get() const { return m_min; }
            void update(Tp val) { m_min = std::min(m_min, val); }
            bool operator<(const Info &rhs) const { return m_min > rhs.m_min; }
        };
        template <typename Node, typename Tp, bool HasInc>
        struct NodeBase {
            Tp m_inc;
        };
        template <typename Node, typename Tp>
        struct NodeBase<Node, Tp, false> {};
        template <typename Tp, typename Line, typename Info>
        struct NoLazy {
            template <typename SizeType>
            Tp get(const Line &line, SizeType i) const { return line.calc(i); }
            const Info &get(const Info &info) const { return info; }
            Tp get(Tp val) const { return val; }
            template <typename Node>
            NoLazy<Tp, Line, Info> apply(Node &) const { return {}; }
        };
        template <typename Tp, typename Line, typename Info>
        struct Lazy {
            Tp m_inc;
            template <typename SizeType>
            Tp get(Line line, SizeType i) const {
                line.add(m_inc);
                return line.calc(i);
            }
            Info get(Info info) const {
                info.m_line.add(m_inc), info.set(info.get() + m_inc);
                return info;
            }
            Tp get(Tp val) const { return val + m_inc; }
            template <typename Node>
            Lazy<Tp, Line, Info> apply(const Node &p) const { return {m_inc + p.m_inc}; }
        };
        template <typename Tp, Tp BackGroundValue, bool MaintainMax = true, bool RangeAdd = false, typename Line = BaseLine<Tp>, typename BackGroundJudger = BaseJudger<Tp, BackGroundValue>>
        class Tree {
            static_assert(std::is_signed<Tp>::value, "Tp Must Be Signed Type");
        public:
            using info_type = Info<Tp, Line, MaintainMax>;
            using lazy_type = typename std::conditional<RangeAdd, Lazy<Tp, Line, info_type>, NoLazy<Tp, Line, info_type>>::type;
            using comp_type = typename std::conditional<MaintainMax, std::less<Tp>, std::greater<Tp>>::type;
            struct node : NodeBase<node, Tp, RangeAdd> {
                info_type m_info;
            };
        private:
            std::vector<node> m_sub;
            size_type m_size, m_cap;
            static Tp _most(const Line &line, size_type left, size_type right) {
                if constexpr (MaintainMax)
                    return line.calc(line.is_ascending() ? right : left);
                else
                    return line.calc(line.is_ascending() ? left : right);
            }
            void _add(size_type i, size_type floor, size_type ceil, Line line) {
                size_type mid = (floor + ceil) >> 1;
                m_sub[i].m_info.update(_most(line, floor, ceil));
                if (comp_type()(m_sub[i].m_info.m_line.calc(mid), line.calc(mid))) std::swap(m_sub[i].m_info.m_line, line);
                if (floor < ceil && !BackGroundJudger()(line))
                    if (comp_type()(m_sub[i].m_info.m_line.calc(floor), line.calc(floor))) {
                        if constexpr (RangeAdd) line.add(-m_sub[i].m_inc);
                        _add(i * 2, floor, mid, line);
                    } else if (comp_type()(m_sub[i].m_info.m_line.calc(ceil), line.calc(ceil))) {
                        if constexpr (RangeAdd) line.add(-m_sub[i].m_inc);
                        _add(i * 2 + 1, mid + 1, ceil, line);
                    }
            }
            void _add(size_type i, size_type floor, size_type ceil, size_type left, size_type right, Line line) {
                if (left == floor && right == ceil)
                    _add(i, floor, ceil, line);
                else {
                    size_type mid = (floor + ceil) >> 1;
                    m_sub[i].m_info.update(_most(line, left, right));
                    if constexpr (RangeAdd) line.add(-m_sub[i].m_inc);
                    if (left <= mid) _add(i * 2, floor, mid, left, std::min(mid, right), line);
                    if (right > mid) _add(i * 2 + 1, mid + 1, ceil, std::max(mid + 1, left), right, line);
                }
            }
            void _add(size_type i, size_type floor, size_type ceil, size_type left, size_type right, Tp inc) {
                auto sub = m_sub.data();
                if (left == floor && right == ceil)
                    sub[i].m_info.m_line.add(inc), sub[i].m_info.set(sub[i].m_info.get() + inc), sub[i].m_inc += inc;
                else {
                    size_type mid = (floor + ceil) >> 1;
                    if (!BackGroundJudger()(sub[i].m_info.m_line)) {
                        auto line = sub[i].m_info.m_line;
                        line.add(-sub[i].m_inc);
                        _add(i * 2, floor, mid, line), _add(i * 2 + 1, mid + 1, ceil, line);
                    }
                    if (left <= mid) _add(i * 2, floor, mid, left, std::min(mid, right), inc);
                    if (right > mid) _add(i * 2 + 1, mid + 1, ceil, std::max(mid + 1, left), right, inc);
                    sub[i].m_info = {Line::make_constant_line(BackGroundValue), BackGroundValue};
                    sub[i].m_info.update(sub[i * 2].m_info.get() + sub[i].m_inc);
                    sub[i].m_info.update(sub[i * 2 + 1].m_info.get() + sub[i].m_inc);
                }
            }
            void _pushup(size_type i, size_type end) {
                auto sub = m_sub.data();
                while ((i >>= 1) != end) sub[i].m_info.update(std::max(sub[i * 2].m_info.get(), sub[i * 2 + 1].m_info.get(), comp_type()));
            }
            void _query(size_type cur, size_type floor, size_type ceil, size_type left, size_type right, lazy_type lazy, Tp &res) const {
                if (!comp_type()(res, lazy.get(m_sub[cur].m_info.get()))) return;
                if (left == floor && right == ceil)
                    res = lazy.get(m_sub[cur].m_info.get());
                else {
                    size_type mid = (floor + ceil) >> 1;
                    res = std::max(res, lazy.get(_most(m_sub[cur].m_info.m_line, left, right)), comp_type());
                    if (left <= mid) _query(cur * 2, floor, mid, left, std::min(mid, right), lazy.apply(m_sub[cur]), res);
                    if (right > mid) _query(cur * 2 + 1, mid + 1, ceil, std::max(mid + 1, left), right, lazy.apply(m_sub[cur]), res);
                }
            }
            void _query(size_type i, size_type left, size_type right, Tp &res) const {
                auto sub = m_sub.data();
                res = std::max(res, sub[i].m_info.get(), comp_type());
                for (size_type d = std::bit_width(i) - 1; d && comp_type()(res, sub[i >> d].m_info.get()); d--) res = std::max(res, _most(sub[i >> d].m_info.m_line, left, right), comp_type());
            }
        public:
            Tree(size_type length = 0) { resize(length); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                node x{};
                x.m_info = {Line::make_constant_line(BackGroundValue), BackGroundValue};
                m_cap = std::bit_ceil(m_size), m_sub.assign(m_cap << 1, x);
            }
            void add_line(size_type i, const Line &line) {
                auto sub = m_sub.data();
                auto val = line.calc(i);
                for (size_type d = std::countr_zero(m_cap); ~d; d--) {
                    sub[(m_cap + i) >> d].m_info.update(val);
                    if constexpr (RangeAdd) val -= sub[(m_cap + i) >> d].m_inc;
                }
            }
            void add_line(size_type left, size_type right, const Line &line) {
                if constexpr (RangeAdd)
                    _add(1, 0, m_cap - 1, left, right, line);
                else {
                    right++;
                    if (left)
                        while (true) {
                            size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                            if (left2 > right) break;
                            _add((m_cap + left) >> j, left, left2 - 1, line);
                            _pushup((m_cap + left) >> j, left2 < right ? (m_cap + left2) >> (std::countr_zero(left2) + 1) : 0);
                            left = left2;
                        }
                    while (left < right) {
                        size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                        _add((m_cap + left) >> (j - 1), left, left2 - 1, line);
                        _pushup((m_cap + left) >> (j - 1), left2 < right ? (m_cap + left) >> j : 0);
                        left = left2;
                    }
                }
            }
            void add_value(size_type left, size_type right, Tp inc) {
                static_assert(RangeAdd, "RangeAdd Must Be True");
                _add(1, 0, m_cap - 1, left, right, inc);
            }
            Tp query(size_type i) const {
                auto sub = m_sub.data();
                Tp res = sub[1].m_info.m_line.calc(i);
                lazy_type lazy{};
                for (size_type d = std::countr_zero(m_cap); d;) {
                    lazy = lazy.apply(sub[(m_cap + i) >> d]);
                    res = std::max(res, lazy.get(sub[(m_cap + i) >> --d].m_info.m_line, i), comp_type());
                }
                return std::max(res, lazy.get(sub[m_cap + i].m_info.get()), comp_type());
            }
            Tp query(size_type left, size_type right) const {
                Tp res = BackGroundValue;
                _query(1, 0, m_cap - 1, left, right, {}, res);
                return res;
            }
        };
    }
    template <typename Tp, Tp BackGroundValue = std::numeric_limits<Tp>::min(), bool RangeAdd = false, typename Line = LCZKWEX::BaseLine<Tp>, typename BackGroundJudger = LCZKWEX::BaseJudger<Tp, BackGroundValue>>
    using LichaoSlopeChmaxZkwTree_ex = LCZKWEX::Tree<Tp, BackGroundValue, true, RangeAdd, Line, BackGroundJudger>;
    template <typename Tp, Tp BackGroundValue = std::numeric_limits<Tp>::max(), bool RangeAdd = false, typename Line = LCZKWEX::BaseLine<Tp>, typename BackGroundJudger = LCZKWEX::BaseJudger<Tp, BackGroundValue>>
    using LichaoSlopeChminZkwTree_ex = LCZKWEX::Tree<Tp, BackGroundValue, false, RangeAdd, Line, BackGroundJudger>;
}

#endif