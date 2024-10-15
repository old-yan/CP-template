/*
最后修改:
20241014
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TAGZKWTREE__
#define __OY_TAGZKWTREE__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace TAGZKW {
        using size_type = uint32_t;
        template <typename Tp>
        struct AddMonoid {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            static Tp pow(const Tp &x, size_type n) { return x * n; }
        };
        template <typename Tp>
        struct BitxorMonoid {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x ^ y; }
            static Tp pow(const Tp &x, size_type n) { return n & 1 ? x : Tp{}; }
        };
        template <typename Tp, Tp Identity, typename Operation, typename Pow>
        struct BaseMonoid {
            using value_type = Tp;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
            static Tp pow(const Tp &x, size_type n) { return Pow()(x, n); }
        };
        template <typename Tp, Tp Identity, typename Operation>
        struct SelfPowMonoid {
            using value_type = Tp;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
            static Tp pow(const Tp &x, size_type) { return x; }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename CommutativeMonoid>
        class Tree {
        public:
            using group = CommutativeMonoid;
            using value_type = typename group::value_type;
            struct node {
                value_type m_val, m_tag;
            };
        private:
            size_type m_size, m_cap, m_dep;
            std::vector<node> m_sub;
            static void _compound(value_type &x, const value_type &y) { x = group::op(x, y); }
            template <typename Callback>
            void _dfs(size_type i, value_type val, Callback &&call) const {
                if (i << (m_dep - std::bit_width(i) + 1) >= m_cap + m_size) return;
                if (i >= m_cap)
                    call(group::op(val, m_sub[i].m_val));
                else
                    _compound(val, m_sub[i].m_tag), _dfs(i * 2, val, call), _dfs(i * 2 + 1, val, call);
            }
        public:
            Tree(size_type length = 0) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            size_type size() const { return m_size; }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_dep = std::max<size_type>(1, std::bit_width(m_size - 1)), m_cap = 1 << m_dep;
                m_sub.assign(m_cap * 2, {group::identity(), group::identity()});
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_dep = std::max<size_type>(1, std::bit_width(m_size - 1)), m_cap = 1 << m_dep;
                m_sub.resize(m_cap * 2);
                auto sub = m_sub.data();
                for (size_type i = 0; i != m_size; i++) sub[m_cap + i].m_val = mapping(i), sub[m_cap + i].m_tag = group::identity();
                std::fill_n(sub + m_cap + m_size, m_cap - m_size, {group::identity(), group::identity()});
                for (size_type len = m_cap / 2, w = 2; len; len >>= 1, w <<= 1)
                    for (size_type i = len; i != len << 1; i++) sub[i].m_val = group::op(sub[i * 2].m_val, sub[i * 2 + 1].m_val), sub[i].m_tag = group::identity();
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, value_type inc) {
                auto sub = m_sub.data();
                for (size_type j = m_dep; ~j; j--) _compound(sub[(m_cap + i) >> j].m_val, inc);
            }
            void add(size_type left, size_type right, value_type inc) {
                auto sub = m_sub.data();
                auto update = [&](size_type i) { sub[i].m_val = group::op(group::op(sub[i * 2].m_val, sub[i * 2 + 1].m_val), group::pow(sub[i].m_tag, size_type(1) << (m_dep + 1 - std::bit_width(i)))); };
                size_type cur = left;
                right++;
                if (cur)
                    while (true) {
                        size_type j = std::countr_zero(cur), nxt = cur + (size_type(1) << j);
                        if (nxt > right) break;
                        _compound(sub[(m_cap + cur) >> j].m_val, group::pow(inc, nxt - cur)), _compound(sub[(m_cap + cur) >> j].m_tag, inc);
                        for (size_type i = (m_cap + cur) >> j, end = nxt < right ? (m_cap + nxt) >> (std::countr_zero(nxt) + 1) : 0; (i >>= 1) != end;) update(i);
                        cur = nxt;
                    }
                while (cur < right) {
                    size_type j = std::bit_width(cur ^ right), nxt = cur + (size_type(1) << (j - 1));
                    _compound(sub[(m_cap + cur) >> (j - 1)].m_val, group::pow(inc, nxt - cur)), _compound(sub[(m_cap + cur) >> (j - 1)].m_tag, inc);
                    for (size_type i = (m_cap + cur) >> (j - 1), end = nxt < right ? (m_cap + cur) >> j : 0; (i >>= 1) != end;) update(i);
                    cur = nxt;
                }
            }
            value_type query(size_type i) const {
                auto sub = m_sub.data();
                value_type res = sub[1].m_tag;
                for (size_type j = m_dep - 1; j; j--) _compound(res, sub[(m_cap + i) >> j].m_tag);
                return group::op(res, sub[m_cap + i].m_val);
            }
            value_type query(size_type left, size_type right) const {
                if (left == right) return query(left);
                const node *sub = m_sub.data();
                size_type t = std::bit_width(left ^ right) - 1;
                value_type inc = sub[1].m_tag;
                for (size_type j = m_dep - 1; j != t; j--) _compound(inc, sub[(m_cap + left) >> j].m_tag);
                value_type res = group::pow(inc, right - left + 1);
                auto collect_left = [&] {
                    for (size_type i = (m_cap + left) >> t, j = t; j; j--) {
                        if (!(left & ((size_type(1) << j) - 1))) return _compound(res, sub[i].m_val);
                        _compound(res, group::pow(sub[i].m_tag, ((~left) & ((size_type(1) << j) - 1)) + 1)), i = i * 2 + 1;
                        if (!(left >> (j - 1) & 1)) _compound(res, sub[i--].m_val);
                    };
                    _compound(res, sub[m_cap + left].m_val);
                };
                auto collect_right = [&] {
                    for (size_type i = (m_cap + right) >> t, j = t; j; j--) {
                        if (!((~right) & ((size_type(1) << j) - 1))) return _compound(res, sub[i].m_val);
                        _compound(res, group::pow(sub[i].m_tag, (right & ((size_type(1) << j) - 1)) + 1)), i = i * 2;
                        if (right >> (j - 1) & 1) _compound(res, sub[i++].m_val);
                    };
                    _compound(res, sub[m_cap + right].m_val);
                };
                collect_left(), collect_right();
                return res;
            }
            value_type query_all() const { return m_sub[1].m_val; }
            template <typename Callback>
            void enumerate(Callback &&call) const { _dfs(1, group::identity(), call); }
        };
        template <typename Ostream, typename CommutativeMonoid>
        Ostream &operator<<(Ostream &out, const Tree<CommutativeMonoid> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, Tp Identity, typename Operation, typename Pow, typename InitMapping, typename TreeType = TAGZKW::Tree<TAGZKW::BaseMonoid<Tp, Identity, Operation, Pow>>>
    auto make_TagZkw(TAGZKW::size_type length, Operation op, Pow pow, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using TagMaxZkw = TAGZKW::Tree<TAGZKW::SelfPowMonoid<Tp, Minimum, TAGZKW::ChoiceByCompare<Tp, std::less<Tp>>>>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using TagMinZkw = TAGZKW::Tree<TAGZKW::SelfPowMonoid<Tp, Maximum, TAGZKW::ChoiceByCompare<Tp, std::greater<Tp>>>>;
    template <typename Tp>
    using TagGcdZkw = TAGZKW::Tree<TAGZKW::SelfPowMonoid<Tp, 0, TAGZKW::FpTransfer<Tp, std::gcd<Tp>>>>;
    template <typename Tp>
    using TagLcmZkw = TAGZKW::Tree<TAGZKW::SelfPowMonoid<Tp, 1, TAGZKW::FpTransfer<Tp, std::lcm<Tp>>>>;
    template <typename Tp, Tp OneMask = Tp(-1)>
    using TagBitAndZkw = TAGZKW::Tree<TAGZKW::SelfPowMonoid<Tp, OneMask, std::bit_and<Tp>>>;
    template <typename Tp, Tp ZeroMask = Tp(0)>
    using TagBitOrZkw = TAGZKW::Tree<TAGZKW::SelfPowMonoid<Tp, ZeroMask, std::bit_or<Tp>>>;
    template <typename Tp>
    using TagBitXorZkw = TAGZKW::Tree<TAGZKW::BitxorMonoid<Tp>>;
    template <typename Tp>
    using TagSumZkw = TAGZKW::Tree<TAGZKW::AddMonoid<Tp>>;
}

#endif