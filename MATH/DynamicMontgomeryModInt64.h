/*
最后修改:
20240316
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
        struct Info {
            mod_type m_mod, m_mod2, m_pinv, m_ninv;
            fast_type m_one;
            uint64_t m_inv;
            bool m_is_prime;
            void set_mod(mod_type P, bool is_prime = false) {
                assert(P % 2 && P > 1 && P < mod_type(1) << 62);
#ifdef _MSC_VER
                mod_type high, low;
                low = _umul128(-P % P, -P % P, &high);
                _udiv128(high, low, P, &m_ninv);
#else
                m_ninv = -long_type(P) % P;
#endif
                m_mod = m_pinv = P, m_mod2 = P * 2, m_inv = uint64_t(-1) / P + 1, m_is_prime = is_prime;
                for (size_t i = 0; i != 5; i++) m_pinv *= mod_type(2) - mod() * m_pinv;
                m_pinv = -m_pinv, m_one = strict_reduce(raw_init(1));
            }
            mod_type mod(uint64_t val) const {
#ifdef _MSC_VER
                uint64_t res;
                _umul128(val, m_inv, &res);
                res = val - res * mod();
#else
                mod_type res = val - uint64_t((__uint128_t(val) * m_inv) >> 64) * mod();
#endif
                if (res >= mod()) res += mod();
                return res;
            }
            mod_type mod() const { return m_mod; }
#ifdef _MSC_VER
            fast_type reduce(mod_type high, mod_type low) const {
                uint64_t _, res, low2 = _umul128(_umul128(low, m_pinv, &_), mod(), &res);
                return res + high + (low2 > -low || (low2 == -low && low));
#else
            fast_type reduce(long_type val) const {
                return (val + long_type(mod_type(val) * m_pinv) * mod()) >> 64;
#endif
            }
            fast_type reduce_zero(mod_type x) const { return x == mod() ? 0 : x; }
            fast_type strict_reduce(fast_type val) const { return val >= mod() ? val - mod() : val; }
            fast_type add(fast_type a, fast_type b) const {
                fast_type val1 = a + b, val2 = val1 - m_mod2;
                return int64_t(val2) < 0 ? val1 : val2;
            }
            fast_type sub(fast_type a, fast_type b) const {
                fast_type val1 = a + m_mod2 - b, val2 = val1 - m_mod2;
                return int64_t(val2) < 0 ? val1 : val2;
            }
            fast_type neg(fast_type val) const { return val ? m_mod2 - val : 0; }
            fast_type mul(fast_type a, fast_type b) const {
#ifdef _MSC_VER
                mod_type high, low;
                low = _umul128(a, b, &high);
                return reduce(high, low);
#else
                return reduce(long_type(a) * b);
#endif
            }
            fast_type pow(fast_type a, uint64_t n) const {
                fast_type res = one(), b = a;
                while (n) {
                    if (n & 1) res = mul(res, b);
                    b = mul(b, b), n >>= 1;
                }
                return res;
            }
            fast_type one() const { return m_one; }
            fast_type raw_init(mod_type val) const { return mul(val, m_ninv); }
            mod_type val(fast_type _val) const {
#ifdef _MSC_VER
                return reduce_zero(reduce(0, _val));
#else
                return reduce_zero(reduce(_val));
#endif
            }
            bool is_prime() const { return m_is_prime; }
        };
        static Info s_info;
        fast_type m_val;
        static mod_type _mod(uint64_t val) { return s_info.mod(val); }
        static fast_type _init(uint64_t val) { return _raw_init(_mod(val)); }
        static fast_type _raw_init(mod_type val) { return s_info.raw_init(val); }
        static fast_type _mul(fast_type a, fast_type b) { return s_info.mul(a, b); }
        DynamicMontgomeryModInt64() = default;
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        DynamicMontgomeryModInt64(Tp val) {
            auto x = val % int64_t(mod());
            if (x < 0) x += mod();
            m_val = _raw_init(x);
        }
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        DynamicMontgomeryModInt64(Tp val) : m_val{val < mod() ? _raw_init(val) : _init(val)} {}
        static mint _raw(fast_type val) {
            mint res;
            res.m_val = val;
            return res;
        }
        static mint raw(mod_type val) { return _raw(_raw_init(val)); }
        static void set_mod(mod_type P, bool is_prime = false) { s_info.set_mod(P, is_prime); }
        static mod_type mod() { return s_info.mod(); }
        mod_type val() const { return s_info.val(m_val); }
        mint pow(uint64_t n) const { return _raw(s_info.pow(m_val, n)); }
        mint inv() const { return s_info.is_prime() ? inv_Fermat() : inv_exgcd(); }
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
            (*this) += _raw(s_info.one());
            return *this;
        }
        mint &operator--() {
            (*this) -= _raw(s_info.one());
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
            m_val = s_info.add(m_val, rhs.m_val);
            return *this;
        }
        mint &operator-=(const mint &rhs) {
            m_val = s_info.sub(m_val, rhs.m_val);
            return *this;
        }
        mint &operator*=(const mint &rhs) {
            m_val = _mul(m_val, rhs.m_val);
            return *this;
        }
        mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        mint operator+() const { return *this; }
        mint operator-() const { return _raw(s_info.neg(m_val)); }
        bool operator==(const mint &rhs) const { return s_info.strict_reduce(m_val) == s_info.strict_reduce(rhs.m_val); }
        bool operator!=(const mint &rhs) const { return s_info.strict_reduce(m_val) != s_info.strict_reduce(rhs.m_val); }
        bool operator<(const mint &rhs) const { return s_info.strict_reduce(m_val) < s_info.strict_reduce(rhs.m_val); }
        bool operator>(const mint &rhs) const { return s_info.strict_reduce(m_val) > s_info.strict_reduce(rhs.m_val); }
        bool operator<=(const mint &rhs) const { return s_info.strict_reduce(m_val) <= s_info.strict_reduce(rhs.m_val); }
        bool operator>=(const mint &rhs) const { return s_info.strict_reduce(m_val) >= s_info.strict_reduce(rhs.m_val); }
        template <typename Tp>
        explicit operator Tp() const { return Tp(val()); }
        friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
    };
    template <size_t Id>
    typename DynamicMontgomeryModInt64<Id>::Info DynamicMontgomeryModInt64<Id>::s_info;
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