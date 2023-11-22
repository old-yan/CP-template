/*
最后修改:
20231120
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DYNAMICMONTGOMERYMODINT64__
#define __OY_DYNAMICMONTGOMERYMODINT64__

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>

#ifdef _MSC_VER
#include <immintrin.h>
#endif

namespace OY {
    template <size_t Id>
    struct DynamicMontgomeryModInt64 {
        using mint = DynamicMontgomeryModInt64<Id>;
        using mod_type = uint64_t;
        using fast_type = mod_type;
#ifndef _MSC_VER
        using long_type = __uint128_t;
#endif
        static mod_type s_mod, s_pinv, s_ninv;
        static uint64_t s_inv;
        static bool s_is_prime;
        fast_type m_val;
        static mod_type _mod(uint64_t val) {
#ifdef _MSC_VER
            mod_type res;
            _umul128(val, s_inv, &res);
            res = val - res * mod();
#else
            mod_type res = val - uint64_t((__uint128_t(val) * s_inv) >> 64) * mod();
#endif
            if (res >= mod()) res += mod();
            return res;
        }
        static fast_type _init(mod_type val) { return _raw_init(_mod(val)); }
        static fast_type _raw_init(mod_type val) { return _mul(val, s_ninv); }
        static mod_type _reduce(fast_type val) {
#ifdef _MSC_VER
            uint64_t _, res;
            _umul128(_umul128(val, s_pinv, &_), mod(), &res);
            res = -res;
#else
            mod_type res = -mod_type((long_type(val * s_pinv) * mod()) >> 64);
#endif
            if (res >= mod()) res += mod();
            return res;
        }
#ifdef _MSC_VER
        static fast_type _reduce_long(mod_type high, mod_type low) {
            uint64_t _, res;
            _umul128(_umul128(low, s_pinv, &_), mod(), &res);
            res = high - res;
#else
        static fast_type _reduce_long(long_type val) {
            fast_type res = (val >> 64) - mod_type((long_type(mod_type(val) * s_pinv) * mod()) >> 64);
#endif
            if (res >= mod()) res += mod();
            return res;
        }
        static fast_type _mul(fast_type a, fast_type b) {
#ifdef _MSC_VER
            mod_type high, low;
            low = _umul128(a, b, &high);
            return _reduce_long(high, low);
#else
            return _reduce_long(long_type(a) * b);
#endif
        }
        DynamicMontgomeryModInt64() : m_val{} {}
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        DynamicMontgomeryModInt64(Tp val) : m_val{} {
            auto x = val % int64_t(mod());
            if (x < 0) x += mod();
            m_val = _raw_init(x);
        }
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        DynamicMontgomeryModInt64(Tp val) : m_val{_init(val)} {}
        static mint _raw(fast_type val) {
            mint res;
            res.m_val = val;
            return res;
        }
        static mint raw(mod_type val) { return _raw(_raw_init(val)); }
        static void set_mod(mod_type P, bool is_prime = false) {
            assert(P % 2 && P > 1 && P < mod_type(1) << 63);
#ifdef _MSC_VER
            mod_type high, low;
            low = _umul128(-P % P, -P % P, &high);
            _udiv128(high, low, P, &s_ninv);
#else
            s_ninv = -long_type(P) % P;
#endif
            s_mod = s_pinv = P, s_inv = uint64_t(-1) / P + 1, s_is_prime = is_prime;
            for (size_t i = 0; i != 5; i++) s_pinv *= mod_type(2) - mod() * s_pinv;
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
    uint64_t DynamicMontgomeryModInt64<Id>::s_mod;
    template <size_t Id>
    uint64_t DynamicMontgomeryModInt64<Id>::s_pinv;
    template <size_t Id>
    uint64_t DynamicMontgomeryModInt64<Id>::s_ninv;
    template <size_t Id>
    uint64_t DynamicMontgomeryModInt64<Id>::s_inv;
    template <size_t Id>
    bool DynamicMontgomeryModInt64<Id>::s_is_prime;
    template <typename Istream, size_t Id>
    Istream &operator>>(Istream &is, DynamicMontgomeryModInt64<Id> &x) {
        uint64_t val;
        is >> val;
        x.m_val = DynamicMontgomeryModInt64<Id>::_raw_init(val);
        return is;
    }
    template <typename Ostream, size_t Id>
    Ostream &operator<<(Ostream &os, const DynamicMontgomeryModInt64<Id> &x) { return os << x.val(); }
}

#endif