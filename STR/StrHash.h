/*
最后修改:
20240329
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STRHASH__
#define __OY_STRHASH__

#include <algorithm>
#include <cstdint>
#include <vector>

namespace OY {
    namespace STRHASH {
        using size_type = uint32_t;
        struct BaseMap {
            template <typename Tp>
            const Tp &operator()(const Tp &x) const { return x; }
        };
        template <typename Tp, typename Tp::mod_type Base>
        struct StrHashInfo {
            static Tp *s_unit, *s_unit_inv, *s_de;
            static void prepare(size_type length) {
                prepare_unit(length), prepare_unit_inv(length), prepare_de(length);
            }
            static void prepare_unit(size_type length) {
                if (s_unit) delete[] s_unit;
                s_unit = new Tp[length + 1];
                s_unit[0] = Tp::raw(1);
                Tp u = Tp::raw(Base);
                for (size_type i = 0; i != length; i++) s_unit[i + 1] = s_unit[i] * u;
            }
            static void prepare_unit_inv(size_type length) {
                if (s_unit_inv) delete[] s_unit_inv;
                s_unit_inv = new Tp[length + 1];
                s_unit_inv[0] = Tp::raw(1);
                Tp ui = Tp::raw(Base).inv();
                for (size_type i = 0; i != length; i++) s_unit_inv[i + 1] = s_unit_inv[i] * ui;
            }
            static void prepare_de(size_type length) {
                if (s_de) delete[] s_de;
                s_de = new Tp[length + 1];
                s_de[0] = Tp::raw(1);
                Tp u = Tp::raw(Base), one = Tp::raw(1), s = one;
                for (size_type i = 0; i != length; i++) s *= u, s_de[i + 1] = s - one;
                for (size_type i = length - 1; i; i--) s_de[i] *= s_de[i + 1];
                Tp inv = s_de[1].inv();
                s = one;
                for (size_type i = 1; i != length; i++) s *= u, s_de[i] = inv * s_de[i + 1], inv *= s - one;
            }
        };
        template <typename Tp, typename Tp::mod_type Base>
        Tp *StrHashInfo<Tp, Base>::s_unit;
        template <typename Tp, typename Tp::mod_type Base>
        Tp *StrHashInfo<Tp, Base>::s_unit_inv;
        template <typename Tp, typename Tp::mod_type Base>
        Tp *StrHashInfo<Tp, Base>::s_de;
        template <typename Tp, typename Tp::mod_type Base>
        struct StrHash {
            using info_type = StrHashInfo<Tp, Base>;
            using hash_type = StrHash<Tp, Base>;
            using value_type = Tp;
            size_type m_len;
            Tp m_val;
            static hash_type single(typename Tp::mod_type val) { return {1, Tp::raw(val)}; }
            static hash_type zero(size_type len) { return {len, Tp::raw(0)}; }
            static Tp combine(Tp val1, size_type len1, Tp val2, size_type len2) { return val1 + val2 * unit_of(len1); }
            static Tp unit_of(size_type i) { return info_type::s_unit[i]; }
            static Tp unit_inv_of(size_type i) { return info_type::s_unit_inv[i]; }
            static Tp unit_de_inv_of(size_type i) { return info_type::s_de[i]; }
            StrHash() = default;
            StrHash(size_type len, Tp val) : m_len(len), m_val(val) {}
            template <typename Iterator, typename Mapping = BaseMap>
            StrHash(Iterator first, Iterator last, Mapping &&map = Mapping()) {
                m_len = last - first, m_val = 0;
                for (size_type i = 0; i != m_len; i++) m_val += Tp::raw(map(*(first + i))) * unit_of(i);
            }
            template <typename Mapping = BaseMap>
            StrHash(const std::string &s, Mapping &&map = Mapping()) : StrHash(s.begin(), s.end(), map) {}
            template <typename ValueType, typename Mapping = BaseMap>
            StrHash(const std::vector<ValueType> &v, Mapping &&map = Mapping()) : StrHash(v.begin(), v.end(), map) {}
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
        template <typename Tp, typename Tp::mod_type Base>
        struct StrHashPresumTable {
            using table_type = StrHashPresumTable<Tp, Base>;
            using hash_type = StrHash<Tp, Base>;
            using info_type = typename hash_type::info_type;
            struct SubStrHashValue {
                size_type m_left;
                Tp m_val;
                friend bool operator==(const SubStrHashValue &lhs, const Tp &rhs) { return lhs.m_val == rhs * info_type::s_unit[lhs.m_left]; }
                friend bool operator!=(const SubStrHashValue &lhs, const Tp &rhs) { return lhs.m_val != rhs * info_type::s_unit[lhs.m_left]; }
                friend bool operator==(const Tp &lhs, const SubStrHashValue &rhs) { return lhs * info_type::s_unit[rhs.m_left] == rhs.m_val; }
                friend bool operator!=(const Tp &lhs, const SubStrHashValue &rhs) { return lhs * info_type::s_unit[rhs.m_left] != rhs.m_val; }
                friend bool operator==(const SubStrHashValue &lhs, const SubStrHashValue &rhs) { return lhs.m_val * info_type::s_unit[rhs.m_left] == rhs.m_val * info_type::s_unit[lhs.m_left]; }
                friend bool operator!=(const SubStrHashValue &lhs, const SubStrHashValue &rhs) { return lhs.m_val * info_type::s_unit[rhs.m_left] != rhs.m_val * info_type::s_unit[lhs.m_left]; }
                operator Tp() const { return m_val * info_type::s_unit_inv[m_left]; }
            };
            std::vector<Tp> m_presum;
            Tp _raw_dif(size_type left, size_type right) const { return m_presum[right + 1] - m_presum[left]; }
            static size_type lcp(const table_type &t1, size_type i1, const table_type &t2, size_type i2, size_type limit) {
                if (i1 > i2) return lcp(t2, i2, t1, i1, limit);
                size_type low = 0, high = limit, d = i2 - i1;
                while (low < high) {
                    size_type mid = (low + high + 1) / 2;
                    if (t1._raw_dif(i1, i1 + mid - 1) * info_type::s_unit[d] == t2._raw_dif(i2, i2 + mid - 1))
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
            StrHashPresumTable() = default;
            template <typename InitMapping>
            StrHashPresumTable(size_type length, InitMapping &&mapping) { resize(length, mapping); }
            template <typename Iterator, typename Mapping = BaseMap>
            StrHashPresumTable(Iterator first, Iterator last, Mapping &&map = Mapping()) { reset(first, last, map); }
            template <typename Mapping = BaseMap>
            StrHashPresumTable(const std::string &s, Mapping &&map = Mapping()) : StrHashPresumTable(s.begin(), s.end(), map) {}
            template <typename ValueType, typename Mapping = BaseMap>
            StrHashPresumTable(const std::vector<ValueType> &v, Mapping &&map = Mapping()) : StrHashPresumTable(v.begin(), v.end(), map) {}
            template <typename InitMapping>
            void resize(size_type length, InitMapping &&mapping) {
                m_presum.resize(length + 1);
                const auto u = Tp::raw(Base);
                for (size_type i = 0; i != length; i++) m_presum[i + 1] = m_presum[i] + Tp::raw(mapping(i)) * info_type::s_unit[i];
            }
            template <typename Iterator, typename Mapping = BaseMap>
            void reset(Iterator first, Iterator last, Mapping &&map = Mapping()) {
                resize(last - first, [&](size_type i) { return map(*(first + i)); });
            }
            void reserve(size_type length) { m_presum.clear(), m_presum.reserve(length + 1), m_presum.push_back({}); }
            void resize(size_type length) { m_presum.resize(length + 1); }
            void clear() { m_presum.clear(), m_presum.push_back({}); }
            void push_back(typename Tp::mod_type elem) { m_presum.push_back(m_presum.back() + Tp::raw(elem) * info_type::s_unit[m_presum.size() - 1]); }
            void pop_back() { m_presum.pop_back(); }
            SubStrHashValue query_value(size_type left, size_type right) const { return {left, _raw_dif(left, right)}; }
            hash_type query_hash(size_type left, size_type right) const { return {right - left + 1, _raw_dif(left, right) * info_type::s_unit_inv[left]}; }
        };
    }
}

#endif