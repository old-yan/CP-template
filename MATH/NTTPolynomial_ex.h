#ifndef __OY_NTTPOLYNOMIALEX__
#define __OY_NTTPOLYNOMIALEX__

#include <algorithm>
#include <bit>
#include <cstdint>
#include "Modular.h"

namespace OY {
    template <typename _Tp, uint32_t _MAXN = 1 << 22, uint32_t _P1 = 1107296257, uint32_t _R1 = 10, uint32_t _P2 = 1711276033, uint32_t _R2 = 29, uint32_t _P3 = 1811939329, uint32_t _R3 = 13>
    struct NTTPolynomial_ex : std::vector<_Tp> {
        using poly = NTTPolynomial_ex<_Tp, _MAXN, _P1, _R1, _P2, _R2, _P3, _R3>;
        using std::vector<_Tp>::vector, std::vector<_Tp>::begin, std::vector<_Tp>::end, std::vector<_Tp>::rbegin, std::vector<_Tp>::rend, std::vector<_Tp>::size, std::vector<_Tp>::back, std::vector<_Tp>::empty, std::vector<_Tp>::clear, std::vector<_Tp>::pop_back, std::vector<_Tp>::resize, std::vector<_Tp>::push_back;
        static inline struct triple {
            static constexpr uint32_t inv_1_2 = Modular32<_P2>::pow(_P1, _P2 - 2), inv_12_3 = Modular32<_P3>::pow(Modular32<_P3>::multiply(_P1, _P2), _P3 - 2);
            static constexpr uint64_t P12 = uint64_t(_P1) * _P2;
            uint32_t x, y, z;
            triple() = default;
            triple(uint32_t _val) : x(_val), y(_val), z(_val) {}
            triple(_Tp _val) : x(_val.val()), y(_val.val()), z(_val.val()) {}
            triple(uint32_t _x, uint32_t _y, uint32_t _z) : x(_x), y(_y), z(_z) {}
            static triple root(uint32_t __n) { return triple(Modular32<_P1>::pow(_R1, (_P1 - 1) / __n), Modular32<_P2>::pow(_R2, (_P2 - 1) / __n), Modular32<_P3>::pow(_R3, (_P3 - 1) / __n)); }
            static triple inv(uint32_t __x) { return triple(Modular32<_P1>::pow(__x, _P1 - 2), Modular32<_P2>::pow(__x, _P2 - 2), Modular32<_P3>::pow(__x, _P3 - 2)); }
            triple operator+(const triple &_other) const { return triple(Modular32<_P1>::plus(x, _other.x), Modular32<_P2>::plus(y, _other.y), Modular32<_P3>::plus(z, _other.z)); }
            triple operator-(const triple &_other) const { return triple(Modular32<_P1>::minus(x, _other.x), Modular32<_P2>::minus(y, _other.y), Modular32<_P3>::minus(z, _other.z)); }
            triple operator*(const triple &_other) const { return triple(Modular32<_P1>::multiply(x, _other.x), Modular32<_P2>::multiply(y, _other.y), Modular32<_P3>::multiply(z, _other.z)); }
            triple &operator+=(const triple &_other) { return *this = operator+(_other); }
            triple &operator-=(const triple &_other) { return *this = operator-(_other); }
            triple &operator*=(const triple &_other) { return *this = operator*(_other); }
            triple &reduce() {
                uint64_t X = uint64_t(Modular32<_P2>::multiply(Modular32<_P2>::minus(y, x), inv_1_2)) * _P1 + x;
                _Tp val = Modular32<_P3>::multiply(Modular32<_P3>::minus(z, Modular32<_P3>::mod(X)), inv_12_3) * _Tp(P12) + X;
                return *this = triple(val.val(), val.val(), val.val());
            }
            operator _Tp() const { return x; }
        } s_dftRoots[_MAXN], s_dftBuffer[_MAXN * 2], s_treeBuffer[_MAXN * 2 * (std::__countr_zero(_MAXN))];
        static inline _Tp s_inverse[_MAXN + 1];
        static inline uint32_t s_dftBin[_MAXN * 2], s_dftSize = 1, s_inverseSize = 0;
        static inline poly s_treeSum;
        static void prepareDFT(uint32_t __length) {
            if (__length == 1 || s_dftBin[__length + 1]) return;
            if (s_dftSize == 1) s_dftRoots[s_dftSize++] = 1;
            while (s_dftSize < __length) {
                const triple wn = triple::root(s_dftSize * 2);
                for (uint32_t i = s_dftSize; i < s_dftSize * 2; i += 2) {
                    s_dftRoots[i] = s_dftRoots[i / 2];
                    s_dftRoots[i + 1] = s_dftRoots[i / 2] * wn;
                }
                s_dftSize *= 2;
            }
            for (uint32_t i = 0; i < __length; i += 2) {
                s_dftBin[__length + i] = s_dftBin[__length + i / 2] / 2;
                s_dftBin[__length + i + 1] = s_dftBin[__length + i / 2] / 2 + __length / 2;
            }
        }
        template <typename _Iterator>
        static _Iterator butterfly(_Iterator __iter, uint32_t __length) {
            for (uint32_t i = 0; i < __length; i++)
                if (uint32_t j = s_dftBin[__length + i]; i < j) std::swap(__iter[i], __iter[j]);
            for (uint32_t h = 2; h <= __length; h *= 2)
                for (uint32_t j = 0; j < __length; j += h)
                    for (uint32_t k = j; k < j + h / 2; k++) {
                        triple x = __iter[k], y = __iter[k + h / 2] * s_dftRoots[h / 2 + k - j];
                        __iter[k] += y;
                        __iter[k + h / 2] = x - y;
                    }
            return __iter;
        }
        template <typename _Iterator>
        static _Iterator setHighZero(_Iterator __iter, uint32_t __length) { return std::fill_n(__iter + __length / 2, __length / 2, 0) - __length; }
        template <typename _Iterator>
        static _Iterator dft(_Iterator __iter, uint32_t __length) {
            prepareDFT(__length);
            return butterfly(__iter, __length);
        }
        template <typename _Iterator, typename _Iterator2>
        static _Iterator dft(_Iterator __iter, uint32_t __dftLength, _Iterator2 __source, uint32_t __sourceLength) { return dft(std::fill_n(std::copy_n(__source, __sourceLength, __iter), __dftLength - __sourceLength, 0) - __dftLength, __dftLength); }
        template <typename _Iterator>
        static _Iterator idft(_Iterator __iter, uint32_t __length) {
            butterfly(__iter, __length);
            std::reverse(__iter + 1, __iter + __length);
            const triple inv = triple::inv(__length);
            for (uint32_t i = 0; i < __length; i++) (__iter[i] *= inv).reduce();
            return __iter;
        }
        template <typename _Iterator, typename _Iterator2, typename _Operation = std::multiplies<triple>>
        static _Iterator transform(_Iterator __iter1, _Iterator2 __iter2, uint32_t __length, _Operation __op = _Operation()) { return std::transform(__iter1, __iter1 + __length, __iter2, __iter1, __op) - __length; }
        static void prepareInverse(uint32_t __length) {
            if (s_inverseSize > __length) return;
            if (!s_inverseSize) s_inverse[0] = s_inverse[++s_inverseSize] = 1;
            const auto P = _Tp::mod();
            for (uint32_t i = s_inverseSize + 1; i <= __length; i++) {
                auto q = P / i, r = P - q * i;
                s_inverse[i] = s_inverse[r] * _Tp(P - q);
            }
            s_inverseSize = __length;
        }
        template <typename _Iterator>
        static void derivate(_Iterator __iter, uint32_t __length) {
            for (uint32_t i = 0; i + 1 < __length; i++) __iter[i] = __iter[i + 1] * (i + 1);
            __iter[__length - 1] = 0;
        }
        template <typename _Iterator>
        static void integrate(_Iterator __iter, uint32_t __length) {
            if (!__length) return;
            for (uint32_t i = __length; i; i--) __iter[i] = __iter[i - 1] * s_inverse[i];
            __iter[0] = 0;
        }
        static poly product(const poly &__a, const poly &__b, uint32_t __length) {
            if (__a.empty() || __b.empty()) return {};
            idft(transform(dft(s_dftBuffer, __length, __a.begin(), std::min<uint32_t>(__length, __a.size())), dft(s_dftBuffer + __length, __length, __b.begin(), std::min<uint32_t>(__length, __b.size())), __length), __length);
            return poly(s_dftBuffer, s_dftBuffer + __length);
        }
        static void initTree(const poly &__xs, uint32_t __length) {
            triple *it = s_treeBuffer + __length * 2 * std::__countr_zero(__length / 2);
            for (uint32_t i = 0; i < __length; i++) *it++ = 1, *it++ = -__xs.at(i);
            for (uint32_t h = 2; h < __length; h *= 2) {
                triple *it = s_treeBuffer + __length * 2 * std::__countr_zero(__length / 2 / h);
                for (uint32_t i = 0; i < __length; i += h, it += h * 2) {
                    idft(std::fill_n(std::transform(dft(it + __length * 2, h), it + __length * 2 + h, dft(it + __length * 2 + h, h), it, std::multiplies<triple>()), h, 0) - h * 2, h);
                    it[h] = _Tp(it[0]) - 1, it[0] = 1;
                }
            }
            std::copy_n(idft(std::transform(dft(s_treeBuffer, __length), s_treeBuffer + __length, dft(s_treeBuffer + __length, __length), s_dftBuffer, std::multiplies<triple>()) - __length, __length), __length, s_treeSum.sizeTo(0).sizeTo(__length * 2).begin());
            s_treeSum[__length] = s_treeSum[0] - 1;
            s_treeSum[0] = 1;
        }
        static poly calcTree(const poly &__f, uint32_t __resLength) {
            uint32_t length = s_treeSum.size() / 2;
            triple res[length];
            std::copy_n(__f.div(s_treeSum.reverse()).reverse().begin(), __f.size(), std::fill_n(res, length - __f.size(), 0));
            for (uint32_t h = length / 2; h; h >>= 1)
                for (triple *it = res, *end = res + __resLength, *it2 = s_treeBuffer + length * 2 * std::__countr_zero(length / 2 / h); it < end; it += h * 2, it2 += h * 4) std::copy_n(s_dftBuffer + h, h, std::copy_n(idft(transform(idft(transform(std::copy_n(it2 + h * 2, h * 2, std::copy_n(it2, h * 2, s_dftBuffer)) - h * 4, dft(it, h * 2), h * 2), h * 2) + h * 2, it, h * 2), h * 2) + h, h, it));
            return poly(res, res + __resLength);
        }
        static poly fromPoints(const poly &__xs, const poly &__ys) {
            if (__xs.size() <= 1) return __ys;
            uint32_t length = std::__bit_ceil(__xs.size());
            initTree(__xs, length);
            triple res[length];
            std::fill(std::copy_n(calcTree(poly(s_treeSum).sizeTo(__xs.size() + 1).reverse().derivate(), __xs.size()).begin(), __xs.size(), res), res + length, 0);
            for (uint32_t i = 0; i < __ys.size(); i++) res[i] = __ys[i] / _Tp(res[i]);
            for (uint32_t h = 1; h < length; h *= 2)
                for (triple *it = res, *end = res + __xs.size(), *it2 = s_treeBuffer + length * 2 * std::__countr_zero(length / 2 / h); it < end; it += h * 2, it2 += h * 4) {
                    dft(dft(s_dftBuffer, h * 2, it, h) + h * 2, h * 2, it + h, h);
                    for (uint32_t i = 0; i < h * 2; i++) it[i] = s_dftBuffer[i] * it2[h * 2 + i] + s_dftBuffer[h * 2 + i] * it2[i];
                    idft(it, h * 2);
                }
            return poly(res, res + __xs.size()).reverse();
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
        poly &derivate() {
            derivate(begin(), size());
            pop_back();
            return *this;
        }
        poly &integrate() {
            push_back(0);
            integrate(begin(), size() - 1);
            return *this;
        }
        _Tp at(uint32_t __i) const { return __i < size() ? (*this)[__i] : 0; }
        poly &operator+=(const poly &__other) {
            transform(sizeTo(std::max<uint32_t>(size(), __other.size())).begin(), __other.begin(), __other.size(), std::plus<_Tp>());
            return shrink();
        }
        poly &operator-=(const poly &__other) {
            transform(sizeTo(std::max<uint32_t>(size(), __other.size())).begin(), __other.begin(), __other.size(), std::minus<_Tp>());
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
        friend poly operator+(const poly &__a, const poly &__b) { return poly(__a) += __b; }
        friend poly operator-(const poly &__a, const poly &__b) { return poly(__a) -= __b; }
        friend poly operator*(const poly &__a, _Tp __b) { return poly(__a) *= __b; }
        friend poly operator*(const poly &__a, const poly &__b) { return (product(__a, __b, std::__bit_ceil(__a.size() + __b.size() - 1))).shrink(); }
        friend poly operator/(const poly &__a, _Tp __b) { return poly(__a) /= __b; }
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
        poly ChirpZ(_Tp __x, uint32_t __n) const {
            if (empty()) return poly(__n, 0);
            uint32_t length = std::__bit_ceil(size() + __n - 1);
            const _Tp inv = __x.inv();
            _Tp cur = 1, pow = 1;
            for (uint32_t i = 0; i < size() + __n - 1; i++, cur *= pow, pow *= __x) s_dftBuffer[i] = cur;
            cur = 1, pow = 1;
            for (uint32_t i = size() - 1; ~i; i--, cur *= pow, pow *= inv) s_dftBuffer[length + i] = (*this)[size() - 1 - i] * cur;
            idft(transform(dft(std::fill_n(s_dftBuffer + size() + __n - 1, length - size() - __n + 1, 0) - length, length), dft(std::fill_n(s_dftBuffer + length + size(), length - size(), 0) - length, length), length), length);
            poly res(s_dftBuffer + size() - 1, s_dftBuffer + size() - 1 + __n);
            cur = _Tp(1).inv(), pow = 1;
            for (auto &a : res) a *= cur, cur *= pow, pow *= inv;
            return res;
        }
        _Tp calc(_Tp __x) const {
            _Tp res = 0;
            for (uint32_t i = size() - 1; ~i; i--) res = res * __x + (*this)[i];
            return res;
        }
        poly inv() const {
            static constexpr uint32_t R = 16;
            uint32_t Block = std::__bit_ceil((size() - 1) / R + 1);
            _Tp res[R * Block];
            triple f[R * Block * 2], g[R * Block * 2], h[Block * 2];
            res[0] = (*this)[0].inv();
            auto dfs = [&](auto self, uint32_t n) -> void {
                if (n == 1) return;
                uint32_t block = std::__bit_ceil((n - 1) / R + 1);
                self(self, block);
                dft(f, block * 2, begin(), block);
                for (uint32_t k = 1; k * block < n; k++) {
                    dft(f + k * 2 * block, block * 2, begin() + k * block, k * block < size() ? std::min<uint32_t>(size() - k * block, block) : 0);
                    dft(g + (k - 1) * 2 * block, block * 2, res + (k - 1) * block, block);
                    std::fill_n(h, block * 2, 0);
                    for (uint32_t j = 0; j < k; j++)
                        for (triple *it = h, *it1 = g + j * 2 * block, *it2 = f + (k - j) * 2 * block, *it3 = f + (k - j - 1) * 2 * block, *end = h + 2 * block; it != end;) *it++ += *it1++ * (*it2++ + *it3++), *it++ += *it1++ * (*it2++ - *it3++);
                    idft(h, block * 2);
                    for (uint32_t i = 0; i < block * 2; i++) h[i] = -_Tp(h[i]);
                    std::copy_n(idft(transform(dft(setHighZero(h, block * 2), block * 2), g, block * 2), block * 2), block, res + k * block);
                }
            };
            dfs(dfs, size());
            return poly(res, res + size());
        }
        poly sqrt(_Tp __a0 = 1) const {
            static constexpr uint32_t R = 16;
            uint32_t Block = std::__bit_ceil((size() - 1) / R + 1);
            const _Tp inv = _Tp(2).inv();
            _Tp res[R * Block];
            triple f[Block * 2], g[R * Block * 2], h[Block * 2];
            res[0] = __a0;
            auto dfs = [&](auto self, uint32_t n) -> void {
                if (n == 1) return;
                uint32_t block = std::__bit_ceil((n - 1) / R + 1);
                self(self, block);
                dft(f, block * 2, poly(res, res + block).inv().begin(), block);
                std::fill_n(g, R * block * 2, 0);
                for (uint32_t k = 1; k * block < n; k++) {
                    dft(g + (k - 1) * 2 * block, block * 2, res + (k - 1) * block, block);
                    std::fill_n(h, block * 2, 0);
                    for (uint32_t j = 0; j < k; j++)
                        for (triple *it = h, *it1 = g + j * 2 * block, *it2 = g + (k - j) * 2 * block, *it3 = g + (k - j - 1) * 2 * block, *end = h + 2 * block; it != end;) *it++ += *it1++ * (*it2++ + *it3++), *it++ += *it1++ * (*it2++ - *it3++);
                    setHighZero(idft(h, block * 2), block * 2);
                    for (uint32_t i = 0; i < block; i++) h[i] = at(k * block + i) - _Tp(h[i]);
                    idft(transform(dft(h, block * 2), f, block * 2), block * 2);
                    for (uint32_t i = 0; i < block; i++) res[k * block + i] = _Tp(h[i]) * inv;
                }
            };
            dfs(dfs, size());
            return poly(res, res + size());
        }
        std::pair<poly, poly> divmod(const poly &__other) const {
            poly x = div(__other, size() - __other.size() + 1);
            return {x, *this - x * __other};
        }
        poly div(const poly &__other, uint32_t __length = -1) const {
            chmin(__length, size());
            static constexpr uint32_t R = 16;
            uint32_t Block = std::__bit_ceil((__length - 1) / R + 1);
            _Tp res[R * Block], a[R * Block], b[R * Block];
            triple f[R * Block * 2], g[R * Block * 2], h[Block * 2], bi[Block * 2];
            poly binv = poly(__other).reverse().sizeTo(Block).inv();
            std::fill(std::copy_n(rbegin(), __length, a), a + R * Block, 0);
            std::fill(std::copy_n(__other.rbegin(), std::min<uint32_t>(__length, __other.size()), b), b + R * Block, 0);
            res[0] = a[0] * b[0].inv();
            auto dfs = [&](auto self, uint32_t n) -> void {
                if (n == 1) return;
                uint32_t block = std::__bit_ceil((n - 1) / R + 1);
                self(self, block);
                dft(bi, block * 2, binv.begin(), block);
                dft(f, block * 2, b, block);
                for (uint32_t k = 1; k * block < n; k++) {
                    dft(f + k * 2 * block, block * 2, b + k * block, block);
                    dft(g + (k - 1) * 2 * block, block * 2, res + (k - 1) * block, block);
                    std::fill_n(h, block * 2, 0);
                    for (uint32_t j = 0; j < k; j++)
                        for (triple *it = h, *it1 = g + j * 2 * block, *it2 = f + (k - j) * 2 * block, *it3 = f + (k - j - 1) * 2 * block, *end = h + 2 * block; it != end;) *it++ += *it1++ * (*it2++ + *it3++), *it++ += *it1++ * (*it2++ - *it3++);
                    setHighZero(idft(h, block * 2), block * 2);
                    for (uint32_t i = 0; i < block; i++) h[i] = _Tp(a[k * block + i]) - _Tp(h[i]);
                    std::copy_n(idft(transform(dft(h, block * 2), bi, block * 2), block * 2), block, res + k * block);
                }
            };
            dfs(dfs, __length);
            return poly(res, res + __length).reverse();
        }
        poly mod(const poly &__other) const { return divmod(__other).second; }
        poly logarithm() const {
            prepareInverse(size());
            poly f(*this);
            for (uint32_t i = 0; i < size(); i++) f[i] *= i;
            f = f.reverse().div(poly(*this).reverse()).reverse();
            for (uint32_t i = 1; i < size(); i++) f[i] *= s_inverse[i];
            return f;
        }
        poly exponent() const {
            static constexpr uint32_t R = 16;
            uint32_t Block = std::__bit_ceil((size() - 1) / R + 1);
            _Tp res[R * Block], a[R * Block];
            triple f[R * Block * 2], g[R * Block * 2], h[Block * 2], bi[Block * 2];
            std::fill(std::copy_n(begin(), size(), a), a + R * Block, 0);
            for (uint32_t i = 0; i < size(); i++) a[i] *= i;
            res[0] = 1;
            prepareInverse((R + 1) * Block);
            auto dfs = [&](auto self, uint32_t n) -> void {
                if (n == 1) return;
                uint32_t block = std::__bit_ceil((n - 1) / R + 1);
                self(self, block);
                dft(bi, block * 2, poly(res, res + block).inv().begin(), block);
                dft(f, block * 2, a, block);
                for (uint32_t k = 1; k * block < n; k++) {
                    dft(f + k * 2 * block, block * 2, a + k * block, block);
                    dft(g + (k - 1) * 2 * block, block * 2, res + (k - 1) * block, block);
                    std::fill_n(h, block * 2, 0);
                    for (uint32_t j = 0; j < k; j++)
                        for (triple *it = h, *it1 = g + j * 2 * block, *it2 = f + (k - j) * 2 * block, *it3 = f + (k - j - 1) * 2 * block, *end = h + 2 * block; it != end;) *it++ += *it1++ * (*it2++ + *it3++), *it++ += *it1++ * (*it2++ - *it3++);
                    std::copy_n(idft(transform(dft(transform(setHighZero(idft(transform(dft(setHighZero(idft(h, block * 2), block * 2), block * 2), bi, block * 2), block * 2), block * 2), s_inverse + k * block, block, std::multiplies<_Tp>()), block * 2), g, block * 2), block * 2), block, res + k * block);
                }
            };
            dfs(dfs, size());
            return poly(res, res + size());
        }
        poly calc(const poly &__xs) const {
            initTree(__xs, std::__bit_ceil(std::max<uint32_t>(__xs.size(), size())));
            return calcTree(*this, __xs.size());
        }
    };
}

#endif