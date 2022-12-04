#ifndef __OY_BIGFLOAT__
#define __OY_BIGFLOAT__

#include "BigInt.h"

namespace OY {
    template <uint32_t _B = 10, uint32_t _W = 6, uint32_t _MAXN = 1 << 6, uint64_t _P = 9223372006790004737, uint64_t _R = 3>
    struct BigFloat {
        using bint = BigInt<_B, _W, _MAXN * 4, _P, _R>;
        using bfloat = BigFloat<_B, _W, _MAXN, _P, _R>;
        static inline int _K = (_MAXN + _W - 1) / _W;
        int m_shift;
        bint m_number;
        BigFloat() : m_shift{}, m_number{} {}
        template <typename _Tp, std::enable_if_t<std::is_integral_v<_Tp> | std::is_same_v<_Tp, __int128_t>, bool> = true>
        explicit BigFloat(_Tp __number) : m_shift(0), m_number(__number) { shrink(); }
        template <typename _Tp, std::enable_if_t<std::is_floating_point_v<_Tp>, bool> = true>
        explicit BigFloat(_Tp __number) : m_shift(std::ceil((std::log(std::abs(__number)) - std::log(_Tp(INT64_MAX))) / std::log(_Tp(_B)))), m_number(int64_t(__number / std::pow(_B, m_shift))) { shrink(); }
        explicit BigFloat(const char *__number) { _fillWithString(*this, __number, std::strlen(__number)); }
        explicit BigFloat(const std::string &__number) { _fillWithString(*this, __number.data(), __number.size()); }
        BigFloat(bint &&__number, int __shift = 0) : m_shift(__shift), m_number(std::move(__number)) { shrink(); }
        BigFloat(const bint &__number, int __shift = 0) : m_shift(__shift), m_number(__number) { shrink(); }
        static void _fillWithString(bfloat &__a, const char *__number, uint32_t __length) {
            uint32_t dot = __length;
            while (~--dot && __number[dot] != '.') {}
            if (!~dot) {
                bint::_fillWithString(__a.m_number, __number, __length);
                __a.m_shift = 0;
            } else {
                char buffer[__length - 1];
                std::copy_n(__number + dot + 1, __length - dot - 1, std::copy_n(__number, dot, buffer));
                bint::_fillWithString(__a.m_number, buffer, __length - 1);
                __a.m_shift = 1 + dot - __length;
            }
            __a.shrink();
        }
        template <typename _Operation>
        static bfloat _plus_minus(const bfloat &__a, const bfloat &__b) {
            if (__a.length() > __b.length() + int(_K * _W)) return __a;
            return bfloat(_Operation()(__a.m_number << (__a.m_shift - __b.m_shift), __b.m_number), __b.m_shift);
        }
        static bfloat rand(uint32_t __integerLength) {
            uint32_t length = (bint::s_rander() % _K + 1) * _W;
            return bfloat(bint::rand(length), __integerLength - length);
        }
        static int absCompare(const bfloat &__a, const bfloat &__b) {
            if (!__a.m_number.m_length) return __b.m_number.m_length ? -1 : 0;
            if (!__b.m_number.m_length) return 1;
            if (int alen = __a.length(), blen = __b.length(); alen != blen) return alen > blen ? 1 : -1;
            for (uint32_t i = __a.m_number.m_length - 1, j = __b.m_number.m_length - 1; ~i || ~j; i--, j--) {
                if (!~i) return -1;
                if (!~j) return 1;
                if (__a.m_number.m_data[i] != __b.m_number.m_data[j]) return __a.m_number.m_data[i] > __b.m_number.m_data[j] ? 1 : -1;
            }
            return 0;
        }
        bfloat &shrink() {
            if (!m_number.m_length) {
                m_shift = 0;
                return *this;
            }
            int i = 0, j = m_number.length() - (m_number.m_length - 1) * _W;
            while (i < m_number.m_length && !m_number.m_data[i]) i++;
            if (int k = !(m_number.m_data[i] % bint::bases.val[j]); m_number.m_length - i - k > _K)
                i = (m_number.m_length - 1) * _W + j - _K * _W;
            else
                i = (i - 1 + k) * _W + j;
            if (i > 0)
                m_shift += i, m_number >>= i;
            else if (i < 0)
                m_shift += i, m_number <<= -i;
            return *this;
        }
        bfloat &opposite() {
            m_number.setSign(!m_number.m_negative);
            return *this;
        }
        bfloat &setSign(bool __negative) {
            m_number.m_negative = m_number.m_length ? __negative : false;
            return *this;
        }
        int length() const { return m_number.m_length * _W + m_shift - 1; }
        bfloat pow(uint64_t __n) const {
            bfloat res(1), b(*this);
            while (__n) {
                if (__n & 1) res *= b;
                if (__n >>= 1) b *= b;
            }
            return res;
        }
        bfloat sqrt(uint32_t __n) const {
            if (__n == 1) return *this;
            bfloat res(std::pow((long double)(*this), (long double)1 / __n));
            uint32_t oldK = _K;
            for (_K = 2; _K < oldK; _K *= 2) res = res * (__n - 1) / __n + (*this) / (res.pow(__n - 1) * __n);
            _K = oldK;
            return res;
        }
        bfloat &operator+=(const bfloat &__other) { return *this = *this + __other; }
        bfloat &operator-=(const bfloat &__other) { return *this = *this - __other; }
        bfloat &operator*=(const bfloat &__other) {
            m_number *= __other.m_number;
            m_shift += __other.m_shift;
            shrink();
            return *this;
        }
        bfloat &operator*=(long long __other) {
            m_number *= __other;
            shrink();
            return *this;
        }
        bfloat &operator/=(const bfloat &__other) { return *this = *this / __other; }
        bfloat &operator/=(long long __other) { return *this = *this / __other; }
        bfloat &operator<<=(uint32_t __shift) {
            if (m_number.m_length) m_shift += __shift;
            return *this;
        }
        bfloat &operator>>=(uint32_t __shift) {
            if (m_number.m_length) m_shift -= __shift;
            return *this;
        }
        bfloat operator+() const { return *this; }
        bfloat operator-() const {
            bfloat res(*this);
            res.opposite();
            return res;
        }
        bool operator==(const bfloat &__other) const = default;
        bool operator!=(const bfloat &__other) const = default;
        bool operator<(const bfloat &__other) const { return m_number.m_negative == __other.m_number.m_negative ? (m_number.m_negative ? absCompare(*this, __other) > 0 : absCompare(*this, __other) < 0) : m_number.m_negative; }
        bool operator>(const bfloat &__other) const { return m_number.m_negative == __other.m_number.m_negative ? (m_number.m_negative ? absCompare(*this, __other) < 0 : absCompare(*this, __other) > 0) : !m_number.m_negative; }
        bool operator<=(const bfloat &__other) const { return m_number.m_negative == __other.m_number.m_negative ? (m_number.m_negative ? absCompare(*this, __other) >= 0 : absCompare(*this, __other) <= 0) : m_number.m_negative; }
        bool operator>=(const bfloat &__other) const { return m_number.m_negative == __other.m_number.m_negative ? (m_number.m_negative ? absCompare(*this, __other) <= 0 : absCompare(*this, __other) >= 0) : !m_number.m_negative; }
        explicit operator bool() const { return m_number.m_length; }
        template <typename _Tp, std::enable_if_t<std::is_integral_v<_Tp>, bool> = true>
        explicit operator _Tp() const { return m_shift ? (_Tp(m_shift > 0 ? m_number << m_shift : m_number >> -m_shift)) : _Tp(m_number); }
        template <typename _Tp, std::enable_if_t<std::is_floating_point_v<_Tp>, bool> = true>
        explicit operator _Tp() const { return _Tp(m_number) * std::pow(_Tp(_B), _Tp(m_shift)); }
        operator std::string() const {
            if (!m_number.m_length) return "0";
            std::string res(m_number);
            if (m_shift >= 0) {
                res += std::string(m_shift, '0');
                return res;
            }
            std::reverse(res.begin(), res.end());
            if (m_number.m_negative) res.pop_back();
            if (res.size() <= -m_shift) res.resize(1 - m_shift, '0');
            res.insert(res.begin() - m_shift, '.');
            if (m_number.m_negative) res.push_back('-');
            std::reverse(res.begin(), res.end());
            while (res.back() == '0') res.pop_back();
            if (res.back() == '.') res.pop_back();
            return res;
        }
        friend bfloat operator+(const bfloat &__a, const bfloat &__b) {
            if (!__a.m_number.m_length) return __b;
            if (!__b.m_number.m_length) return __a;
            if (__a.m_shift == __b.m_shift)
                return bfloat(__a.m_number + __b.m_number, __a.m_shift);
            else if (__a.m_shift > __b.m_shift)
                return _plus_minus<std::plus<bint>>(__a, __b);
            else
                return _plus_minus<std::plus<bint>>(__b, __a);
        }
        friend bfloat operator-(const bfloat &__a, const bfloat &__b) {
            if (!__a.m_number.m_length) return -__b;
            if (!__b.m_number.m_length) return __a;
            if (__a.m_shift == __b.m_shift)
                return bfloat(__a.m_number - __b.m_number, __a.m_shift);
            else if (__a.m_shift > __b.m_shift)
                return _plus_minus<std::minus<bint>>(__a, __b);
            else {
                bfloat res = _plus_minus<std::minus<bint>>(__b, __a);
                res.opposite();
                return res;
            }
        }
        friend bfloat operator*(const bfloat &__a, const bfloat &__b) { return bfloat(__a.m_number * __b.m_number, __a.m_shift + __b.m_shift); }
        friend bfloat operator*(const bfloat &__a, long long __b) { return bfloat(__a.m_number * __b, __a.m_shift); }
        friend bfloat operator/(const bfloat &__a, const bfloat &__b) {
            if (!__a.m_number.m_length) return __a;
            int a_shift = (_K * 2 - __a.m_number.m_length) * _W, b_shift = (_K - __b.m_number.m_length) * _W;
            return bfloat((a_shift >= 0 ? __a.m_number << a_shift : __a.m_number >> -a_shift) / (b_shift >= 0 ? __b.m_number << b_shift : __b.m_number >> -b_shift), __a.m_shift - a_shift - __b.m_shift + b_shift);
        }
        friend bfloat operator/(const bfloat &__a, long long __b) {
            if (!__a.m_number.m_length) return __a;
            int a_up = (_K * 2 - __a.m_number.m_length) * _W;
            return bfloat((__a.m_number << a_up) / __b, __a.m_shift - a_up);
        }
        friend bfloat operator<<(const bfloat &__a, uint32_t __shift) { return __a.m_number.m_length && __shift ? bfloat(__a.m_number, __a.m_shift + __shift) : __a; }
        friend bfloat operator>>(const bfloat &__a, uint32_t __shift) { return __a.m_number.m_length && __shift ? bfloat(__a.m_number, __a.m_shift - __shift) : __a; }
        template <typename _Istream>
        friend _Istream &operator>>(_Istream &is, bfloat &self) {
            std::string number;
            is >> number;
            if (self.m_number.m_length) bint::_free(self.m_number.m_data);
            _fillWithString(self, number.data(), number.size());
            return is;
        }
        template <typename _Ostream>
        friend _Ostream &operator<<(_Ostream &os, const bfloat &self) { return os << std::string(self); }
    };
}

#endif