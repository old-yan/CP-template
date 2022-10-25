#ifndef __OY_MTTPOLYNOMIAL__
#define __OY_MTTPOLYNOMIAL__

#include <algorithm>
#include <bit>
#include <cstdint>

namespace OY {
    template <typename _Tp, uint32_t _MAXN = 1 << 22, typename _Fp = double, uint32_t _Split = 1 << 15>
    struct MTTPolynomial : std::vector<_Tp> {
        using poly = MTTPolynomial<_Tp, _MAXN, _Fp, _Split>;
        using std::vector<_Tp>::vector, std::vector<_Tp>::begin, std::vector<_Tp>::end, std::vector<_Tp>::rbegin, std::vector<_Tp>::rend, std::vector<_Tp>::size, std::vector<_Tp>::back, std::vector<_Tp>::empty, std::vector<_Tp>::clear, std::vector<_Tp>::pop_back, std::vector<_Tp>::resize, std::vector<_Tp>::push_back;
        static inline struct complex {
            _Fp x, y;
            complex() = default;
            complex(_Fp _val) : x(_val), y(0) {}
            complex(_Tp _val) : x(_val.val() / _Split), y(_val.val() % _Split) {}
            complex(_Fp _x, _Fp _y) : x(_x), y(_y) {}
            complex conj() const { return complex(x, -y); }
            complex operator-() const { return complex(-x, -y); }
            complex operator+(const complex &_other) const { return complex(x + _other.x, y + _other.y); }
            complex operator-(const complex &_other) const { return complex(x - _other.x, y - _other.y); }
            complex operator*(const complex &_other) const { return complex(x * _other.x - y * _other.y, x * _other.y + y * _other.x); }
            complex operator/(const complex &_other) const { return complex(x * _other.x + y * _other.y, y * _other.x - x * _other.y); }
            complex &operator+=(const complex &_other) { return *this = complex(x + _other.x, y + _other.y); }
            complex &operator-=(const complex &_other) { return *this = complex(x - _other.x, y - _other.y); }
            complex &operator*=(const complex &_other) { return *this = complex(x * _other.x - y * _other.y, x * _other.y + y * _other.x); }
            operator _Tp() const { return std::round(x) * _Tp(_Split) + std::round(y); }
        } s_dftRoots[_MAXN], s_dftBuffer[_MAXN * 2], s_treeBuffer[_MAXN * 2 * (std::__countr_zero(_MAXN))];
        static inline struct complex_pair {
            complex p, q;
            complex_pair() = default;
            complex_pair(_Fp _val) : p(_val), q(_val) {}
            complex_pair(const complex &_p, const complex &_q) : p(_p), q(_q) {}
            complex_pair(const complex &_a, const complex &_b, const complex &_c) : p(_c * _a), q(_c / _b) {}
            complex_pair operator+(const complex_pair &_other) const { return {p + _other.p, q + _other.q}; }
            complex_pair operator-(const complex_pair &_other) const { return {p - _other.p, q - _other.q}; }
            complex_pair operator*(const complex &_other) const { return {p * _other, q * _other}; }
            complex_pair &operator+=(const complex_pair &_other) { return *this = {p + _other.p, q + _other.q}; }
            complex_pair &operator-=(const complex_pair &_other) { return *this = {p - _other.p, q - _other.q}; }
            complex_pair &operator*=(_Fp _other) { return *this = {p * _other, q * _other}; }
            operator _Tp() const {
                complex x(p + q), y(p - q);
                _Tp u(_Split);
                return _Tp(std::round(x.x / 2) * u + std::round((x.y + y.y) / 2)) * u + std::round(-y.x / 2);
            }
            operator complex() const { return _Tp(*this); }
        } s_dftResultBuffer[_MAXN];
        static inline _Tp s_inverse[_MAXN + 1];
        static inline uint32_t s_dftBin[_MAXN * 2], s_dftSize = 1, s_inverseSize = 0;
        static inline poly s_treeSum;
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
                        auto x(*(__iter + k)), y(*(__iter + (k + h / 2)) * s_dftRoots[h / 2 + k - j]);
                        *(__iter + k) += y;
                        *(__iter + (k + h / 2)) = x - y;
                    }
            return __iter;
        }
        template <typename _Iterator>
        static _Iterator dft(_Iterator __iter, uint32_t __length) { return prepareDFT(__length), _dft(__iter, __length); }
        template <typename _Iterator, typename _Iterator2>
        static _Iterator dft(_Iterator __iter, uint32_t __dftLength, _Iterator2 __source, uint32_t __sourceLength) { return dft(std::fill_n(std::copy_n(__source, __sourceLength, __iter), __dftLength - __sourceLength, 0) - __dftLength, __dftLength); }
        template <typename _Iterator>
        static _Iterator idft(_Iterator __iter, uint32_t __length) {
            _dft(__iter, __length);
            std::reverse(__iter + 1, __iter + __length);
            const _Fp inv(1 / _Fp(__length));
            for (uint32_t i = 0; i < __length; i++) *(__iter + i) *= inv;
            return __iter;
        }
        template <typename _Iterator, typename _Iterator2, typename _Iterator3>
        static _Iterator _transform(_Iterator __iter1, _Iterator2 __iter2, _Iterator3 __iter3, uint32_t __length) {
            for (uint32_t i = 0, j = 0; i < __length; i++, j = j ? j - 1 : __length - 1) {
                (*(__iter1 + i)).p = *(__iter3 + i) * *(__iter2 + i);
                (*(__iter1 + i)).q = *(__iter3 + i) / *(__iter2 + (i ? __length - i : 0));
            }
            return __iter1;
        }
        static void prepareInverse(uint32_t __length) {
            if (s_inverseSize >= __length) return;
            if (!s_inverseSize) s_inverse[0] = s_inverse[++s_inverseSize] = _Tp(1);
            const auto P(_Tp::mod());
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
            prepareInverse(__length);
            for (uint32_t i = __length; i; i--) *(__iter + i) = *(__iter + (i - 1)) * s_inverse[i];
            __iter[0] = 0;
        }
        static poly product(const poly &__a, const poly &__b, uint32_t __length) {
            if (__a.empty() || __b.empty()) return poly();
            idft(_transform(s_dftResultBuffer, dft(s_dftBuffer, __length, __a.begin(), std::min<uint32_t>(__length, __a.size())), dft(s_dftBuffer + __length, __length, __b.begin(), std::min<uint32_t>(__length, __b.size())), __length), __length);
            return poly(s_dftResultBuffer, s_dftResultBuffer + __length);
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
            shrink();
            return *this;
        }
        poly &integrate() {
            push_back(0);
            integrate(begin(), size() - 1);
            shrink();
            return *this;
        }
        _Tp at(uint32_t __i) const { return __i < size() ? (*this)[__i] : 0; }
        poly &operator+=(const poly &__other) {
            if (size() < __other.size()) resize(__other.size());
            std::transform(begin(), begin() + __other.size(), __other.begin(), begin(), std::plus<_Tp>());
            return shrink();
        }
        poly &operator-=(const poly &__other) {
            if (size() < __other.size()) resize(__other.size());
            std::transform(begin(), begin() + __other.size(), __other.begin(), begin(), std::minus<_Tp>());
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
            _Tp res = 0;
            for (uint32_t i = size() - 1; ~i; i--) res = res * __x + (*this)[i];
            return res;
        }
        poly ChirpZ(_Tp __x, uint32_t __n) const {
            if (empty()) return poly(__n, 0);
            const uint32_t length = std::__bit_ceil(size() + __n - 1);
            const _Tp inv(__x.inv());
            _Tp cur(1), pow(1);
            for (uint32_t i = 0; i < size() + __n - 1; i++, cur *= pow, pow *= __x) s_dftBuffer[i] = cur;
            cur = _Tp(1), pow = _Tp(1);
            for (uint32_t i = size() - 1; ~i; i--, cur *= pow, pow *= inv) s_dftBuffer[length + i] = (*this)[size() - 1 - i] * cur;
            idft(_transform(s_dftResultBuffer, dft(std::fill_n(s_dftBuffer + size() + __n - 1, length - size() - __n + 1, _Fp(0)) - length, length), dft(std::fill_n(s_dftBuffer + length + size(), length - size(), _Fp(0)) - length, length), length), length);
            poly res(s_dftResultBuffer + size() - 1, s_dftResultBuffer + size() - 1 + __n);
            cur = _Tp(1).inv(), pow = 1;
            for (auto &a : res) a *= cur, cur *= pow, pow *= inv;
            return res;
        }
        poly inv() const {
            static constexpr uint32_t R = 16;
            const uint32_t Block = std::__bit_ceil((size() - 1) / R + 1);
            _Tp res[Block * R];
            complex f[Block * R * 2], g[Block * R * 2], h[Block * 2];
            res[0] = (*this)[0].inv();
            auto dfs = [&](auto self, uint32_t n) -> void {
                if (n == 1) return;
                const uint32_t block = std::__bit_ceil((n - 1) / R + 1);
                self(self, block);
                dft(f, block * 2, begin(), block);
                for (uint32_t k = 1; block * k < n; k++) {
                    dft(f + block * k * 2, block * 2, begin() + block * k, block * k < size() ? std::min<uint32_t>(size() - block * k, block) : 0);
                    dft(g + block * (k - 1) * 2, block * 2, res + block * (k - 1), block);
                    std::fill_n(s_dftResultBuffer, block * 2, 0);
                    for (uint32_t j = 0; j < k; j++) {
                        complex *it1 = g + block * j * 2, *it2 = f + block * (k - j) * 2, *it3 = f + block * (k - j - 1) * 2;
                        for (uint32_t i = 0; i < block * 2; i += 2) s_dftResultBuffer[i] -= {*(it1 + i), *(it1 + (i ? block * 2 - i : 0)), *(it2 + i) + *(it3 + i)}, s_dftResultBuffer[i + 1] -= {*(it1 + (i + 1)), *(it1 + (block * 2 - i - 1)), *(it2 + (i + 1)) - *(it3 + (i + 1))};
                    }
                    std::copy_n(idft(_transform(s_dftResultBuffer, dft(h, block * 2, idft(s_dftResultBuffer, block * 2), block), g, block * 2), block * 2), block, res + block * k);
                }
            };
            dfs(dfs, size());
            return poly(res, res + size());
        }
        poly sqrt(_Tp __a0 = 1) const {
            if (empty()) return poly();
            static constexpr uint32_t R = 16;
            const _Tp inv(_Tp(2).inv());
            const uint32_t Block = std::__bit_ceil((size() - 1) / R + 1);
            _Tp res[Block * R];
            complex f[Block * 2], g[Block * R * 2], h[Block * 2];
            res[0] = __a0;
            auto dfs = [&](auto self, uint32_t n) {
                if (n == 1) return;
                const uint32_t block = std::__bit_ceil((n - 1) / R + 1);
                self(self, block);
                dft(f, block * 2, poly(res, res + block).inv().begin(), block);
                std::fill_n(g, block * R * 2, 0);
                for (uint32_t k = 1; block * k < n; k++) {
                    dft(g + block * (k - 1) * 2, block * 2, res + block * (k - 1), block);
                    std::fill_n(s_dftResultBuffer, block * 2, 0);
                    for (uint32_t j = 0; j < k; j++) {
                        complex *it1 = g + block * j * 2, *it2 = g + block * (k - j) * 2, *it3 = g + block * (k - j - 1) * 2;
                        for (uint32_t i = 0; i < block * 2; i += 2) s_dftResultBuffer[i] += {*(it1 + i), *(it1 + (i ? block * 2 - i : 0)), *(it2 + i) + *(it3 + i)}, s_dftResultBuffer[i + 1] += {*(it1 + (i + 1)), *(it1 + (block * 2 - i - 1)), *(it2 + (i + 1)) - *(it3 + (i + 1))};
                    }
                    idft(s_dftResultBuffer, block * 2);
                    for (uint32_t i = 0; i < block; i++) h[i] = at(block * k + i) - _Tp(s_dftResultBuffer[i]);
                    idft(_transform(s_dftResultBuffer, dft(std::fill_n(h + block, block, _Fp(0)) - block * 2, block * 2), f, block * 2), block * 2);
                    for (uint32_t i = 0; i < block; i++) res[block * k + i] = _Tp(s_dftResultBuffer[i]) * inv;
                }
            };
            dfs(dfs, size());
            return poly(res, res + size());
        }
        std::pair<poly, poly> divmod(const poly &__other) const {
            if (size() < __other.size()) return {poly(), *this};
            std::pair<poly, poly> res;
            res.first = div(__other, size() - __other.size() + 1);
            res.second = *this - res.first * __other;
            return res;
        }
        poly div(const poly &__other, uint32_t __length = -1) const {
            __length = std::min<uint32_t>(__length, size());
            static constexpr uint32_t R = 16;
            const uint32_t Block = std::__bit_ceil((__length - 1) / R + 1);
            _Tp res[Block * R], a[Block * R], b[Block * R];
            complex f[Block * R * 2], g[Block * R * 2], h[Block * 2], bi[Block * 2];
            poly binv(poly(__other).reverse().sizeTo(Block).inv());
            std::fill(std::copy_n(rbegin(), __length, a), a + Block * R, 0);
            std::fill(std::copy_n(__other.rbegin(), std::min<uint32_t>(__length, __other.size()), b), b + Block * R, 0);
            res[0] = a[0] * b[0].inv();
            auto dfs = [&](auto self, uint32_t n) -> void {
                if (n == 1) return;
                const uint32_t block = std::__bit_ceil((n - 1) / R + 1);
                self(self, block);
                dft(bi, block * 2, binv.begin(), block);
                dft(f, block * 2, b, block);
                for (uint32_t k = 1; block * k < n; k++) {
                    dft(f + block * k * 2, block * 2, b + block * k, block);
                    dft(g + block * (k - 1) * 2, block * 2, res + block * (k - 1), block);
                    std::fill_n(s_dftResultBuffer, block * 2, 0);
                    for (uint32_t j = 0; j < k; j++) {
                        complex *it1 = g + block * j * 2, *it2 = f + block * (k - j) * 2, *it3 = f + block * (k - j - 1) * 2;
                        for (uint32_t i = 0; i < block * 2; i += 2) s_dftResultBuffer[i] += {*(it1 + i), *(it1 + (i ? block * 2 - i : 0)), *(it2 + i) + *(it3 + i)}, s_dftResultBuffer[i + 1] += {*(it1 + (i + 1)), *(it1 + (block * 2 - i - 1)), *(it2 + (i + 1)) - *(it3 + (i + 1))};
                    }
                    idft(s_dftResultBuffer, block * 2);
                    for (uint32_t i = 0; i < block; i++) h[i] = _Tp(a[block * k + i]) - _Tp(s_dftResultBuffer[i]);
                    std::copy_n(idft(_transform(s_dftResultBuffer, dft(std::fill_n(h + block, block, 0) - block * 2, block * 2), bi, block * 2), block * 2), block, res + block * k);
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
            (f = f.reverse().div(poly(*this).reverse())).reverse();
            for (uint32_t i = 1; i < size(); i++) f[i] *= s_inverse[i];
            return f;
        }
        poly exponent() const {
            if (empty()) return poly{_Tp(1)};
            static constexpr uint32_t R = 16;
            const uint32_t Block = std::__bit_ceil((size() - 1) / R + 1);
            _Tp res[Block * R], a[Block * R];
            complex f[Block * R * 2], g[Block * R * 2], h[Block * 2], bi[Block * 2];
            std::fill(std::copy_n(begin(), size(), a), a + Block * R, 0);
            for (uint32_t i = 0; i < size(); i++) a[i] *= i;
            res[0] = 1;
            prepareInverse(Block * (R + 1));
            auto dfs = [&](auto self, uint32_t n) -> void {
                if (n == 1) return;
                const uint32_t block = std::__bit_ceil((n - 1) / R + 1);
                self(self, block);
                dft(bi, block * 2, poly(res, res + block).inv().begin(), block);
                dft(f, block * 2, a, block);
                for (uint32_t k = 1; block * k < n; k++) {
                    dft(f + block * k * 2, block * 2, a + block * k, block);
                    dft(g + block * (k - 1) * 2, block * 2, res + block * (k - 1), block);
                    std::fill_n(s_dftResultBuffer, block * 2, 0);
                    for (uint32_t j = 0; j < k; j++) {
                        complex *it1 = g + block * j * 2, *it2 = f + block * (k - j) * 2, *it3 = f + block * (k - j - 1) * 2;
                        for (uint32_t i = 0; i < block * 2; i += 2) s_dftResultBuffer[i] += {*(it1 + i), *(it1 + (i ? block * 2 - i : 0)), *(it2 + i) + *(it3 + i)}, s_dftResultBuffer[i + 1] += {*(it1 + (i + 1)), *(it1 + (block * 2 - i - 1)), *(it2 + (i + 1)) - *(it3 + (i + 1))};
                    }
                    idft(_transform(s_dftResultBuffer, dft(h, block * 2, idft(s_dftResultBuffer, block * 2), block), bi, block * 2), block * 2);
                    for (uint32_t i = 0; i < block; i++) h[i] = _Tp(s_dftResultBuffer[i]) * s_inverse[block * k + i];
                    std::copy_n(idft(_transform(s_dftResultBuffer, dft(std::fill_n(h + block, block, _Fp(0)) - block * 2, block * 2), g, block * 2), block * 2), block, res + block * k);
                }
            };
            dfs(dfs, size());
            return poly(res, res + size());
        }
        static poly _simplePow(const poly &__a, _Tp __n) { return (__a.logarithm() * __n).exponent(); }
        template <typename _Tp2>
        poly pow(_Tp __n1, _Tp2 __n2, uint64_t __nAbs = UINT_MAX) const {
            const uint64_t zero = std::find_if(begin(), end(), [](const _Tp &x) { return x.val() != 0; }) - begin();
            if (zero && __nAbs >= (size() + zero - 1) / zero) return poly();
            if (!__n1) return poly{(*this)[zero].pow(__n2.val())};
            uint32_t rest = size() - zero * __nAbs;
            poly a(begin() + zero, begin() + zero + std::min<uint32_t>(rest, size() - zero)), res(size(), _Tp(0));
            const _Tp a0(a[0]);
            (a = _simplePow(a *= a0.inv(), __n1)) *= a0.pow(__n2.val());
            std::copy_n(a.begin(), a.size(), res.begin() + (size() - rest));
            return res;
        }
        static void _initTree(const poly &__xs, uint32_t __length) {
            complex *it = s_treeBuffer + __length * 2 * std::__countr_zero(__length / 2);
            for (uint32_t i = 0; i < __length; i++) {
                *it++ = _Tp(1), *it++ = -__xs.at(i);
                dft(it - 2, 2);
            }
            for (uint32_t h = 2; h < __length; h *= 2) {
                complex *it = s_treeBuffer + __length * 2 * std::__countr_zero(__length / 2 / h);
                for (uint32_t i = 0; i < __length; i += h, it += h * 2) {
                    std::fill_n(std::copy_n(idft(_transform(s_dftResultBuffer, it + __length * 2, it + __length * 2 + h, h), h), h, it), h, _Fp(0));
                    *(it + h) = _Tp(*it) - 1, *it = _Tp(1);
                    dft(it, h * 2);
                }
            }
            std::copy_n(idft(_transform(s_dftResultBuffer, s_treeBuffer, s_treeBuffer + __length, __length), __length), __length, s_treeSum.sizeTo(0).sizeTo(__length * 2).begin());
            s_treeSum[__length] = _Tp(s_treeSum[0]) - _Tp(1);
            s_treeSum[0] = _Tp(1);
        }
        static poly _calcTree(const poly &__f, uint32_t __resLength) {
            const uint32_t length = s_treeSum.size() / 2;
            _Tp res[length];
            std::copy_n(__f.div(s_treeSum.reverse()).reverse().begin(), __f.size(), std::fill_n(res, length - __f.size(), 0));
            for (uint32_t h = length / 2; h; h >>= 1) {
                complex *it2 = s_treeBuffer + length * 2 * std::__countr_zero(length / 2 / h);
                for (_Tp *it = res, *end = res + __resLength; it < end; it += h * 2, it2 += h * 4) std::copy_n(s_dftResultBuffer + h, h, std::copy_n(idft(_transform(idft(_transform(s_dftResultBuffer, s_dftBuffer, dft(std::copy_n(it2 + h * 2, h * 2, std::copy_n(it2, h * 2, s_dftBuffer)), h * 2, it, h * 2), h * 2), h * 2) + h * 2, s_dftBuffer + h * 2, s_dftBuffer + h * 4, h * 2), h * 2) + h, h, it));
            }
            return poly(res, res + __resLength);
        }
        static poly fromPoints(const poly &__xs, const poly &__ys) {
            if (__xs.size() <= 1) return __ys;
            const uint32_t length = std::__bit_ceil(__xs.size());
            _initTree(__xs, length);
            _Tp res[length];
            std::fill(std::copy_n(_calcTree(poly(s_treeSum).sizeTo(__xs.size() + 1).reverse().derivate(), __xs.size()).begin(), __xs.size(), res), res + length, 0);
            for (uint32_t i = 0; i < __ys.size(); i++) res[i] = __ys[i] / res[i];
            for (uint32_t h = 1; h < length; h *= 2) {
                _Tp *it = res, *end = res + __xs.size();
                complex *it2 = s_treeBuffer + length * 2 * std::__countr_zero(length / 2 / h);
                for (; it < end; it += h * 2, it2 += h * 4) {
                    dft(dft(s_dftBuffer, h * 2, it, h) + h * 2, h * 2, it + h, h);
                    for (uint32_t i = 0; i < h * 2; i += 2) {
                        s_dftResultBuffer[i] = {s_dftBuffer[i], s_dftBuffer[i ? h * 2 - i : 0], *(it2 + (h * 2 + i))};
                        s_dftResultBuffer[i] += {s_dftBuffer[h * 2 + i], s_dftBuffer[i ? h * 4 - i : h * 2], *(it2 + i)};
                        s_dftResultBuffer[i + 1] = {s_dftBuffer[i + 1], s_dftBuffer[h * 2 - i - 1], *(it2 + (h * 2 + i + 1))};
                        s_dftResultBuffer[i + 1] += {s_dftBuffer[h * 2 + i + 1], s_dftBuffer[h * 4 - i - 1], *(it2 + (i + 1))};
                    }
                    std::copy_n(idft(s_dftResultBuffer, h * 2), h * 2, it);
                }
            }
            poly _res(res, res + __xs.size());
            _res.reverse();
            return _res;
        }
        poly calc(const poly &__xs) const {
            _initTree(__xs, std::__bit_ceil(std::max<uint32_t>(__xs.size(), size())));
            return _calcTree(*this, __xs.size());
        }
    };
}

#endif