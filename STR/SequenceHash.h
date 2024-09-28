/*
最后修改:
20240610
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEQUENCEHASH__
#define __OY_SEQUENCEHASH__

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <random>
#include <string>
#include <vector>

namespace OY {
    namespace SEQHASH {
        using size_type = uint32_t;
        struct BaseMap {
            template <typename Tp>
            Tp operator()(const Tp &x) const { return x; }
        };
        template <typename... Tps>
        struct SeqHashInfo;
        template <typename Tp>
        struct SeqHashInfo<Tp> {
            using value_type = Tp;
            using mod_type = typename Tp::mod_type;
            mod_type m_base;
            Tp *m_u{}, *m_ui{}, *m_de{};
            Tp _get_base() const { return Tp::raw(m_base); }
            void set_base(mod_type base) { m_base = base; }
            void set_random_base(mod_type min_base = 128) {
                mod_type low = min_base, high = Tp::mod();
                set_base(low + std::chrono::steady_clock::now().time_since_epoch().count() * 11995408973635179863ULL % (high - low));
            }
            void set_random_base_and_prepare(size_type length, mod_type min_base = 128) { set_random_base(min_base), prepare(length); }
            void prepare(size_type length) {
                prepare_unit(length), prepare_unit_inv(length), prepare_de(length);
            }
            void prepare_unit(size_type length) {
                if (m_u) delete[] m_u;
                m_u = new Tp[length + 1];
                m_u[0] = Tp::raw(1);
                Tp u = Tp::raw(m_base);
                for (size_type i = 0; i != length; i++) m_u[i + 1] = m_u[i] * u;
            }
            void prepare_unit_inv(size_type length) {
                if (m_ui) delete[] m_ui;
                m_ui = new Tp[length + 1];
                m_ui[0] = Tp::raw(1);
                Tp ui = Tp::raw(m_base).inv();
                for (size_type i = 0; i != length; i++) m_ui[i + 1] = m_ui[i] * ui;
            }
            void prepare_de(size_type length) {
                if (m_de) delete[] m_de;
                m_de = new Tp[length + 1];
                m_de[0] = Tp::raw(1);
                Tp u = Tp::raw(m_base), one = Tp::raw(1), s = one;
                for (size_type i = 0; i != length; i++) s *= u, m_de[i + 1] = s - one;
                for (size_type i = length - 1; i; i--) m_de[i] *= m_de[i + 1];
                Tp inv = m_de[1].inv();
                s = one;
                for (size_type i = 1; i != length; i++) s *= u, m_de[i] = inv * m_de[i + 1], inv *= s - one;
            }
        };
        template <typename... Tps>
        struct SeqHash {
            using hash_type = SeqHash<Tps...>;
            using info_type = SeqHashInfo<Tps...>;
            using Tp = typename info_type::value_type;
            static info_type s_info;
            size_type m_len;
            Tp m_val;
            static hash_type single(typename Tp::mod_type val) { return {1, Tp::raw(val)}; }
            static hash_type zero(size_type len) { return {len, Tp::raw(0)}; }
            static Tp combine(Tp val1, size_type len1, Tp val2, size_type len2) { return val1 + val2 * unit_of(len1); }
            static Tp unit_of(size_type i) { return s_info.m_u[i]; }
            static Tp unit_inv_of(size_type i) { return s_info.m_ui[i]; }
            static Tp unit_de_inv_of(size_type i) { return s_info.m_de[i]; }
            SeqHash() = default;
            SeqHash(size_type len, Tp val) : m_len(len), m_val(val) {}
            template <typename Iterator, typename Mapping = BaseMap>
            SeqHash(Iterator first, Iterator last, Mapping &&map = Mapping()) {
                m_len = last - first, m_val = 0;
                for (size_type i = 0; i != m_len; i++) m_val += Tp::raw(map(*(first + i))) * unit_of(i);
            }
            template <typename Mapping = BaseMap>
            SeqHash(const std::string &s, Mapping &&map = Mapping()) : SeqHash(s.begin(), s.end(), map) {}
            template <typename ValueType, typename Mapping = BaseMap>
            SeqHash(const std::vector<ValueType> &v, Mapping &&map = Mapping()) : SeqHash(v.begin(), v.end(), map) {}
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
            friend hash_type operator+(const hash_type &lhs, Tp rhs) { return {lhs.m_len, lhs.m_val + rhs}; }
            friend hash_type operator-(const hash_type &lhs, Tp rhs) { return {lhs.m_len, lhs.m_val - rhs}; }
        };
        template <typename... Tps>
        typename SeqHash<Tps...>::info_type SeqHash<Tps...>::s_info;
        template <typename... Tps>
        struct SeqHashPresumTable {
            using table_type = SeqHashPresumTable<Tps...>;
            using hash_type = SeqHash<Tps...>;
            using Tp = typename hash_type::Tp;
            struct SubSeqHashValue {
                size_type m_left;
                Tp m_val;
                friend bool operator==(const SubSeqHashValue &lhs, const Tp &rhs) { return lhs.m_val == rhs * hash_type::unit_of(lhs.m_left); }
                friend bool operator!=(const SubSeqHashValue &lhs, const Tp &rhs) { return lhs.m_val != rhs * hash_type::unit_of(lhs.m_left); }
                friend bool operator==(const Tp &lhs, const SubSeqHashValue &rhs) { return lhs * hash_type::unit_of(rhs.m_left) == rhs.m_val; }
                friend bool operator!=(const Tp &lhs, const SubSeqHashValue &rhs) { return lhs * hash_type::unit_of(rhs.m_left) != rhs.m_val; }
                friend bool operator==(const SubSeqHashValue &lhs, const SubSeqHashValue &rhs) { return lhs.m_val * hash_type::unit_of(rhs.m_left) == rhs.m_val * hash_type::unit_of(lhs.m_left); }
                friend bool operator!=(const SubSeqHashValue &lhs, const SubSeqHashValue &rhs) { return lhs.m_val * hash_type::unit_of(rhs.m_left) != rhs.m_val * hash_type::unit_of(lhs.m_left); }
                operator Tp() const { return m_val * hash_type::unit_inv_of(m_left); }
            };
            std::vector<Tp> m_presum;
            Tp _raw_dif(size_type left, size_type right) const { return m_presum[right + 1] - m_presum[left]; }
            static size_type lcp(const table_type &t1, size_type i1, const table_type &t2, size_type i2, size_type limit) {
                if (i1 > i2) return lcp(t2, i2, t1, i1, limit);
                size_type low = 0, high = limit, d = i2 - i1;
                while (low < high) {
                    size_type mid = (low + high + 1) / 2;
                    if (t1._raw_dif(i1, i1 + mid - 1) * hash_type::unit_of(d) == t2._raw_dif(i2, i2 + mid - 1))
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
                    return Tp(t1.query_value(l1 + len, l1 + len)) < Tp(t2.query_value(l2 + len, l2 + len)) ? -1 : 1;
            }
            SeqHashPresumTable() = default;
            template <typename InitMapping>
            SeqHashPresumTable(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator, typename Mapping = BaseMap>
            SeqHashPresumTable(Iterator first, Iterator last, Mapping &&map = Mapping()) { reset(first, last, map); }
            template <typename Mapping = BaseMap>
            SeqHashPresumTable(const std::string &s, Mapping &&map = Mapping()) : SeqHashPresumTable(s.begin(), s.end(), map) {}
            template <typename ValueType, typename Mapping = BaseMap>
            SeqHashPresumTable(const std::vector<ValueType> &v, Mapping &&map = Mapping()) : SeqHashPresumTable(v.begin(), v.end(), map) {}
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_presum.resize(length + 1);
                const auto u = hash_type::s_info._get_base();
                for (size_type i = 0; i != length; i++) m_presum[i + 1] = m_presum[i] + Tp::raw(mapping(i)) * hash_type::unit_of(i);
            }
            template <typename Iterator, typename Mapping = BaseMap>
            void reset(Iterator first, Iterator last, Mapping &&map = Mapping()) {
                resize(last - first, [&](size_type i) { return map(*(first + i)); });
            }
            void reserve(size_type length) { m_presum.clear(), m_presum.reserve(length + 1), m_presum.push_back({}); }
            void resize(size_type length) { m_presum.resize(length + 1); }
            void clear() { m_presum.clear(), m_presum.push_back({}); }
            void push_back(typename Tp::mod_type elem) { m_presum.push_back(m_presum.back() + Tp::raw(elem) * hash_type::unit_of(m_presum.size() - 1)); }
            void pop_back() { m_presum.pop_back(); }
            SubSeqHashValue query_value(size_type left, size_type right) const { return {left, _raw_dif(left, right)}; }
            hash_type query_hash(size_type left, size_type right) const { return {right - left + 1, _raw_dif(left, right) * hash_type::unit_inv_of(left)}; }
        };
    }
}

#endif