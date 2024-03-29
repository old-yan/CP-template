/*
最后修改:
20240329
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HASHLCP__
#define __OY_HASHLCP__

#include "StrHash.h"

namespace OY {
    namespace HASHLCP {
        using size_type = uint32_t;
        template <typename Sequence, typename Tp, typename Tp::mod_type Base>
        struct LCP {
            using table_type = STRHASH::StrHashPresumTable<Tp, Base>;
            size_type m_length;
            Sequence m_sequence;
            table_type m_table;
            template <typename Iterator, typename Mapping = STRHASH::BaseMap>
            LCP(Iterator first, Iterator last, Mapping &&map = Mapping()) : m_length(last - first), m_sequence(first, last), m_table(first, last, map) {}
            template <typename Mapping = STRHASH::BaseMap>
            LCP(const std::vector<int> &seq, Mapping &&map = Mapping()) : LCP(seq.begin(), seq.end(), map) {}
            template <typename Mapping = STRHASH::BaseMap>
            LCP(const std::string &seq, Mapping &&map = Mapping()) : LCP(seq.begin(), seq.end(), map) {}
            size_type lcp(size_type a, size_type b, size_type limit) const {
                size_type low = 0, high = limit;
                while (low < high) {
                    size_type mid = (low + high + 1) / 2;
                    if (m_table.query_value(a, a + mid - 1) == m_table.query_value(b, b + mid - 1))
                        low = mid;
                    else
                        high = mid - 1;
                }
                return low;
            }
            size_type lcp(size_type a, size_type b) const {
                if (a == b) return m_length - a;
                return lcp(a, b, m_length - std::max(a, b));
            }
            int compare(size_type l1, size_type r1, size_type l2, size_type r2) const {
                size_type len1 = r1 - l1 + 1, len2 = r2 - l2 + 1, len = lcp(l1, l2, std::min(len1, len2));
                if (len == len1)
                    return len == len2 ? 0 : -1;
                else if (len == len2)
                    return 1;
                else
                    return m_sequence[l1 + len] < m_sequence[l2 + len] ? -1 : 1;
            }
        };
    }
    template <typename Tp, typename Tp::mod_type Base, typename Mapping = STRHASH::BaseMap, typename TableType = HASHLCP::LCP<std::vector<int>, Tp, Base>>
    auto make_hash_LCP(const std::vector<int> &seq, Mapping &&map = Mapping()) -> TableType { return TableType(seq.begin(), seq.end(), map); }
    template <typename Tp, typename Tp::mod_type Base, typename Mapping = STRHASH::BaseMap, typename TableType = HASHLCP::LCP<std::string, Tp, Base>>
    auto make_hash_LCP(const std::string &seq, Mapping &&map = Mapping()) -> TableType { return TableType(seq.begin(), seq.end(), map); }
    template <typename Tp, typename Tp::mod_type Base, typename ValueType, typename Mapping = STRHASH::BaseMap, typename TableType = HASHLCP::LCP<std::vector<ValueType>, Tp, Base>>
    auto make_hash_LCP(ValueType *first, ValueType *last, Mapping &&map = Mapping()) -> TableType { return TableType(first, last, map); }
}

#endif