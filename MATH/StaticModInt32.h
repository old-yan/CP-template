/*
最后修改:
20240329
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STATICMODINT32__
#define __OY_STATICMODINT32__

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
    template <uint32_t P, bool IsPrime, typename = typename std::enable_if<(P > 1 && P < uint32_t(1) << 31)>::type>
    struct StaticModInt32 {
        using mint = StaticModInt32<P, IsPrime>;
        using mod_type = uint32_t;
        mod_type m_val;
        static constexpr mod_type _reduce_norm(int32_t x) { return x < 0 ? x + mod() : x; }
        static constexpr mod_type _mul(mod_type a, mod_type b) { return uint64_t(a) * b % mod(); }
        constexpr StaticModInt32() = default;
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        constexpr StaticModInt32(Tp val) : m_val(_reduce_norm(val % int32_t(mod()))) {}
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        constexpr StaticModInt32(Tp val) : m_val(val % mod()) {}
        static CONSTEXPR14 mint raw(mod_type val) {
            mint res{};
            res.m_val = val;
            return res;
        }
        static constexpr mod_type mod() { return P; }
        constexpr mod_type val() const { return m_val; }
        CONSTEXPR14 mint pow(uint64_t n) const {
            mod_type res = 1, b = m_val;
            while (n) {
                if (n & 1) res = _mul(res, b);
                b = _mul(b, b), n >>= 1;
            }
            return raw(res);
        }
        CONSTEXPR14 mint inv() const {
            if constexpr (IsPrime)
                return inv_Fermat();
            else
                return inv_exgcd();
        }
        CONSTEXPR14 mint inv_exgcd() const {
            mod_type x = mod(), y = m_val, m0 = 0, m1 = 1;
            while (y) {
                mod_type z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return raw(m0);
        }
        constexpr mint inv_Fermat() const { return pow(mod() - 2); }
        CONSTEXPR14 mint &operator++() {
            if (++m_val == mod()) m_val = 0;
            return *this;
        }
        CONSTEXPR14 mint &operator--() {
            if (!m_val) m_val = mod();
            m_val--;
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
            if (m_val >= mod()) m_val -= mod();
            return *this;
        }
        CONSTEXPR14 mint &operator-=(const mint &rhs) {
            m_val += mod() - rhs.m_val;
            if (m_val >= mod()) m_val -= mod();
            return *this;
        }
        CONSTEXPR14 mint &operator*=(const mint &rhs) {
            m_val = _mul(m_val, rhs.m_val);
            return *this;
        }
        CONSTEXPR14 mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        constexpr mint operator+() const { return *this; }
        constexpr mint operator-() const { return raw(m_val ? mod() - m_val : 0); }
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
    template <typename Istream, uint32_t P, bool IsPrime>
    Istream &operator>>(Istream &is, StaticModInt32<P, IsPrime> &x) { return is >> x.m_val; }
    template <typename Ostream, uint32_t P, bool IsPrime>
    Ostream &operator<<(Ostream &os, const StaticModInt32<P, IsPrime> &x) { return os << x.val(); }
    using mint998244353 = StaticModInt32<998244353, true>;
    using mint1000000007 = StaticModInt32<1000000007, true>;
}

#endif