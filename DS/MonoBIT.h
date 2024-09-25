/*
最后修改:
20240904
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MONOBIT__
#define __OY_MONOBIT__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace MONOBIT {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        inline size_type meet(size_type a, size_type b) { return ((a + 1) & -(size_type(1) << std::bit_width((a + 1) ^ (b + 1)))) - 1; }
        struct Self {
            template <typename Tp>
            Tp operator()(const Tp &x) const { return x; }
        };
        template <typename Tp, Tp Identity, typename Operation>
        struct BaseCommutativeMonoid {
            using value_type = Tp;
            static constexpr Tp identity() { return Identity; }
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, Tp Identity, typename Operation, typename Inverse>
        struct BaseCommutativeGroup {
            using value_type = Tp;
            static constexpr Tp identity() { return Identity; }
            static value_type inverse(const value_type &x) { return Inverse()(x); }
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename CommutativeGroup>
        class Tree {
        public:
            using group = CommutativeGroup;
            using value_type = typename group::value_type;
        private:
            size_type m_size, m_cap;
            std::vector<value_type> m_sub;
        public:
            Tree(size_type length = 0) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size - 1));
                m_sub.assign(m_cap, group::identity());
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size - 1));
                m_sub.resize(m_cap);
                auto sub = m_sub.data();
                for (size_type i = 0; i != m_size; i++) sub[i] = mapping(i);
                std::fill(sub + m_size, sub + m_cap, group::identity());
                for (size_type i = 0; i != m_size; i++) {
                    size_type j = i + lowbit(i + 1);
                    if (j < m_cap) sub[j] = group::op(sub[i], sub[j]);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, value_type inc) {
                auto sub = m_sub.data();
                while (i < m_cap) {
                    sub[i] = group::op(inc, sub[i]);
                    i += lowbit(i + 1);
                }
            }
            value_type presum(size_type i) const {
                auto sub = m_sub.data();
                value_type res = group::identity();
                for (size_type j = i; ~j; j -= lowbit(j + 1)) res = group::op(sub[j], res);
                return res;
            }
            value_type query(size_type left, size_type right) const {
                auto sub = m_sub.data();
                value_type resl = group::identity(), resr = group::identity();
                size_type _m = meet(left - 1, right);
                for (size_type j = left - 1; j != _m; j -= lowbit(j + 1)) resl = group::op(resl, sub[j]);
                for (size_type j = right; j != _m; j -= lowbit(j + 1)) resr = group::op(resr, sub[j]);
                return group::op(resr, group::inverse(resl));
            }
            value_type query_all() const { return m_sub.back(); }
            template <typename Judge>
            size_type max_right(Judge &&judge) const {
                auto sub = m_sub.data();
                if (!judge(sub[0])) return -1;
                if (judge(sub[m_cap - 1])) return m_size - 1;
                size_type cursor = -1;
                value_type cur = group::identity();
                for (size_type d = m_cap >> 1; d; d >>= 1) {
                    value_type a = group::op(cur, sub[cursor + d]);
                    if (judge(a)) cur = a, cursor += d;
                }
                return std::min(cursor, m_size - 1);
            }
        };
    }
    template <typename Tp, Tp Identity, typename Operation, typename InitMapping, typename TreeType = MONOBIT::Tree<MONOBIT::BaseCommutativeMonoid<Tp, Identity, Operation>>>
    auto make_MonoBIT(MONOBIT::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using MonoMaxBIT = MONOBIT::Tree<MONOBIT::BaseCommutativeMonoid<Tp, Minimum, MONOBIT::ChoiceByCompare<Tp, std::less<Tp>>>>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using MonoMinBIT = MONOBIT::Tree<MONOBIT::BaseCommutativeMonoid<Tp, Maximum, MONOBIT::ChoiceByCompare<Tp, std::greater<Tp>>>>;
    template <typename Tp>
    using MonoGcdBIT = MONOBIT::Tree<MONOBIT::BaseCommutativeMonoid<Tp, 0, MONOBIT::FpTransfer<Tp, std::gcd<Tp>>>>;
    template <typename Tp>
    using MonoLcmBIT = MONOBIT::Tree<MONOBIT::BaseCommutativeMonoid<Tp, 1, MONOBIT::FpTransfer<Tp, std::lcm<Tp>>>>;
    template <typename Tp, Tp OneMask = Tp(-1)>
    using MonoBitAndBIT = MONOBIT::Tree<MONOBIT::BaseCommutativeMonoid<Tp, OneMask, std::bit_and<Tp>>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitOrBIT = MONOBIT::Tree<MONOBIT::BaseCommutativeMonoid<Tp, ZeroMask, std::bit_or<Tp>>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitXorBIT = MONOBIT::Tree<MONOBIT::BaseCommutativeGroup<Tp, ZeroMask, std::bit_xor<Tp>, MONOBIT::Self>>;
    template <typename Tp, Tp Zero = Tp()>
    using MonoSumBIT = MONOBIT::Tree<MONOBIT::BaseCommutativeGroup<Tp, Zero, std::plus<Tp>, std::negate<Tp>>>;
}

#endif