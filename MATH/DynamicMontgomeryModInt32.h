/*
最后修改:
20231120
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DYNAMICMONTGOMERYMODINT32__
#define __OY_DYNAMICMONTGOMERYMODINT32__

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>

namespace OY {
    template <size_t Id>
    struct DynamicMontgomeryModInt32 {
        using mint = DynamicMontgomeryModInt32<Id>;
        using mod_type = uint32_t;
        using fast_type = mod_type;
        using long_type = uint64_t;
        static mod_type s_mod, s_pinv, s_ninv;
        static uint64_t s_inv;
        static bool s_is_prime;
        fast_type m_val;
        static mod_type _mod(uint64_t val) {
#ifdef _MSC_VER
            uint64_t x;
            _umul128(val, s_inv, &x);
            mod_type res = val - x * mod();
#else
            mod_type res = val - uint64_t((__uint128_t(val) * s_inv) >> 64) * mod();
#endif
            if (res >= mod()) res += mod();
            return res;
        }
        static fast_type _init(uint64_t val) { return _raw_init(_mod(val)); }
        static fast_type _raw_init(mod_type val) { return _mul(val, s_ninv); }
        static mod_type _reduce(fast_type val) {
            mod_type res = -mod_type((long_type(val * s_pinv) * mod()) >> 32);
            if (res >= mod()) res += mod();
            return res;
        }
        static fast_type _reduce_long(long_type val) {
            fast_type res = (val >> 32) - mod_type((long_type(mod_type(val) * s_pinv) * mod()) >> 32);
            if (res >= mod()) res += mod();
            return res;
        }
        static fast_type _mul(fast_type a, fast_type b) { return _reduce_long(long_type(a) * b); }
        DynamicMontgomeryModInt32() : m_val{} {}
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        DynamicMontgomeryModInt32(Tp val) : m_val{} {
            auto x = val % int32_t(mod());
            if (x < 0) x += mod();
            m_val = _raw_init(x);
        }
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        DynamicMontgomeryModInt32(Tp val) : m_val{_init(val)} {}
        static mint _raw(fast_type val) {
            mint res;
            res.m_val = val;
            return res;
        }
        static mint raw(mod_type val) { return _raw(_raw_init(val)); }
        static void set_mod(mod_type P, bool is_prime = false) {
            assert(P % 2 && P > 1 && P < mod_type(1) << 31);
            s_mod = s_pinv = P, s_ninv = -long_type(P) % P, s_inv = uint64_t(-1) / P + 1, s_is_prime = is_prime;
            for (size_t i = 0; i != 4; i++) s_pinv *= mod_type(2) - mod() * s_pinv;
        }
        static mod_type mod() { return s_mod; }
        mod_type val() const { return _reduce(m_val); }
        mint pow(uint64_t n) const {
            fast_type res = _raw_init(1), b = m_val;
            while (n) {
                if (n & 1) res = _mul(res, b);
                b = _mul(b, b), n >>= 1;
            }
            return _raw(res);
        }
        mint inv() const { return s_is_prime ? inv_Fermat() : inv_exgcd(); }
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
            m_val += rhs.m_val;
            if (m_val >= mod()) m_val -= mod();
            return *this;
        }
        mint &operator-=(const mint &rhs) {
            m_val += mod() - rhs.m_val;
            if (m_val >= mod()) m_val -= mod();
            return *this;
        }
        mint &operator*=(const mint &rhs) {
            m_val = _mul(m_val, rhs.m_val);
            return *this;
        }
        mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        mint operator+() const { return *this; }
        mint operator-() const { return _raw(m_val ? mod() - m_val : 0); }
        bool operator==(const mint &rhs) const { return m_val == rhs.m_val; }
        bool operator!=(const mint &rhs) const { return m_val != rhs.m_val; }
        bool operator<(const mint &rhs) const { return m_val < rhs.m_val; }
        bool operator>(const mint &rhs) const { return m_val > rhs.m_val; }
        bool operator<=(const mint &rhs) const { return m_val <= rhs.m_val; }
        bool operator>=(const mint &rhs) const { return m_val <= rhs.m_val; }
        template <typename Tp>
        explicit operator Tp() const { return Tp(_reduce(m_val)); }
        friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
    };
    template <size_t Id>
    uint32_t DynamicMontgomeryModInt32<Id>::s_mod;
    template <size_t Id>
    uint32_t DynamicMontgomeryModInt32<Id>::s_pinv;
    template <size_t Id>
    uint32_t DynamicMontgomeryModInt32<Id>::s_ninv;
    template <size_t Id>
    uint64_t DynamicMontgomeryModInt32<Id>::s_inv;
    template <size_t Id>
    bool DynamicMontgomeryModInt32<Id>::s_is_prime;
    template <typename Istream, size_t Id>
    Istream &operator>>(Istream &is, DynamicMontgomeryModInt32<Id> &x) {
        uint32_t val;
        is >> val;
        x.m_val = DynamicMontgomeryModInt32<Id>::_raw_init(val);
        return is;
    }
    template <typename Ostream, size_t Id>
    Ostream &operator<<(Ostream &os, const DynamicMontgomeryModInt32<Id> &x) { return os << x.val(); }
}

#endif