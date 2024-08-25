/*
最后修改:
20240825
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MONOZKWTREE__
#define __OY_MONOZKWTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace MONOZKW {
        using size_type = uint32_t;
        template <typename Tp, Tp Val>
        struct ConstexprIdentity {
            template <typename... Args>
            Tp operator()(Args...) const { return Val; }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename Tp, typename IdentityMapping, typename Operation>
        struct Tree {
            size_type m_size, m_depth, m_capacity;
            std::vector<Tp> m_sub;
            static bool _check_pushup(Tp &old, Tp val) { return old != val ? old = val, true : false; }
            Tree(size_type length = 0) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_depth = std::max<size_type>(1, std::bit_width(m_size - 1)), m_capacity = 1 << m_depth;
                m_sub.assign(m_capacity * 2, IdentityMapping()());
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_depth = std::max<size_type>(1, std::bit_width(m_size - 1)), m_capacity = 1 << m_depth;
                m_sub.resize(m_capacity * 2);
                Tp *sub = m_sub.data();
                for (size_type i = 0; i != m_size; i++) sub[m_capacity + i] = mapping(i);
                std::fill_n(sub + m_capacity + m_size, m_capacity - m_size, IdentityMapping()());
                for (size_type len = m_capacity / 2, w = 2; len; len >>= 1, w <<= 1)
                    for (size_type i = len; i != len << 1; i++) sub[i] = Operation()(sub[i * 2], sub[i * 2 + 1]);
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, Tp val) {
                Tp *sub = m_sub.data();
                sub[i += m_capacity] = val;
                while ((i >>= 1) && _check_pushup(sub[i], Operation()(sub[i * 2], sub[i * 2 + 1]))) {}
            }
            Tp query(size_type i) const { return m_sub[m_capacity + i]; }
            Tp query(size_type left, size_type right) const {
                if (left == right) return query(left);
                const Tp *sub = m_sub.data();
                Tp res = IdentityMapping()();
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        res = Operation()(res, sub[(m_capacity + left) >> j]);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    res = Operation()(res, sub[(m_capacity + left) >> (j - 1)]);
                    left = left2;
                }
                return res;
            }
            Tp query_all() const { return m_sub[1]; }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                const Tp *sub = m_sub.data();
                Tp val = sub[left += m_capacity];
                if (!judge(val)) return left - m_capacity - 1;
                left++;
                for (size_type len = 1; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    Tp a(Operation()(val, sub[left >>= ctz]));
                    len <<= ctz;
                    if (judge(a))
                        val = a, left++;
                    else {
                        for (; left < m_capacity; len >>= 1) {
                            Tp a(Operation()(val, sub[left <<= 1]));
                            if (judge(a)) val = a, left++;
                        }
                        return std::min(left - m_capacity, m_size) - 1;
                    }
                }
                return m_size - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                const Tp *sub = m_sub.data();
                Tp val = sub[right += m_capacity];
                if (!judge(val)) return right - m_capacity + 1;
                for (size_type len = 1; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_capacity);
                    Tp a(Operation()(sub[(right >>= ctz) - 1], val));
                    len >>= ctz;
                    if (judge(a))
                        val = a, right--;
                    else {
                        for (; right <= m_capacity; len >>= 1) {
                            Tp a(Operation()(sub[(right <<= 1) - 1], val));
                            if (judge(a)) val = a, right--;
                        }
                        return right - m_capacity;
                    }
                }
                return 0;
            }
        };
        template <typename Ostream, typename Tp, typename IdentityMapping, typename Operation>
        Ostream &operator<<(Ostream &out, const Tree<Tp, IdentityMapping, Operation> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using MonoMaxTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, Minimum>, MONOZKW::ChoiceByCompare<Tp, std::less<Tp>>>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using MonoMinTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, Maximum>, MONOZKW::ChoiceByCompare<Tp, std::greater<Tp>>>;
    template <typename Tp>
    using MonoGcdTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, 0>, MONOZKW::FpTransfer<Tp, std::gcd<Tp>>>;
    template <typename Tp>
    using MonoLcmTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, 1>, MONOZKW::FpTransfer<Tp, std::lcm<Tp>>>;
    template <typename Tp, Tp OneMask = Tp(-1)>
    using MonoBitAndTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, OneMask>, std::bit_and<Tp>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitOrTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, ZeroMask>, std::bit_or<Tp>>;
    template <typename Tp, Tp Zero = Tp()>
    using MonoSumTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, Zero>, std::plus<Tp>>;
}

#endif