#ifndef __OY_STATICMONTGOMERYMODINT__
#define __OY_STATICMONTGOMERYMODINT__

#include "Montgomery.h"
#include <cstdint>
#include <functional>

namespace OY {
    template <typename _ModType, _ModType _P, bool _IsPrime = false>
    struct StaticMontgomeryModInt {
        using mint = StaticMontgomeryModInt<_ModType, _P, _IsPrime>;
        using _FastType = typename Montgomery<_ModType>::_FastType;
        using _LongType = typename Montgomery<_ModType>::_LongType;
        static constexpr Montgomery<_ModType> mg = Montgomery<_ModType>(_P);
        _FastType m_val;
        static_assert(_P % 2 == 1 && _P > 1 && _P < _MontgomeryTag<_ModType>::limit);
        StaticMontgomeryModInt() : m_val(0) {}
        template <typename _Tp, std::enable_if_t<std::is_signed_v<_Tp>> * = nullptr>
        StaticMontgomeryModInt(_Tp __val) {
            int64_t x = int64_t(__val) % int64_t(mod());
            if (x < 0) x += mod();
            m_val = mg.raw_init(x);
        }
        template <typename _Tp, std::enable_if_t<std::is_unsigned_v<_Tp>> * = nullptr>
        StaticMontgomeryModInt(_Tp __val) { m_val = mg.raw_init(__val % mod()); }
        static mint raw(_FastType __val) {
            mint res;
            res.m_val = __val;
            return res;
        }
        static constexpr _ModType mod() { return _P; }
        _ModType val() const { return mg.reduce(m_val); }
        mint pow(uint64_t __n) const { return raw(mg.pow(m_val, __n)); }
        mint inv() const {
            if constexpr (_IsPrime)
                return inv_Fermat();
            else
                return inv_exgcd();
        }
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
            (*this) += raw(mg.raw_init(1));
            return *this;
        }
        mint &operator--() {
            (*this) -= raw(mg.raw_init(1));
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
            m_val = mg.plus(m_val, __other.m_val);
            return *this;
        }
        mint &operator-=(const mint &__other) { return (*this) += -__other; }
        mint &operator*=(const mint &__other) {
            m_val = mg.multiply(m_val, __other.m_val);
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
        explicit operator _Tp() const { return _Tp(mg.reduce(m_val)); }
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
    template <uint32_t _P, bool _IsPrime>
    using StaticMontgomeryModInt32 = StaticMontgomeryModInt<uint32_t, _P, _IsPrime>;
    template <uint64_t _P, bool _IsPrime>
    using StaticMontgomeryModInt64 = StaticMontgomeryModInt<uint64_t, _P, _IsPrime>;
}

#endif