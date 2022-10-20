#ifndef __OY_FFTPOLYNOMIAL__
#define __OY_FFTPOLYNOMIAL__

#include <algorithm>
#include <bit>
#include <cstdint>

namespace OY {
    template <typename _Tp = uint32_t, uint32_t _MAXN = 1 << 22, typename _Fp = double>
    struct FFTPolynomial : std::vector<_Tp> {
        using poly = FFTPolynomial<_Tp, _MAXN, _Fp>;
        using std::vector<_Tp>::vector, std::vector<_Tp>::begin, std::vector<_Tp>::end, std::vector<_Tp>::size, std::vector<_Tp>::back, std::vector<_Tp>::empty, std::vector<_Tp>::clear, std::vector<_Tp>::pop_back, std::vector<_Tp>::resize, std::vector<_Tp>::push_back;
        static inline struct complex {
            _Fp x, y;
            complex() = default;
            complex(_Fp _val) : x(_val), y(0) {}
            complex(_Fp _x, _Fp _y) : x(_x), y(_y) {}
            complex operator+(const complex &_other) const { return complex(x + _other.x, y + _other.y); }
            complex operator-(const complex &_other) const { return complex(x - _other.x, y - _other.y); }
            complex operator*(const complex &_other) const { return complex(x * _other.x - y * _other.y, x * _other.y + y * _other.x); }
            complex operator/(const complex &_other) const { return complex(x * _other.x + y * _other.y, y * _other.x - x * _other.y); }
            complex &operator+=(const complex &_other) { return *this = complex(x + _other.x, y + _other.y); }
            complex &operator-=(const complex &_other) { return *this = complex(x - _other.x, y - _other.y); }
            complex &operator*=(const complex &_other) { return *this = complex(x * _other.x - y * _other.y, x * _other.y + y * _other.x); }
            operator _Tp() const { return x; }
        } s_dftRoots[_MAXN], s_dftBuffer[_MAXN * 2];
        static inline uint32_t s_dftBin[_MAXN], s_dftSize = 1;
        static void prepareDFT(uint32_t __length) {
            if (__length > s_dftSize) {
                if (s_dftSize == 1) s_dftRoots[s_dftSize++] = complex(1);
                for (; s_dftSize < __length; s_dftSize *= 2) {
                    const _Fp x(3.14159265358979323846L / s_dftSize);
                    const complex wn(std::cos(x), std::sin(x));
                    for (uint32_t i = s_dftSize; i < s_dftSize * 2; i += 2) {
                        s_dftRoots[i] = s_dftRoots[i / 2];
                        s_dftRoots[i + 1] = s_dftRoots[i / 2] * wn;
                    }
                }
            }
            if (__length > 1 && !s_dftBin[__length + 1])
                for (uint32_t i = 0; i < __length; i += 2) {
                    s_dftBin[__length + i] = s_dftBin[__length + i / 2] / 2;
                    s_dftBin[__length + i + 1] = s_dftBin[__length + i / 2] / 2 + __length / 2;
                }
        }
        template <typename _Iterator>
        static _Iterator _dft(_Iterator __iter, uint32_t __length) {
            for (uint32_t i = 0; i < __length; i++)
                if (uint32_t j = s_dftBin[__length + i]; i < j) std::swap(*(__iter + i), *(__iter + j));
            for (uint32_t h = 2; h <= __length; h *= 2)
                for (uint32_t j = 0; j < __length; j += h)
                    for (uint32_t k = j; k < j + h / 2; k++) {
                        complex x(*(__iter + k)), y(*(__iter + (k + h / 2)) * s_dftRoots[h / 2 + k - j]);
                        *(__iter + k) += y;
                        *(__iter + (k + h / 2)) = x - y;
                    }
            return __iter;
        }
        template <typename _Iterator>
        static _Iterator dft(_Iterator __iter, uint32_t __length) { return prepareDFT(__length), _dft(__iter, __length); }
        template <typename _Iterator, typename _Iterator2>
        static _Iterator dft(_Iterator __iter, uint32_t __dftLength, _Iterator2 __source, uint32_t __sourceLength) { return dft(std::fill_n(std::copy_n(__source, __sourceLength, __iter), __dftLength - __sourceLength, _Fp(0)) - __dftLength, __dftLength); }
        template <typename _Iterator>
        static _Iterator idft(_Iterator __iter, uint32_t __length) {
            _dft(__iter, __length);
            std::reverse(__iter + 1, __iter + __length);
            for (uint32_t i = 0; i < __length; i++) *(__iter + i) = complex(std::round((*(__iter + i)).x / __length), 0);
            return __iter;
        }
        template <typename _Iterator, typename _Iterator2, typename _Operation = std::multiplies<complex>>
        static _Iterator _transform(_Iterator __iter1, _Iterator2 __iter2, uint32_t __length, _Operation __op = _Operation()) { return std::transform(__iter1, __iter1 + __length, __iter2, __iter1, __op) - __length; }
        static poly product(const poly &__a, const poly &__b, uint32_t __length) {
            if (__a.empty() || __b.empty()) return poly();
            idft(_transform(dft(s_dftBuffer, __length, __a.begin(), std::min<uint32_t>(__length, __a.size())), dft(s_dftBuffer + __length, __length, __b.begin(), std::min<uint32_t>(__length, __b.size())), __length, std::multiplies<complex>()), __length);
            return poly(s_dftBuffer, s_dftBuffer + __length);
        }
        poly &shrink() {
            while (size() && !back()) pop_back();
            return *this;
        }
        poly &reverse() {
            std::reverse(begin(), end());
            return *this;
        }
        poly &sizeTo(uint32_t __size) {
            resize(__size);
            return *this;
        }
        _Tp at(uint32_t __i) const { return __i < size() ? (*this)[__i] : 0; }
        poly &operator+=(const poly &__other) {
            _transform(sizeTo(std::max<uint32_t>(size(), __other.size())).begin(), __other.begin(), __other.size(), std::plus<_Tp>());
            return shrink();
        }
        poly &operator-=(const poly &__other) {
            _transform(sizeTo(std::max<uint32_t>(size(), __other.size())).begin(), __other.begin(), __other.size(), std::minus<_Tp>());
            return shrink();
        }
        poly &operator*=(_Tp __other) {
            if (!__other)
                clear();
            else
                for (auto &a : *this) a *= __other;
            return *this;
        }
        poly &operator*=(const poly &__other) { return (*this = product(*this, __other, std::__bit_ceil(size() + __other.size() - 1))).shrink(); }
        poly &operator/=(_Tp __other) {
            for (auto &a : *this) a /= __other;
            return *this;
        }
        poly operator+() const { return *this; }
        poly operator-() const {
            poly res(*this);
            std::transform(res.begin(), res.end(), res.begin(), std::negate<_Tp>());
            return res;
        }
        friend poly operator+(const poly &__a, const poly &__b) {
            poly res(__a);
            res += __b;
            return res;
        }
        friend poly operator-(const poly &__a, const poly &__b) {
            poly res(__a);
            res -= __b;
            return res;
        }
        friend poly operator*(const poly &__a, _Tp __b) {
            poly res(__a);
            res *= __b;
            return res;
        }
        friend poly operator*(const poly &__a, const poly &__b) {
            poly res(product(__a, __b, std::__bit_ceil(__a.size() + __b.size() - 1)));
            res.shrink();
            return res;
        }
        friend poly operator/(const poly &__a, _Tp __b) {
            poly res(__a);
            res /= __b;
            return res;
        }
        template <typename _Istream>
        friend _Istream &operator>>(_Istream &__is, poly &__self) {
            for (auto &a : __self) __is >> a;
            return __is;
        }
        template <typename _Ostream>
        friend _Ostream &operator<<(_Ostream &__os, const poly &__self) {
            for (auto &a : __self) __os << a << ' ';
            return __os;
        }
        _Tp calc(_Tp __x) const {
            _Tp res(0);
            for (uint32_t i = size() - 1; ~i; i--) res = res * __x + (*this)[i];
            return res;
        }
    };
}

#endif