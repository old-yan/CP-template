/*
最后修改:
20240912
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ACCUMULATOR__
#define __OY_ACCUMULATOR__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace ACC {
        using size_type = uint32_t;
        template <typename Tp, typename Operation>
        struct BaseSemiGroup {
            using value_type = Tp;
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename SemiGroup, bool Prefix, bool Suffix>
        class Table {
        public:
            using group = SemiGroup;
            using value_type = typename group::value_type;
        private:
            size_type m_size;
            std::vector<value_type> m_data, m_prefix, m_suffix;
            void _update(size_type i) {
                if constexpr (Prefix) {
                    m_prefix[i] = i ? group::op(m_prefix[i - 1], m_data[i]) : m_data[i];
                    for (size_type j = i + 1; j != m_size; j++) m_prefix[j] = group::op(m_prefix[j - 1], m_data[j]);
                }
                if constexpr (Suffix) {
                    m_suffix[i] = i + 1 < m_size ? group::op(m_data[i], m_suffix[i + 1]) : m_data[i];
                    for (size_type j = i - 1; ~j; j--) m_suffix[j] = group::op(m_data[j], m_suffix[j + 1]);
                }
            }
        public:
            Table() = default;
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_data.resize(m_size);
                for (size_type i = 0; i != m_size; i++) m_data[i] = mapping(i);
                if constexpr (Prefix) {
                    m_prefix = m_data;
                    for (size_type i = 1; i != m_size; i++) m_prefix[i] = group::op(m_prefix[i - 1], m_prefix[i]);
                }
                if constexpr (Suffix) {
                    m_suffix = m_data;
                    for (size_type i = m_size - 2; ~i; i--) m_suffix[i] = group::op(m_suffix[i], m_suffix[i + 1]);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type size() const { return m_size; }
            void modify(size_type i, value_type val) { m_data[i] = val, _update(i); }
            value_type prefix(size_type i) const {
                static_assert(Prefix, "Prefix Tag Must Be True");
                return m_prefix[i];
            }
            value_type suffix(size_type i) const {
                static_assert(Suffix, "Prefix Tag Must Be True");
                return m_suffix[i];
            }
            value_type query(size_type i) const { return m_data[i]; }
            value_type query(size_type left, size_type right) const {
                value_type res = m_data[left];
                for (size_type i = left + 1; i <= right; i++) res = group::op(res, m_data[i]);
                return res;
            }
            value_type query_all() const {
                if constexpr (Prefix)
                    return m_prefix[m_size - 1];
                else if constexpr (Suffix)
                    return m_suffix[0];
                else {
                    value_type res = m_data[0];
                    for (size_type i = 1; i != m_size; i++) res = group::op(res, m_data[i]);
                    return res;
                }
            }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_data[left];
                if (!judge(val)) return left - 1;
                while (++left != m_size) {
                    val = group::op(val, m_data[left]);
                    if (!judge(val)) break;
                }
                return left - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_data[right];
                if (!judge(val)) return right + 1;
                while (~--right) {
                    val = group::op(m_data[right], val);
                    if (!judge(val)) break;
                }
                return right + 1;
            }
        };
        template <typename Ostream, typename SemiGroup, bool Prefix, bool Suffix>
        Ostream &operator<<(Ostream &out, const Table<SemiGroup, Prefix, Suffix> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, bool Prefix = true, bool Suffix = true, typename Operation, typename InitMapping, typename TreeType = ACC::Table<ACC::BaseSemiGroup<Tp, Operation>, Prefix, Suffix>>
    auto make_Accumulator(ACC::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <bool Prefix = true, bool Suffix = true, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ACC::Table<ACC::BaseSemiGroup<Tp, Operation>, Prefix, Suffix>>
    auto make_Accumulator(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename SemiGroup>
    using PreSufTable = ACC::Table<SemiGroup, true, true>;
}

#endif
