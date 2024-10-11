/*
最后修改:
20241010
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GLOBALHASHHISTORYBIT2DEX__
#define __OY_GLOBALHASHHISTORYBIT2DEX__

#include "GlobalHashBIT2D.h"

namespace OY {
    namespace GHBIT2D {
        using size_type = GBIT2D::size_type;
        template <typename Key, typename Mapped, bool RangeUpdate, bool MakeRecord, size_type BUFFER>
        class Tree {
        public:
            struct info {
                Mapped m_val, m_hdif;
                info() = default;
                info(Mapped val) : m_val(val), m_hdif{} {}
                info(Mapped val, Mapped hdif) : m_val(val), m_hdif(hdif) {}
                info operator-() const { return {-m_val, -m_hdif}; }
                info operator+(const info &rhs) const { return {m_val + rhs.m_val, m_hdif + rhs.m_hdif}; }
                info operator-(const info &rhs) const { return {m_val - rhs.m_val, m_hdif - rhs.m_hdif}; }
                info operator*(Key len) const { return {Mapped(m_val * len), Mapped(m_hdif * len)}; }
                info &operator+=(const info &rhs) {
                    m_val += rhs.m_val, m_hdif += rhs.m_hdif;
                    return *this;
                }
                info &operator-=(const info &rhs) {
                    m_val -= rhs.m_val, m_hdif -= rhs.m_hdif;
                    return *this;
                }
                Mapped calc(Key ver_cnt) const { return m_val * ver_cnt + m_hdif; }
            };
        private:
            GBIT2D::Tree<Key, info, RangeUpdate, MakeRecord, BUFFER> m_bit;
            Key m_ver_cnt;
        public:
            Tree() = default;
            Tree(Key row, Key column) { resize(row, column); }
            void resize(Key row, Key column) {
                if (row && column) m_bit.resize(row, column), m_ver_cnt = 1;
            }
            Key version_count() const { return m_ver_cnt; }
            Key row() const { return m_bit.row(); }
            Key column() const { return m_bit.column(); }
            void copy_version(Key cnt = 1) { m_ver_cnt += cnt; }
            void add(Key i, Key j, const Mapped &inc) { m_bit.add(i, j, {inc, Mapped((-inc) * (m_ver_cnt - 1))}); }
            void add(Key row1, Key row2, Key column1, Key column2, const Mapped &inc) { m_bit.add(row1, row2, column1, column2, {inc, Mapped((-inc) * (m_ver_cnt - 1))}); }
            Mapped presum(Key i, Key j) const { return m_bit.presum(i, j).m_val; }
            Mapped query(Key i, Key j) const { return m_bit.query(i, j).m_val; }
            Mapped query(Key row1, Key row2, Key column1, Key column2) const { return m_bit.query(row1, row2, column1, column2).m_val; }
            Mapped query_all() const { return m_bit.query_all().m_val; }
            Mapped history_presum(Key i, Key j) const { return m_bit.presum(i, j).calc(m_ver_cnt); }
            Mapped history_query(Key i, Key j) const { return m_bit.query(i, j).calc(m_ver_cnt); }
            Mapped history_query(Key row1, Key row2, Key column1, Key column2) const { return m_bit.query(row1, row2, column1, column2).calc(m_ver_cnt); }
            Mapped history_query_all() const { return m_bit.query_all().calc(m_ver_cnt); }
        };
    }
}

#endif