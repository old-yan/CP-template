/*
最后修改:
20240609
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ROLLBACKADJDIFF__
#define __OY_ROLLBACKADJDIFF__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace RollbackAdjDiff {
        using size_type = uint32_t;
        template <typename Tp>
        class Table {
            mutable std::vector<Tp> m_sum;
        public:
            Table() { m_sum.push_back({}); }
            Table(size_type length) { resize(length); }
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) { m_sum.clear(), m_sum.assign(length + 1, {}); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_sum.clear(), m_sum.reserve(length + 1);
                m_sum.push_back({});
                for (size_type i = 0; i < length; i++) m_sum.push_back(m_sum.back() + mapping(i));
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            Tp query(size_type i) const { return m_sum[i + 1] - m_sum[i]; }
            Tp query(size_type left, size_type right) const { return m_sum[right + 1] - m_sum[left]; }
            Tp query_all() const { return m_sum.back(); }
            void push_back(Tp x) { m_sum.push_back(m_sum.back() + x); }
            void pop_back() { m_sum.pop_back(); }
            void clear() { m_sum.clear(), m_sum.push_back({}); }
            size_type size() const { return m_sum.size() - 1; }
            bool empty() const { return m_sum.size() == 1; }
            void reserve(size_type capacity) { m_sum.reserve(capacity + 1); }
        };
        template <typename Ostream, typename Tp>
        Ostream &operator<<(Ostream &out, const Table<Tp> &x) {
            out << "[";
            for (size_type i = 0; i < x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        };
    }
}

#endif