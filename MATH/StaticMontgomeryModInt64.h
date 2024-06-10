/*
最后修改:
20240316
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STATICMONTGOMERYMODINT64__
#define __OY_STATICMONTGOMERYMODINT64__

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>

#ifdef _MSC_VER
#include <immintrin.h>
#endif

namespace OY {
    template <uint64_t P, bool IsPrime, typename = typename std::enable_if<(P % 2 && P > 1 && P < uint64_t(1) << 62)>::type>
    struct StaticMontgomeryModInt64 {
        using mint = StaticMontgomeryModInt64<P, IsPrime>;
        using mod_type = uint64_t;
        using fast_type = mod_type;
#ifndef _MSC_VER
        using long_type = __uint128_t;
#endif
        static constexpr mod_type _conv(mod_type x) { return x * (mod_type(2) - P * x); }
        static constexpr mod_type _shift1(mod_type x) { return x * 2 % P; }
        static constexpr mod_type _shift4(mod_type x) { return _shift1(_shift1(_shift1(_shift1(x)))); }
        static constexpr mod_type _shift16(mod_type x) { return _shift4(_shift4(_shift4(_shift4(x)))); }
        static constexpr mod_type _shift64(mod_type x) { return _shift16(_shift16(_shift16(_shift16(x)))); }
        static constexpr mod_type pinv = -_conv(_conv(_conv(_conv(_conv(P)))));
        static constexpr mod_type ninv = _shift64(_shift64(1));
        fast_type m_val;
        static mod_type _mod(uint64_t val) { return val % mod(); }
        static fast_type _init(mod_type val) { return _raw_init(_mod(val)); }
        static fast_type _raw_init(mod_type val) { return _mul(val, ninv); }
#ifdef _MSC_VER
        static fast_type _reduce(mod_type high, mod_type low) {
            uint64_t _, res, low2 = _umul128(_umul128(low, pinv, &_), mod(), &res);
            if (low2 > -low || (low2 == -low && low)) ++res;
            return res + high;
#else
        static fast_type _reduce(long_type val) {
            return (val + long_type(mod_type(val) * pinv) * mod()) >> 64;
#endif
        }
        static constexpr fast_type _reduce_zero(mod_type x) { return x == mod() ? 0 : x; }
        static constexpr fast_type _strict_reduce(fast_type val) { return val >= mod() ? val - mod() : val; }
        static fast_type _mul(fast_type a, fast_type b) {
#ifdef _MSC_VER
            mod_type high, low;
            low = _umul128(a, b, &high);
            return _reduce(high, low);
#else
            return _reduce(long_type(a) * b);
#endif
        }
        StaticMontgomeryModInt64() = default;
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        StaticMontgomeryModInt64(Tp val) {
            auto x = val % int64_t(mod());
            if (x < 0) x += mod();
            m_val = _raw_init(x);
        }
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        StaticMontgomeryModInt64(Tp val) : m_val{_init(val)} {}
        static mint _raw(fast_type val) {
            mint res;
            res.m_val = val;
            return res;
        }
        static mint raw(mod_type val) { return _raw(_raw_init(val)); }
        static constexpr mod_type mod() { return P; }
        mod_type val() const {
#ifdef _MSC_VER
            mod_type res = _reduce(0, m_val);
#else
            mod_type res = _reduce(m_val);
#endif
            return _reduce_zero(res);
        }
        mint pow(uint64_t n) const {
            fast_type res = _raw_init(1), b = m_val;
            while (n) {
                if (n & 1) res = _mul(res, b);
                b = _mul(b, b), n >>= 1;
            }
            return _raw(res);
        }
        mint inv() const {
            if constexpr (IsPrime)
                return inv_Fermat();
            else
                return inv_exgcd();
        }
        mint inv_exgcd() const {
            mod_type x = mod(), y = val(), m0 = 0, m1 = 1;
            while (y) {
                mod_type z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return m0;
        }
        mint inv_Fermat() const { return pow(mod() - 2); }
        mint &operator++() {
            (*this) += raw(1);
            return *this;
        }
        mint &operator--() {
            (*this) += raw(mod() - 1);
            return *this;
        }
        mint operator++(int) {
            mint old(*this);
            ++*this;
            return old;
        }
        mint operator--(int) {
            mint old(*this);
            --*this;
            return old;
        }
        mint &operator+=(const mint &rhs) {
            fast_type val1 = m_val + rhs.m_val, val2 = val1 - mod() * 2;
            m_val = int64_t(val2) < 0 ? val1 : val2;
            return *this;
        }
        mint &operator-=(const mint &rhs) {
            fast_type val1 = m_val - rhs.m_val, val2 = val1 + mod() * 2;
            m_val = int64_t(val1) < 0 ? val2 : val1;
            return *this;
        }
        mint &operator*=(const mint &rhs) {
            m_val = _mul(m_val, rhs.m_val);
            return *this;
        }
        mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        mint operator+() const { return *this; }
        mint operator-() const { return _raw(m_val ? mod() * 2 - m_val : 0); }
        bool operator==(const mint &rhs) const { return _strict_reduce(m_val) == _strict_reduce(rhs.m_val); }
        bool operator!=(const mint &rhs) const { return _strict_reduce(m_val) != _strict_reduce(rhs.m_val); }
        bool operator<(const mint &rhs) const { return _strict_reduce(m_val) < _strict_reduce(rhs.m_val); }
        bool operator>(const mint &rhs) const { return _strict_reduce(m_val) > _strict_reduce(rhs.m_val); }
        bool operator<=(const mint &rhs) const { return _strict_reduce(m_val) <= _strict_reduce(rhs.m_val); }
        bool operator>=(const mint &rhs) const { return _strict_reduce(m_val) >= _strict_reduce(rhs.m_val); }
        template <typename Tp>
        explicit operator Tp() const { return Tp(val()); }
        friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
    };
    template <typename Istream, uint64_t P, bool IsPrime>
    Istream &operator>>(Istream &is, StaticMontgomeryModInt64<P, IsPrime> &x) {
        uint64_t val;
        is >> val;
        x.m_val = StaticMontgomeryModInt64<P, IsPrime>::_raw_init(val);
        return is;
    }
    template <typename Ostream, uint64_t P, bool IsPrime>
    Ostream &operator<<(Ostream &os, const StaticMontgomeryModInt64<P, IsPrime> &x) { return os << x.val(); }
    using mgint4611686018427387847 = StaticMontgomeryModInt64<4611686018427387847, true>;
}

#endif