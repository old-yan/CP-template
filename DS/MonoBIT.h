/*
最后修改:
20240902
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
            size_type m_size, m_cap;
            std::vector<Tp> m_sub;
            static size_type _lowbit(size_type x) { return x & -x; }
            Tree(size_type length = 0) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size - 1));
                m_sub.assign(m_cap, IdentityMapping()());
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size - 1));
                m_sub.resize(m_cap);
                Tp *sub = m_sub.data();
                for (size_type i = 0; i != m_size; i++) sub[i] = mapping(i);
                std::fill(sub + m_size, sub + m_cap, IdentityMapping()());
                for (size_type i = 0; i != m_size; i++) {
                    size_type j = i + _lowbit(i + 1);
                    if (j < m_cap) sub[j] = Operation()(sub[i], sub[j]);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, Tp inc) {
                Tp *sub = m_sub.data();
                while (i < m_cap) {
                    sub[i] = Operation()(inc, sub[i]);
                    i += _lowbit(i + 1);
                }
            }
            Tp presum(size_type i) const {
                auto sub = m_sub.data();
                Tp res = IdentityMapping()();
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res = Operation()(sub[j], res);
                return res;
            }
            Tp query_all() const { return m_sub.back(); }
        };
    }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using MonoMaxBIT = MONOBIT::Tree<Tp, MONOBIT::ConstexprIdentity<Tp, Minimum>, MONOBIT::ChoiceByCompare<Tp, std::less<Tp>>>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using MonoMinBIT = MONOBIT::Tree<Tp, MONOBIT::ConstexprIdentity<Tp, Maximum>, MONOBIT::ChoiceByCompare<Tp, std::greater<Tp>>>;
    template <typename Tp>
    using MonoGcdBIT = MONOBIT::Tree<Tp, MONOBIT::ConstexprIdentity<Tp, 0>, MONOBIT::FpTransfer<Tp, std::gcd<Tp>>>;
    template <typename Tp>
    using MonoLcmBIT = MONOBIT::Tree<Tp, MONOBIT::ConstexprIdentity<Tp, 1>, MONOBIT::FpTransfer<Tp, std::lcm<Tp>>>;
    template <typename Tp, Tp OneMask = Tp(-1)>
    using MonoBitAndBIT = MONOBIT::Tree<Tp, MONOBIT::ConstexprIdentity<Tp, OneMask>, std::bit_and<Tp>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitOrBIT = MONOBIT::Tree<Tp, MONOBIT::ConstexprIdentity<Tp, ZeroMask>, std::bit_or<Tp>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitXorBIT = MONOBIT::Tree<Tp, MONOBIT::ConstexprIdentity<Tp, ZeroMask>, std::bit_xor<Tp>>;
    template <typename Tp, Tp Zero = Tp()>
    using MonoSumBIT = MONOBIT::Tree<Tp, MONOBIT::ConstexprIdentity<Tp, Zero>, std::plus<Tp>>;
    template <typename Tp, Tp One = 1>
    using MonoMulBIT = MONOBIT::Tree<Tp, MONOBIT::ConstexprIdentity<Tp, One>, std::multiplies<Tp>>;
}

#endif