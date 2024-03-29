/*
最后修改:
20240315
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STATICMONTGOMERYMODINT32__
#define __OY_STATICMONTGOMERYMODINT32__

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>

namespace OY {
    template <uint32_t P, bool IsPrime, typename = typename std::enable_if<(P % 2 && P > 1 && P < uint32_t(1) << 30)>::type>
    struct StaticMontgomeryModInt32 {
        using mint = StaticMontgomeryModInt32<P, IsPrime>;
        using mod_type = uint32_t;
        using fast_type = mod_type;
        using long_type = uint64_t;
        static constexpr mod_type _conv(mod_type x) { return x * (mod_type(2) - P * x); }
        static constexpr mod_type pinv = -_conv(_conv(_conv(_conv(P))));
        static constexpr mod_type ninv = -long_type(P) % P;
        fast_type m_val;
        static constexpr mod_type _mod(uint64_t val) { return val % mod(); }
        static constexpr fast_type _init(uint64_t val) { return _raw_init(_mod(val)); }
        static constexpr fast_type _raw_init(mod_type val) { return _mul(val, ninv); }
        static constexpr fast_type _reduce(long_type val) { return (val + long_type(mod_type(val) * pinv) * mod()) >> 32; }
        static constexpr fast_type _reduce_norm(int32_t x) { return x < 0 ? x + mod() : x; }
        static constexpr fast_type _mul(fast_type a, fast_type b) { return _reduce(long_type(a) * b); }
        constexpr StaticMontgomeryModInt32() = default;
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        constexpr StaticMontgomeryModInt32(Tp val) : m_val(_raw_init(_reduce_norm(val % int32_t(mod())))) {}
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        constexpr StaticMontgomeryModInt32(Tp val) : m_val{_init(val)} {}
        static constexpr mint _raw(fast_type val) {
            mint res;
            res.m_val = val;
            return res;
        }
        static constexpr mint raw(mod_type val) { return _raw(_raw_init(val)); }
        static constexpr mod_type mod() { return P; }
        constexpr mod_type val() const { return _reduce_norm(_reduce(m_val)); }
        constexpr mint pow(uint64_t n) const {
            fast_type res = _raw_init(1), b = m_val;
            while (n) {
                if (n & 1) res = _mul(res, b);
                b = _mul(b, b), n >>= 1;
            }
            return _raw(res);
        }
        constexpr mint inv() const {
            if constexpr (IsPrime)
                return inv_Fermat();
            else
                return inv_exgcd();
        }
        constexpr mint inv_exgcd() const {
            mod_type x = mod(), y = val(), m0 = 0, m1 = 1;
            while (y) {
                mod_type z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return m0;
        }
        constexpr mint inv_Fermat() const { return pow(mod() - 2); }
        constexpr mint &operator++() {
            (*this) += raw(1);
            return *this;
        }
        constexpr mint &operator--() {
            (*this) += raw(mod() - 1);
            return *this;
        }
        constexpr mint operator++(int) {
            mint old(*this);
            ++*this;
            return old;
        }
        constexpr mint operator--(int) {
            mint old(*this);
            --*this;
            return old;
        }
        mint &operator+=(const mint &rhs) {
            fast_type val1 = m_val + rhs.m_val, val2 = val1 - mod() * 2;
            m_val = int32_t(val2) < 0 ? val1 : val2;
            return *this;
        }
        mint &operator-=(const mint &rhs) {
            fast_type val1 = m_val - rhs.m_val, val2 = val1 + mod() * 2;
            m_val = int32_t(val1) < 0 ? val2 : val1;
            return *this;
        }
        mint &operator*=(const mint &rhs) {
            m_val = _mul(m_val, rhs.m_val);
            return *this;
        }
        constexpr mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        constexpr mint operator+() const { return *this; }
        constexpr mint operator-() const { return _raw(m_val ? mod() * 2 - m_val : 0); }
        constexpr bool operator==(const mint &rhs) const { return m_val == rhs.m_val; }
        constexpr bool operator!=(const mint &rhs) const { return m_val != rhs.m_val; }
        constexpr bool operator<(const mint &rhs) const { return m_val < rhs.m_val; }
        constexpr bool operator>(const mint &rhs) const { return m_val > rhs.m_val; }
        constexpr bool operator<=(const mint &rhs) const { return m_val <= rhs.m_val; }
        constexpr bool operator>=(const mint &rhs) const { return m_val <= rhs.m_val; }
        template <typename Tp>
        constexpr explicit operator Tp() const { return Tp(val()); }
        friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
    };
    template <typename Istream, uint32_t P, bool IsPrime>
    Istream &operator>>(Istream &is, StaticMontgomeryModInt32<P, IsPrime> &x) {
        uint32_t val;
        is >> val;
        x.m_val = StaticMontgomeryModInt32<P, IsPrime>::_raw_init(val);
        return is;
    }
    template <typename Ostream, uint32_t P, bool IsPrime>
    Ostream &operator<<(Ostream &os, const StaticMontgomeryModInt32<P, IsPrime> &x) { return os << x.val(); }
    using mgint998244353 = OY::StaticMontgomeryModInt32<998244353, true>;
    using mgint1000000007 = OY::StaticMontgomeryModInt32<1000000007, true>;
}

#endif