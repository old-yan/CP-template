/*
最后修改:
20240812
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HISTORYBIT__
#define __OY_HISTORYBIT__

#include "BIT.h"

namespace OY {
    namespace HBIT {
        using size_type = BIT::size_type;
        using Ignore = BIT::Ignore;
        template <typename Tp>
        struct Info {
            Tp m_val, m_hdif;
            Info() = default;
            Info(Tp val) : m_val(val), m_hdif{} {}
            Info(Tp val, Tp hdif) : m_val(val), m_hdif(hdif) {}
            Info operator-() const { return {-m_val, -m_hdif}; }
            Info operator-(const Info &rhs) const {
                return {m_val - rhs.m_val, m_hdif - rhs.m_hdif};
            }
            Info operator*(size_type len) const {
                return {Tp(m_val * len), Tp(m_hdif * len)};
            }
            Info &operator+=(const Info &rhs) {
                m_val += rhs.m_val, m_hdif += rhs.m_hdif;
                return *this;
            }
            Info &operator-=(const Info &rhs) {
                m_val -= rhs.m_val, m_hdif -= rhs.m_hdif;
                return *this;
            }
            Tp calc(size_type ver_cnt) const { return m_val * ver_cnt + m_hdif; }
        };
        template <typename Tp, bool RangeUpdate = false, template <typename> typename BufferType = BIT::VectorBuffer>
        class Tree {
            BIT::Tree<Info<Tp>, RangeUpdate, BufferType> m_bit;
            size_type m_ver_cnt;
        public:
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(size_type length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!length) return;
                m_bit.resize(length, mapping);
                m_ver_cnt = 1;
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type version_count() const { return m_ver_cnt; }
            size_type size() const { return m_bit.size(); }
            void copy_version() { m_ver_cnt++; }
            void add(size_type i, const Tp &inc) { m_bit.add(i, {inc, Tp((-inc) * (m_ver_cnt - 1))}); }
            void add(size_type left, size_type right, const Tp &inc) { m_bit.add(left, right, {inc, Tp((-inc) * (m_ver_cnt - 1))}); }
            Tp presum(size_type i) const { return m_bit.presum(i).m_val; }
            Tp query(size_type i) const { return m_bit.query(i).m_val; }
            Tp query(size_type left, size_type right) const { return m_bit.query(left, right).m_val; }
            Tp query_all() const { return m_bit.query_all().m_val; }
            Tp history_presum(size_type i) const { return m_bit.presum(i).calc(m_ver_cnt); }
            Tp history_query(size_type i) const { return m_bit.query(i).calc(m_ver_cnt); }
            Tp history_query(size_type left, size_type right) const { return m_bit.query(left, right).calc(m_ver_cnt); }
            Tp history_query_all() const { return m_bit.query_all().calc(m_ver_cnt); }
        };
        template <typename Ostream, typename Tp, bool RangeUpdate, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Tp, RangeUpdate, BufferType> &x) {
            out << '[';
            for (size_type i = 0; i != x.size(); i++) out << (i ? ", " : "") << x.query(i);
            return out << "]";
        }
    }
    template <typename Tp, bool RangeUpdate, HBIT::size_type BUFFER = 1 << 22>
    using StaticHistoryBIT = HBIT::Tree<Tp, RangeUpdate, BIT::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp, bool RangeUpdate>
    using VectorHistoryBIT = HBIT::Tree<Tp, RangeUpdate>;
}

#endif