#ifndef __OY_STATICMATRIX__
#define __OY_STATICMATRIX__

#include <cstdint>
#include <functional>

namespace OY {
    template <typename _Tp, uint32_t _M, uint32_t _N>
    struct StaticMatrix {
        _Tp m_val[_M][_N];
        static constexpr uint32_t row() { return _M; }
        static constexpr uint32_t column() { return _N; }
        static StaticMatrix<_Tp, _M, _N> raw(_Tp __a) {
            StaticMatrix<_Tp, _M, _N> res;
            for (uint32_t i = 0; i < _M; i++)
                for (uint32_t j = 0; j < _N; j++) res[i][j] = __a;
            return res;
        }
        static StaticMatrix<_Tp, _M, _M> unit() {
            static_assert(_M == _N);
            StaticMatrix<_Tp, _M, _M> res{};
            for (uint32_t i = 0; i < _M; i++) res[i][i] = 1;
            return res;
        }
        _Tp *operator[](uint32_t __i) { return m_val[__i]; }
        const _Tp *operator[](uint32_t __i) const { return m_val[__i]; }
        StaticMatrix<_Tp, _M, _N> &operator+=(_Tp __a) {
            for (uint32_t i = 0; i < _M; i++)
                for (uint32_t j = 0; j < _N; j++) m_val[i][j] += __a;
            return *this;
        }
        StaticMatrix<_Tp, _M, _N> &operator-=(_Tp __a) {
            for (uint32_t i = 0; i < _M; i++)
                for (uint32_t j = 0; j < _N; j++) m_val[i][j] -= __a;
            return *this;
        }
        StaticMatrix<_Tp, _M, _N> &operator*=(_Tp __a) {
            for (uint32_t i = 0; i < _M; i++)
                for (uint32_t j = 0; j < _N; j++) m_val[i][j] *= __a;
            return *this;
        }
        StaticMatrix<_Tp, _M, _N> &operator+=(const StaticMatrix<_Tp, _M, _N> &__other) {
            for (uint32_t i = 0; i < _M; i++)
                for (uint32_t j = 0; j < _N; j++) m_val[i][j] += __other[i][j];
            return *this;
        }
        StaticMatrix<_Tp, _M, _N> &operator-=(const StaticMatrix<_Tp, _M, _N> &__other) {
            for (uint32_t i = 0; i < _M; i++)
                for (uint32_t j = 0; j < _N; j++) m_val[i][j] -= __other[i][j];
            return *this;
        }
        StaticMatrix<_Tp, _M, _M> pow(uint64_t __n) const {
            static_assert(_M == _N);
            StaticMatrix<_Tp, _M, _M> res = unit(), a = *this;
            while (__n) {
                if (__n & 1) res = res * a;
                a = a * a;
                __n >>= 1;
            }
            return res;
        }
        template <typename _Fp>
        friend StaticMatrix<_Tp, _M, _N> operator+(const StaticMatrix<_Tp, _M, _N> &__a, const _Fp &__b) { return StaticMatrix<_Tp, _M, _N>(__a) += __b; }
        template <typename _Fp>
        friend StaticMatrix<_Tp, _M, _N> operator-(const StaticMatrix<_Tp, _M, _N> &__a, const _Fp &__b) { return StaticMatrix<_Tp, _M, _N>(__a) -= __b; }
        friend StaticMatrix<_Tp, _M, _N> operator*(const StaticMatrix<_Tp, _M, _N> &__a, const _Tp &__b) { return StaticMatrix<_Tp, _M, _N>(__a) *= __b; }
        template <uint32_t _L>
        friend StaticMatrix<_Tp, _M, _L> operator*(const StaticMatrix<_Tp, _M, _N> &__a, const StaticMatrix<_Tp, _N, _L> &__b) {
            StaticMatrix<_Tp, _M, _L> res{};
            for (uint32_t i = 0; i < _M; i++)
                for (uint32_t j = 0; j < _N; j++) {
                    _Tp a = __a[i][j];
                    for (uint32_t k = 0; k < _L; k++) res[i][k] += a * __b[j][k];
                }
            return res;
        }
        friend bool operator==(const StaticMatrix<_Tp, _M, _N> &__a, const StaticMatrix<_Tp, _M, _N> &__b) {
            for (uint32_t i = 0; i < _M; i++)
                for (uint32_t j = 0; j < _N; j++)
                    if (__a[i][j] != __b[i][j]) return false;
            return true;
        }
        friend bool operator!=(const StaticMatrix<_Tp, _M, _N> &__a, const StaticMatrix<_Tp, _M, _N> &__b) {
            for (uint32_t i = 0; i < _M; i++)
                for (uint32_t j = 0; j < _N; j++)
                    if (__a[i][j] != __b[i][j]) return true;
            return false;
        }
    };
};

#endif