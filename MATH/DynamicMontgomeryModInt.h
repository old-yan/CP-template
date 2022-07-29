#ifndef __OY_MONTGOMERYMODINT__
#define __OY_MONTGOMERYMODINT__

#include <cstdint>
#include <functional>
#include "Montgomery.h"

namespace OY {
    template <typename _ModType, int _Id>
    struct DynamicMontgomeryModInt {
        using mint = DynamicMontgomeryModInt<_ModType, _Id>;
        using _FastType = typename Montgomery<_ModType>::_FastType;
        static inline Montgomery<_ModType> s_mg;
        static inline bool s_isPrime = true;
        _FastType m_val;
        DynamicMontgomeryModInt() : m_val(0) {}
        template <typename _Tp, std::enable_if_t<std::is_signed_v<_Tp>> * = nullptr>
        DynamicMontgomeryModInt(_Tp __val) {
            int64_t x = int64_t(__val) % int64_t(mod());
            if (x < 0) x += mod();
            m_val = s_mg.init(x);
        }
        template <typename _Tp, std::enable_if_t<std::is_unsigned_v<_Tp>> * = nullptr>
        DynamicMontgomeryModInt(_Tp __val) { m_val = s_mg.init(__val); }
        static mint raw(_FastType __val) {
            mint res;
            res.m_val = __val;
            return res;
        }
        static void setMod(_ModType __P, bool __isPrime = false) {
            assert(__P % 2 == 1 && __P > 1 && __P < _MontgomeryTag<_ModType>::limit);
            s_mg = Montgomery<_ModType>(__P);
            s_isPrime = __isPrime;
        }
        static _ModType mod() { return s_mg.mod(); }
        _ModType val() const { return s_mg.reduce(m_val); }
        mint pow(uint64_t __n) const { return raw(s_mg.pow(m_val, __n)); }
        mint inv() const { return s_isPrime ? inv_Fermat() : inv_exgcd(); }
        mint inv_exgcd() const {
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
        mint inv_Fermat() const { return pow(mod() - 2); }
        mint &operator++() {
            (*this) += raw(s_mg.raw_init(1));
            return *this;
        }
        mint &operator--() {
            (*this) += raw(s_mg.raw_init(mod() - 1));
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
        mint &operator+=(const mint &__other) {
            m_val = s_mg.plus(m_val, __other.m_val);
            return *this;
        }
        mint &operator-=(const mint &__other) {
            m_val = s_mg.minus(m_val, __other.m_val);
            return *this;
        }
        mint &operator*=(const mint &__other) {
            m_val = s_mg.multiply(m_val, __other.m_val);
            return *this;
        }
        mint &operator/=(const mint &__other) { return *this *= __other.inv(); }
        mint operator+() const { return *this; }
        mint operator-() const { return raw(m_val ? mod() - m_val : 0); }
        bool operator==(const mint &__other) const { return m_val == __other.m_val; }
        bool operator!=(const mint &__other) const { return m_val != __other.m_val; }
        bool operator<(const mint &__other) const { return m_val < __other.m_val; }
        bool operator>(const mint &__other) const { return m_val > __other.m_val; }
        bool operator<=(const mint &__other) const { return m_val <= __other.m_val; }
        bool operator>=(const mint &__other) const { return m_val <= __other.m_val; }
        template <typename _Tp>
        explicit operator _Tp() const { return _Tp(s_mg.reduce(m_val)); }
        friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
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
    template <int _Id>
    using DynamicMontgomeryModInt32 = DynamicMontgomeryModInt<uint32_t, _Id>;
    template <int _Id>
    using DynamicMontgomeryModInt64 = DynamicMontgomeryModInt<uint64_t, _Id>;
}

#endif