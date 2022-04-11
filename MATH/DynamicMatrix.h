#ifndef __OY_DYNAMICMATRIX__
#define __OY_DYNAMICMATRIX__

#include <cassert>
#include <functional>

namespace OY {
    template <typename _Tp>
    struct DynamicMatrix {
        std::vector<std::vector<_Tp>> m_val;
        DynamicMatrix(const std::vector<std::vector<_Tp>> &__vals) : m_val(__vals) {}
        DynamicMatrix(uint32_t __m, uint32_t __n, _Tp __val = _Tp()) : m_val(__m, std::vector<_Tp>(__n, __val)) {}
        uint32_t row() const { return m_val.size(); }
        uint32_t column() const { return m_val[0].size(); }
        static DynamicMatrix<_Tp> unit(uint32_t __m) {
            DynamicMatrix<_Tp> res(__m, __m, 0);
            for (uint32_t i = 0; i < __m; i++) res[i][i] = 1;
            return res;
        }
        _Tp *operator[](uint32_t __i) { return m_val[__i].data(); }
        const _Tp *operator[](uint32_t __i) const { return m_val[__i].data(); }
        DynamicMatrix<_Tp> &operator+=(_Tp __a) {
            for (std::vector<_Tp> &row : m_val)
                for (_Tp &a : row) a += __a;
            return *this;
        }
        DynamicMatrix<_Tp> &operator-=(_Tp __a) {
            for (std::vector<_Tp> &row : m_val)
                for (_Tp &a : row) a -= __a;
            return *this;
        }
        DynamicMatrix<_Tp> &operator*=(_Tp __a) {
            for (std::vector<_Tp> &row : m_val)
                for (_Tp &a : row) a *= __a;
            return *this;
        }
        DynamicMatrix<_Tp> &operator+=(const DynamicMatrix<_Tp> &__other) {
            assert(row() == __other.row() && column() == __other.column());
            const uint32_t r = row(), c = column();
            for (uint32_t i = 0; i < r; i++)
                for (uint32_t j = 0; j < c; j++) m_val[i][j] += __other[i][j];
            return *this;
        }
        DynamicMatrix<_Tp> &operator-=(const DynamicMatrix<_Tp> &__other) {
            assert(row() == __other.row() && column() == __other.column());
            const uint32_t r = row(), c = column();
            for (uint32_t i = 0; i < r; i++)
                for (uint32_t j = 0; j < c; j++) m_val[i][j] -= __other[i][j];
            return *this;
        }
        DynamicMatrix<_Tp> pow(uint64_t __n) const {
            assert(row() == column());
            DynamicMatrix<_Tp> res = unit(row()), a = *this;
            while (__n) {
                if (__n & 1) res = res * a;
                a = a * a;
                __n >>= 1;
            }
            return res;
        }
        template <typename _Fp>
        friend DynamicMatrix<_Tp> operator+(const DynamicMatrix<_Tp> &__a, const _Fp &__b) { return DynamicMatrix<_Tp>(__a) += __b; }
        template <typename _Fp>
        friend DynamicMatrix<_Tp> operator-(const DynamicMatrix<_Tp> &__a, const _Fp &__b) { return DynamicMatrix<_Tp>(__a) -= __b; }
        friend DynamicMatrix<_Tp> operator*(const DynamicMatrix<_Tp> &__a, const _Tp &__b) { return DynamicMatrix<_Tp>(__a) *= __b; }
        friend DynamicMatrix<_Tp> operator*(const DynamicMatrix<_Tp> &__a, const DynamicMatrix<_Tp> &__b) {
            assert(__a.column() == __b.row());
            const uint32_t r = __a.row(), c = __a.column(), l = __b.column();
            DynamicMatrix<_Tp> res(r, l, 0);
            for (uint32_t i = 0; i < r; i++)
                for (uint32_t j = 0; j < c; j++) {
                    _Tp a = __a[i][j];
                    for (uint32_t k = 0; k < l; k++) res[i][k] += a * __b[j][k];
                }
            return res;
        }
        friend bool operator==(const DynamicMatrix<_Tp> &__a, const DynamicMatrix<_Tp> &__b) { return __a.m_val == __b.m_val; }
        friend bool operator!=(const DynamicMatrix<_Tp> &__a, const DynamicMatrix<_Tp> &__b) { return __a.m_val != __b.m_val; }
    };
};

#endif