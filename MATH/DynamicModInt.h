#ifndef __OY_DYNAMICMODINT__
#define __OY_DYNAMICMODINT__

#include "Barrett.h"
#include <cassert>
#include <cstdint>
#include <functional>

namespace OY {
    template <typename _ModType, int _Id>
    struct DynamicModInt {
        using mint = DynamicModInt<_ModType, _Id>;
        _ModType m_val;
        static inline Barrett<_ModType> s_brt;
        static inline bool s_isPrime = true;
        DynamicModInt() : m_val(0) {}
        template <typename _Tp, std::enable_if_t<std::is_signed_v<_Tp>> * = nullptr>
        DynamicModInt(_Tp __val) {
            int64_t x = int64_t(__val) % int64_t(mod());
            if (x < 0) x += mod();
            m_val = x;
        }
        template <typename _Tp, std::enable_if_t<std::is_unsigned_v<_Tp>> * = nullptr>
        DynamicModInt(_Tp __val) { m_val = s_brt.mod(__val); }
        static mint raw(_ModType __val) {
            mint res;
            res.m_val = __val;
            return res;
        }
        static void setMod(_ModType __P, bool __isPrime = false) {
            assert(__P > 1 && __P < 1ull << 63);
            s_brt = Barrett<_ModType>(__P);
            s_isPrime = __isPrime;
        }
        static _ModType mod() { return s_brt.mod(); }
        _ModType val() const { return m_val; }
        mint pow(uint64_t __n) const { return s_brt.pow(m_val, __n); }
        mint inv() const { return s_isPrime ? inv_Fermat() : inv_exgcd(); }
        mint inv_exgcd() const {
            _ModType x = mod(), y = m_val, m0 = 0, m1 = 1;
            while (y) {
                _ModType z = x / y;
                x -= y * z;
                m0 -= m1 * z;
                std::swap(x, y);
                std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return raw(m0);
        }
        mint inv_Fermat() const { return pow(mod() - 2); }
        mint &operator++() {
            if (++m_val == mod()) m_val = 0;
            return *this;
        }
        mint &operator--() {
            if (m_val-- == 0) m_val = mod() - 1;
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
            m_val = s_brt.plus(m_val, __other.m_val);
            return *this;
        }
        mint &operator-=(const mint &__other) {
            m_val = s_brt.minus(m_val, __other.m_val);
            return *this;
        }
        mint &operator*=(const mint &__other) {
            m_val = s_brt.multiply(m_val, __other.m_val);
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
        explicit operator _Tp() const { return _Tp(m_val); }
        friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
        template <typename _Istream>
        friend _Istream &operator>>(_Istream &is, mint &self) { return is >> self.m_val; }
        template <typename _Ostream>
        friend _Ostream &operator<<(_Ostream &os, const mint &self) { return os << self.m_val; }
    };
    template <int _Id>
    using DynamicModInt32 = DynamicModInt<uint32_t, _Id>;
    template <int _Id>
    using DynamicModInt64 = DynamicModInt<uint64_t, _Id>;
}

#endif