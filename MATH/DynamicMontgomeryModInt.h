#ifndef __OY_MONTGOMERYMODINT__
#define __OY_MONTGOMERYMODINT__

#include <cstdint>
#include <functional>
#include "Montgomery.h"

namespace OY {
    template <typename _ModType, _ModType _Id>
    struct DynamicMontgomeryModInt {
        using mint = DynamicMontgomeryModInt<_ModType, _Id>;
        using _FastType = typename Montgomery<_ModType>::_FastType;
        static inline Montgomery<_ModType> s_mg = Montgomery<_ModType>(_Id);
        static inline bool s_isPrime = true;
        _FastType m_val;
        constexpr DynamicMontgomeryModInt() = default;
        template <typename _Tp, std::enable_if_t<std::is_signed_v<_Tp>, bool> = true>
        constexpr DynamicMontgomeryModInt(_Tp __val) : m_val(0) {
            int64_t x = int64_t(__val) % int64_t(mod());
            if (x < 0) x += mod();
            m_val = s_mg.init(x);
        }
        template <typename _Tp, std::enable_if_t<std::is_unsigned_v<_Tp>, bool> = true>
        constexpr DynamicMontgomeryModInt(_Tp __val) : m_val(s_mg.init(__val)) {}
        static constexpr mint raw(_FastType __val) {
            mint res;
            res.m_val = __val;
            return res;
        }
        static constexpr void setMod(_ModType __P, bool __isPrime = false) {
            assert(__P % 2 == 1 && __P > 1 && __P <= _MontgomeryTag<_ModType>::limit);
            s_mg = Montgomery<_ModType>(__P);
            s_isPrime = __isPrime;
        }
        static constexpr _ModType mod() { return s_mg.mod(); }
        constexpr _ModType val() const { return s_mg.reduce(m_val); }
        constexpr mint pow(uint64_t __n) const { return raw(s_mg.pow(m_val, __n)); }
        constexpr mint inv() const { return s_isPrime ? inv_Fermat() : inv_exgcd(); }
        constexpr mint inv_exgcd() const {
            _ModType x = mod(), y = val(), m0 = 0, m1 = 1;
            while (y) {
                _ModType z = x / y;
                x -= y * z;
                m0 -= m1 * z;
                std::swap(x, y);
                std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return m0;
        }
        constexpr mint inv_Fermat() const { return pow(mod() - 2); }
        constexpr mint &operator++() {
            (*this) += raw(s_mg.raw_init(1));
            return *this;
        }
        constexpr mint &operator--() {
            (*this) += raw(s_mg.raw_init(mod() - 1));
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
        constexpr mint &operator+=(const mint &__other) {
            m_val = s_mg.plus(m_val, __other.m_val);
            return *this;
        }
        constexpr mint &operator-=(const mint &__other) {
            m_val = s_mg.minus(m_val, __other.m_val);
            return *this;
        }
        constexpr mint &operator*=(const mint &__other) {
            m_val = s_mg.multiply(m_val, __other.m_val);
            return *this;
        }
        constexpr mint &operator/=(const mint &__other) { return *this *= __other.inv(); }
        constexpr mint operator+() const { return *this; }
        constexpr mint operator-() const { return raw(m_val ? mod() - m_val : 0); }
        constexpr bool operator==(const mint &__other) const { return m_val == __other.m_val; }
        constexpr bool operator!=(const mint &__other) const { return m_val != __other.m_val; }
        constexpr bool operator<(const mint &__other) const { return m_val < __other.m_val; }
        constexpr bool operator>(const mint &__other) const { return m_val > __other.m_val; }
        constexpr bool operator<=(const mint &__other) const { return m_val <= __other.m_val; }
        constexpr bool operator>=(const mint &__other) const { return m_val <= __other.m_val; }
        template <typename _Tp>
        constexpr explicit operator _Tp() const { return _Tp(s_mg.reduce(m_val)); }
        constexpr friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        constexpr friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        constexpr friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        constexpr friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
        template <typename _Istream>
        friend _Istream &operator>>(_Istream &is, mint &self) {
            _ModType x;
            is >> x;
            self = mint(x);
            return is;
        }
        template <typename _Ostream>
        friend _Ostream &operator<<(_Ostream &os, const mint &self) { return os << self.val(); }
    };
    template <uint32_t _Id>
    using DynamicMontgomeryModInt32 = DynamicMontgomeryModInt<uint32_t, _Id>;
    template <uint64_t _Id>
    using DynamicMontgomeryModInt64 = DynamicMontgomeryModInt<uint64_t, _Id>;
}

#endif