/*
最后修改:
20231206
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEQUENCEHASH__
#define __OY_SEQUENCEHASH__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace STRHASH {
        using size_type = uint32_t;
        struct BaseMap {
            template <typename Tp>
            const Tp &operator()(const Tp &x) const { return x; }
        };
        template <typename Tp, typename Tp::mod_type Base, size_type MAX_LEN>
        static Tp hash_unit(size_type n) {
            struct UnitTable {
                Tp m_val[MAX_LEN + 1];
                UnitTable() {
                    m_val[0] = Tp(1);
                    auto u = Tp::raw(Base);
                    for (size_type i = 1; i <= MAX_LEN; i++) m_val[i] = m_val[i - 1] * u;
                }
            };
            static UnitTable s_unit_table;
            return s_unit_table.m_val[n];
        }
        template <typename Tp, typename Tp::mod_type Base, size_type MAX_LEN>
        static Tp hash_unit_inv(size_type n) {
            struct UnitInvTable {
                Tp m_val[MAX_LEN + 1];
                UnitInvTable() {
                    m_val[0] = Tp::raw(1);
                    auto u = Tp::raw(Base).inv();
                    for (size_type i = 1; i <= MAX_LEN; i++) m_val[i] = m_val[i - 1] * u;
                }
            };
            static UnitInvTable s_unit_inv_table;
            return s_unit_inv_table.m_val[n];
        }
        template <typename Tp, typename Tp::mod_type Base, size_type MAX_LEN>
        static Tp hash_unit_de_inv(size_type n) {
            struct UnitDeInvTable {
                Tp m_val[MAX_LEN + 1];
                UnitDeInvTable() {
                    m_val[0] = Tp::raw(1);
                    auto u = Tp::raw(Base), s = Tp::raw(1);
                    for (size_type i = 1; i <= MAX_LEN; i++) s *= u, m_val[i] = s - Tp::raw(1);
                    for (size_type i = MAX_LEN - 1; i; i--) m_val[i] *= m_val[i + 1];
                    auto inv = m_val[1].inv();
                    s = Tp::raw(1);
                    for (size_type i = 1; i != MAX_LEN; i++) s *= u, m_val[i] = inv * m_val[i + 1], inv *= s - Tp::raw(1);
                }
            };
            static UnitDeInvTable s_unit_de_inv_table;
            return s_unit_de_inv_table.m_val[n];
        }
        template <typename Tp, typename Tp::mod_type Base, size_type MAX_LEN>
        struct SequenceHash {
            using hash_type = SequenceHash<Tp, Base, MAX_LEN>;
            using value_type = Tp;
            size_type m_len;
            Tp m_val;
            static hash_type single(typename Tp::mod_type val) { return {1, Tp::raw(val)}; }
            static hash_type zero(size_type len) { return {len, Tp::raw(0)}; }
            static Tp combine(Tp val1, size_type len1, Tp val2, size_type len2) { return val1 + val2 * unit_of(len1); }
            static Tp unit_of(size_type i) { return hash_unit<Tp, Base, MAX_LEN>(i); }
            static Tp unit_inv_of(size_type i) { return hash_unit_inv<Tp, Base, MAX_LEN>(i); }
            static Tp unit_de_inv_of(size_type i) { return hash_unit_de_inv<Tp, Base, MAX_LEN>(i); }
            SequenceHash() = default;
            SequenceHash(size_type len, Tp val) : m_len(len), m_val(val) {}
            template <typename Iterator, typename Mapping = BaseMap>
            SequenceHash(Iterator first, Iterator last, Mapping &&map = Mapping()) {
                m_len = last - first, m_val = 0;
                for (size_type i = 0; i != m_len; i++) m_val += Tp::raw(map(*(first + i))) * unit_of(i);
            }
            template <typename Mapping = BaseMap>
            SequenceHash(const std::string &s, Mapping &&map = Mapping()) : SequenceHash(s.begin(), s.end(), map) {}
            template <typename ValueType, typename Mapping = BaseMap>
            SequenceHash(const std::vector<ValueType> &v, Mapping &&map = Mapping()) : SequenceHash(v.begin(), v.end(), map) {}
            hash_type append_left(const hash_type &lhs) const { return {lhs.m_len + m_len, lhs.m_val + m_val * unit_of(lhs.m_len)}; }
            hash_type append_right(const hash_type &rhs) const { return {m_len + rhs.m_len, m_val + rhs.m_val * unit_of(m_len)}; }
            hash_type remove_prefix(const hash_type &pre) const { return {m_len - pre.m_len, (m_val - pre.m_val) * unit_inv_of(pre.m_len)}; }
            hash_type remove_suffix(const hash_type &suf) const { return {m_len - suf.m_len, m_val - suf.m_val * unit_of(m_len - suf.m_len)}; }
            hash_type repeat_for(size_type times) const { return {m_len * times, m_val * (unit_of(m_len * times) - Tp::raw(1)) * unit_de_inv_of(m_len)}; }
            bool operator==(const hash_type &rhs) const { return m_len == rhs.m_len && m_val == rhs.m_val; }
            bool operator!=(const hash_type &rhs) const { return m_len != rhs.m_len || m_val != rhs.m_val; }
            bool operator<(const hash_type &rhs) const { return m_len < rhs.m_len || (m_len == rhs.m_len && m_val < rhs.m_val); }
            bool operator<=(const hash_type &rhs) const { return m_len < rhs.m_len || (m_len == rhs.m_len && m_val <= rhs.m_val); }
            bool operator>(const hash_type &rhs) const { return m_len > rhs.m_len || (m_len == rhs.m_len && m_val > rhs.m_val); }
            bool operator>=(const hash_type &rhs) const { return m_len > rhs.m_len || (m_len == rhs.m_len && m_val >= rhs.m_val); }
            friend hash_type operator+(const hash_type &lhs, const hash_type &rhs) { return {lhs.m_len, lhs.m_val + rhs.m_val}; }
            friend hash_type operator-(const hash_type &lhs, const hash_type &rhs) { return {lhs.m_len, lhs.m_val - rhs.m_val}; }
        };
        template <typename Tp, typename Tp::mod_type Base, size_type MAX_LEN>
        struct SequenceHashPresumTable {
            using table_type = SequenceHashPresumTable<Tp, Base, MAX_LEN>;
            using hash_type = SequenceHash<Tp, Base, MAX_LEN>;
            std::vector<Tp> m_presum;
            static size_type lcp(const table_type &t1, size_type i1, const table_type &t2, size_type i2, size_type limit) {
                size_type low = 0, high = limit;
                while (low < high) {
                    size_type mid = (low + high + 1) / 2;
                    if (t1.query(i1, i1 + mid - 1) == t2.query(i2, i2 + mid - 1))
                        low = mid;
                    else
                        high = mid - 1;
                }
                return low;
            }
            static size_type lcp(const table_type &t1, size_type i1, const table_type &t2, size_type i2) { return lcp(t1, i1, t2, i2, std::min<size_type>(t1.m_presum.size() - 1 - i1, t2.m_presum.size() - 1 - i2)); }
            static int compare(const table_type &t1, size_type l1, size_type r1, const table_type &t2, size_type l2, size_type r2) {
                size_type len1 = r1 - l1 + 1, len2 = r2 - l2 + 1, len = lcp(t1, l1, t2, l2, std::min(len1, len2));
                if (len == len1)
                    return len == len2 ? 0 : -1;
                else if (len == len2)
                    return 1;
                else
                    return t1.query(l1 + len, l1 + len) < t2.query(l2 + len, l2 + len) ? -1 : 1;
            }
            SequenceHashPresumTable() = default;
            template <typename InitMapping>
            SequenceHashPresumTable(size_type length, InitMapping &&mapping) { resize(length, mapping); }
            template <typename Iterator, typename Mapping = BaseMap>
            SequenceHashPresumTable(Iterator first, Iterator last, Mapping &&map = Mapping()) { reset(first, last, map); }
            template <typename Mapping = BaseMap>
            SequenceHashPresumTable(const std::string &s, Mapping &&map = Mapping()) : SequenceHashPresumTable(s.begin(), s.end(), map) {}
            template <typename ValueType, typename Mapping = BaseMap>
            SequenceHashPresumTable(const std::vector<ValueType> &v, Mapping &&map = Mapping()) : SequenceHashPresumTable(v.begin(), v.end(), map) {}
            template <typename InitMapping>
            void resize(size_type length, InitMapping &&mapping) {
                m_presum.resize(length + 1);
                const auto u = Tp::raw(Base);
                for (size_type i = 0; i != length; i++) m_presum[i + 1] = m_presum[i] + Tp::raw(mapping(i)) * hash_unit<Tp, Base, MAX_LEN>(i);
            }
            template <typename Iterator, typename Mapping = BaseMap>
            void reset(Iterator first, Iterator last, Mapping &&map = Mapping()) {
                resize(last - first, [&](size_type i) { return map(*(first + i)); });
            }
            void reserve(size_type length) { m_presum.clear(), m_presum.reserve(length + 1), m_presum.push_back({}); }
            void clear() { m_presum.clear(), m_presum.push_back({}); }
            void push_back(typename Tp::mod_type elem) { m_presum.push_back(m_presum.back() + Tp::raw(elem) * hash_unit<Tp, Base, MAX_LEN>(m_presum.size() - 1)); }
            void pop_back() { m_presum.pop_back(); }
            hash_type query(size_type left, size_type right) const { return {right - left + 1, (m_presum[right + 1] - m_presum[left]) * hash_unit_inv<Tp, Base, MAX_LEN>(left)}; }
        };
    }
}

#endif