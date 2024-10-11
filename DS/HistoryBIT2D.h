/*
最后修改:
20241010
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HISTORYBIT2DEX__
#define __OY_HISTORYBIT2DEX__

#include "BIT2D.h"

namespace OY {
    namespace HBIT2D {
        using size_type = BIT2D::size_type;
        template <typename Tp>
        class Tree {
        public:
            struct info {
                Tp m_val, m_hdif;
                info() = default;
                info(Tp val) : m_val(val), m_hdif{} {}
                info(Tp val, Tp hdif) : m_val(val), m_hdif(hdif) {}
                info operator-() const { return {-m_val, -m_hdif}; }
                info operator+(const info &rhs) const { return {m_val + rhs.m_val, m_hdif + rhs.m_hdif}; }
                info operator-(const info &rhs) const { return {m_val - rhs.m_val, m_hdif - rhs.m_hdif}; }
                info operator*(size_type len) const { return {Tp(m_val * len), Tp(m_hdif * len)}; }
                info &operator+=(const info &rhs) {
                    m_val += rhs.m_val, m_hdif += rhs.m_hdif;
                    return *this;
                }
                info &operator-=(const info &rhs) {
                    m_val -= rhs.m_val, m_hdif -= rhs.m_hdif;
                    return *this;
                }
                Tp calc(size_type ver_cnt) const { return m_val * ver_cnt + m_hdif; }
            };
        private:
            BIT2D::Tree<info> m_bit;
            size_type m_ver_cnt;
        public:
            Tree() = default;
            Tree(size_type row, size_type column) { resize(row, column); }
            template <typename InitMapping>
            Tree(size_type row, size_type column, InitMapping mapping) { resize(row, column, mapping); }
            void resize(size_type row, size_type column) {
                if (row && column) m_bit.resize(row, column), m_ver_cnt = 1;
            }
            template <typename InitMapping>
            void resize(size_type row, size_type column, InitMapping mapping) {
                if (row && column) m_bit.resize(row, column, mapping), m_ver_cnt = 1;
            }
            size_type version_count() const { return m_ver_cnt; }
            size_type row() const { return m_bit.row(); }
            size_type column() const { return m_bit.column(); }
            void copy_version() { m_ver_cnt++; }
            void add(size_type i, size_type j, const Tp &inc) { m_bit.add(i, j, {inc, Tp((-inc) * (m_ver_cnt - 1))}); }
            Tp presum(size_type i, size_type j) const { return m_bit.presum(i, j).m_val; }
            Tp query(size_type i, size_type j) const { return m_bit.query(i, j).m_val; }
            Tp query(size_type row1, size_type row2, size_type column1, size_type column2) const { return m_bit.query(row1, row2, column1, column2).m_val; }
            Tp query_all() const { return m_bit.query_all().m_val; }
            Tp history_presum(size_type i, size_type j) const { return m_bit.presum(i, j).calc(m_ver_cnt); }
            Tp history_query(size_type i, size_type j) const { return m_bit.query(i, j).calc(m_ver_cnt); }
            Tp history_query(size_type row1, size_type row2, size_type column1, size_type column2) const { return m_bit.query(row1, row2, column1, column2).calc(m_ver_cnt); }
            Tp history_query_all() const { return m_bit.query_all().calc(m_ver_cnt); }
        };
        template <typename Ostream, typename Tp>
        Ostream &operator<<(Ostream &out, const Tree<Tp> &x) {
            out << "[";
            for (size_type i = 0; i != x.row(); i++)
                for (size_type j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        }
    }
}

#endif