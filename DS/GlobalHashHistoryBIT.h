/*
最后修改:
20241009
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GLOBALHASHHISTORYBIT__
#define __OY_GLOBALHASHHISTORYBIT__

#include "GlobalHashBIT.h"

namespace OY {
    namespace GHBIT{
        using GHASH::size_type;
        template <typename Key, typename Mapped, bool RangeUpdate, bool MakeRecord, size_type BUFFER>
        class Tree {
        public:
            struct info {
                Mapped m_val, m_hdif;
                info() = default;
                info(Mapped val) : m_val(val), m_hdif{} {}
                info(Mapped val, Mapped hdif) : m_val(val), m_hdif(hdif) {}
                info operator-() const { return {-m_val, -m_hdif}; }
                info operator-(const info &rhs) const { return {m_val - rhs.m_val, m_hdif - rhs.m_hdif}; }
                info operator*(size_type len) const { return {Mapped(m_val * len), Mapped(m_hdif * len)}; }
                info &operator+=(const info &rhs) {
                    m_val += rhs.m_val, m_hdif += rhs.m_hdif;
                    return *this;
                }
                info &operator-=(const info &rhs) {
                    m_val -= rhs.m_val, m_hdif -= rhs.m_hdif;
                    return *this;
                }
                Mapped calc(size_type ver_cnt) const { return m_val * ver_cnt + m_hdif; }
            };
        private:
            GBIT::Tree<Key, info, RangeUpdate, MakeRecord, BUFFER> m_bit;
            Key m_ver_cnt;
        public:
            Tree() = default;
            Tree(Key length) { resize(length); }
            void resize(Key length) {
                if (length) m_bit.resize(length), m_ver_cnt = 1;
            }
            Key version_count() const { return m_ver_cnt; }
            Key size() const { return m_bit.size(); }
            void copy_version(Key cnt = 1) { m_ver_cnt += cnt; }
            void add(Key i, const Mapped &inc) { m_bit.add(i, {inc, Mapped((-inc) * (m_ver_cnt - 1))}); }
            void add(Key left, Key right, const Mapped &inc) { m_bit.add(left, right, {inc, Mapped((-inc) * (m_ver_cnt - 1))}); }
            Mapped presum(Key i) const { return m_bit.presum(i).m_val; }
            Mapped query(Key i) const { return m_bit.query(i).m_val; }
            Mapped query(Key left, Key right) const { return m_bit.query(left, right).m_val; }
            Mapped query_all() const { return m_bit.query_all().m_val; }
            Mapped history_presum(Key i) const { return m_bit.presum(i).calc(m_ver_cnt); }
            Mapped history_query(Key i) const { return m_bit.query(i).calc(m_ver_cnt); }
            Mapped history_query(Key left, Key right) const { return m_bit.query(left, right).calc(m_ver_cnt); }
            Mapped history_query_all() const { return m_bit.query_all().calc(m_ver_cnt); }
            void clear() { m_bit.clear(); }
        };
    }
}

#endif