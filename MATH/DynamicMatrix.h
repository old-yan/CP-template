/*
最后修改:
20231127
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DYNAMICMATRIX__
#define __OY_DYNAMICMATRIX__

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <valarray>

namespace OY {
    template <typename Tp>
    struct DynamicMatrix {
        uint32_t m_row, m_column;
        std::valarray<Tp> m_val;
        DynamicMatrix() = default;
        DynamicMatrix(uint32_t m, uint32_t n, const std::valarray<Tp> &vals) : m_row(m), m_column(n), m_val(vals) {}
        DynamicMatrix(std::initializer_list<std::initializer_list<Tp>> vals) : m_row(vals.size()), m_column(vals.begin()->size()), m_val(m_row * m_column) {
            size_t i = 0;
            for (auto it = vals.begin(); it != vals.end(); ++it) {
                m_val[std::slice(i, m_column, 1)] = *it;
                i += m_column;
            }
        }
        static DynamicMatrix<Tp> raw(uint32_t m, uint32_t n, Tp val = Tp()) {
            DynamicMatrix<Tp> res;
            res.m_row = m, res.m_column = n, res.m_val.resize(m * n, val);
            return res;
        }
        uint32_t row() const { return m_row; }
        uint32_t column() const { return m_column; }
        static DynamicMatrix<Tp> unit(uint32_t m) {
            auto res = raw(m, m);
            for (uint32_t i = 0; i < m; i++) res[i][i] = 1;
            return res;
        }
        Tp *operator[](uint32_t i) { return &m_val[i * m_column]; }
        const Tp *operator[](uint32_t i) const { return &m_val[i * m_column]; }
        DynamicMatrix<Tp> &operator+=(Tp a) {
            m_val += a;
            return *this;
        }
        DynamicMatrix<Tp> &operator-=(Tp a) {
            m_val -= a;
            return *this;
        }
        DynamicMatrix<Tp> &operator*=(Tp a) {
            m_val *= a;
            return *this;
        }
        DynamicMatrix<Tp> &operator+=(const DynamicMatrix<Tp> &rhs) {
            m_val += rhs.m_val;
            return *this;
        }
        DynamicMatrix<Tp> &operator-=(const DynamicMatrix<Tp> &rhs) {
            m_val -= rhs.m_val;
            return *this;
        }
        DynamicMatrix<Tp> pow(uint64_t n) const {
            assert(row() == column());
            const uint32_t l = row();
            DynamicMatrix<Tp> res = unit(l), a = *this, tmp = raw(l, l);
            while (n) {
                if (n & 1) {
                    tmp.m_val = 0;
                    for (uint32_t i = 0; i != l; i++)
                        for (uint32_t j = 0; j != l; j++) {
                            Tp x = res[i][j];
                            for (uint32_t k = 0; k != l; k++) tmp[i][k] += x * a[j][k];
                        }
                    res.m_val = tmp.m_val;
                }
                if (n >>= 1) {
                    tmp.m_val = 0;
                    for (uint32_t i = 0; i != l; i++)
                        for (uint32_t j = 0; j != l; j++) {
                            Tp x = a[i][j];
                            for (uint32_t k = 0; k != l; k++) tmp[i][k] += x * a[j][k];
                        }
                    a.m_val = tmp.m_val;
                }
            }
            return res;
        }
        template <typename Fp>
        friend DynamicMatrix<Tp> operator+(const DynamicMatrix<Tp> &a, const Fp &b) { return DynamicMatrix<Tp>(a) += b; }
        template <typename Fp>
        friend DynamicMatrix<Tp> operator-(const DynamicMatrix<Tp> &a, const Fp &b) { return DynamicMatrix<Tp>(a) -= b; }
        friend DynamicMatrix<Tp> operator*(const DynamicMatrix<Tp> &a, const Tp &b) { return DynamicMatrix<Tp>(a) *= b; }
        friend DynamicMatrix<Tp> operator*(const DynamicMatrix<Tp> &a, const DynamicMatrix<Tp> &b) {
            assert(a.column() == b.row());
            const uint32_t m = a.row(), n = a.column(), l = b.column();
            auto res = raw(m, l, 0);
            for (uint32_t i = 0; i != m; i++)
                for (uint32_t j = 0; j != n; j++) {
                    Tp x = a[i][j];
                    for (uint32_t k = 0; k != l; k++) res[i][k] += x * b[j][k];
                }
            return res;
        }
        friend bool operator==(const DynamicMatrix<Tp> &a, const DynamicMatrix<Tp> &b) { return a.m_val == b.m_val; }
        friend bool operator!=(const DynamicMatrix<Tp> &a, const DynamicMatrix<Tp> &b) { return a.m_val != b.m_val; }
    };
};

#endif