/*
最后修改:
20240329
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OVERFLOWUNSIGNED__
#define __OY_OVERFLOWUNSIGNED__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>

#if __cpp_constexpr >= 201304L
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14
#endif

namespace OY {
    template <typename ModType>
    struct OverflowUnsigned {
        using mint = OverflowUnsigned<ModType>;
        using mod_type = ModType;
        using signed_type = typename std::make_signed<mod_type>::type;
        static CONSTEXPR14 mod_type _inv_exgcd(mod_type val) {
            mod_type m0 = 1, m1 = -(-val / val), x = val, y = m1 * val;
            while (y) {
                mod_type z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            return m0;
        }
        mod_type m_val;
        constexpr OverflowUnsigned() = default;
        template <typename Tp, typename std::enable_if<std::is_integral<Tp>::value>::type * = nullptr>
        constexpr OverflowUnsigned(Tp val) : m_val(val) {}
        static constexpr mint raw(mod_type val) { return mint(val); }
        static constexpr mod_type mod() { return 0; }
        constexpr mod_type val() const { return m_val; }
        CONSTEXPR14 mint pow(uint64_t n) const {
            mod_type res = 1, b = m_val;
            while (n) {
                if (n & 1) res *= b;
                b *= b, n >>= 1;
            }
            return raw(res);
        }
        constexpr mint inv() const { return raw(signed_type(m_val) >= 0 ? _inv_exgcd(m_val) : (_inv_exgcd(m_val ^ (mod_type(1) << ((sizeof(mod_type) << 3) - 1))) ^ (mod_type(1) << ((sizeof(mod_type) << 3) - 1)))); }
        CONSTEXPR14 mint &operator++() {
            ++m_val;
            return *this;
        }
        CONSTEXPR14 mint &operator--() {
            --m_val;
            return *this;
        }
        CONSTEXPR14 mint operator++(int) {
            mint old(*this);
            ++*this;
            return old;
        }
        CONSTEXPR14 mint operator--(int) {
            mint old(*this);
            --*this;
            return old;
        }
        CONSTEXPR14 mint &operator+=(const mint &rhs) {
            m_val += rhs.m_val;
            return *this;
        }
        CONSTEXPR14 mint &operator-=(const mint &rhs) {
            m_val -= rhs.m_val;
            return *this;
        }
        CONSTEXPR14 mint &operator*=(const mint &rhs) {
            m_val *= rhs.m_val;
            return *this;
        }
        CONSTEXPR14 mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        constexpr mint operator+() const { return *this; }
        constexpr mint operator-() const { return raw(-m_val); }
        constexpr bool operator==(const mint &rhs) const { return m_val == rhs.m_val; }
        constexpr bool operator!=(const mint &rhs) const { return m_val != rhs.m_val; }
        constexpr bool operator<(const mint &rhs) const { return m_val < rhs.m_val; }
        constexpr bool operator>(const mint &rhs) const { return m_val > rhs.m_val; }
        constexpr bool operator<=(const mint &rhs) const { return m_val <= rhs.m_val; }
        constexpr bool operator>=(const mint &rhs) const { return m_val <= rhs.m_val; }
        template <typename Tp>
        constexpr explicit operator Tp() const { return Tp(m_val); }
        friend CONSTEXPR14 mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend CONSTEXPR14 mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend CONSTEXPR14 mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend CONSTEXPR14 mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
    };
    template <typename Istream, typename ModType>
    Istream &operator>>(Istream &is, OverflowUnsigned<ModType> &x) { return is >> x.m_val; }
    template <typename Ostream, typename ModType>
    Ostream &operator<<(Ostream &os, const OverflowUnsigned<ModType> &x) { return os << x.val(); }
    using mintu32 = OverflowUnsigned<uint32_t>;
    using mintu64 = OverflowUnsigned<uint64_t>;
}

#endif