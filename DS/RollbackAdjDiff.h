/*
最后修改:
20241008
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
        struct AddCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            static Tp inverse(const Tp &x) { return -x; }
        };
        template <typename Tp>
        struct BitxorCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x ^ y; }
            static Tp inverse(const Tp &x) { return x; }
        };
        template <typename CommutativeGroup>
        class Table {
        public:
            using group = CommutativeGroup;
            using value_type = typename group::value_type;
        private:
            mutable std::vector<value_type> m_sum;
        public:
            Table() : m_sum{group::identity()} {}
            Table(size_type length) { resize(length); }
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) { m_sum.clear(), m_sum.assign(length + 1, group::identity()); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_sum.clear(), m_sum.reserve(length + 1);
                m_sum.push_back(group::identity());
                for (size_type i = 0; i != length; i++) m_sum.push_back(group::op(m_sum.back(), mapping(i)));
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            value_type query(size_type i) const { return group::op(m_sum[i + 1], group::inverse(m_sum[i])); }
            value_type query(size_type left, size_type right) const { return group::op(m_sum[right + 1], group::inverse(m_sum[left])); }
            value_type query_all() const { return m_sum.back(); }
            void push_back(value_type x) { m_sum.push_back(group::op(m_sum.back(), x)); }
            void pop_back() { m_sum.pop_back(); }
            void clear() { m_sum.clear(), m_sum.push_back(group::identity()); }
            size_type size() const { return m_sum.size() - 1; }
            bool empty() const { return m_sum.size() == 1; }
            void reserve(size_type capacity) { m_sum.reserve(capacity + 1); }
        };
        template <typename Ostream, typename CommutativeGroup>
        Ostream &operator<<(Ostream &out, const Table<CommutativeGroup> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        };
    }
    template <typename Tp>
    using RollbackSumTable = RollbackAdjDiff::Table<RollbackAdjDiff::AddCommutativeGroup<Tp>>;
    template <typename Tp>
    using RollbackBitxorTable = RollbackAdjDiff::Table<RollbackAdjDiff::BitxorCommutativeGroup<Tp>>;
}

#endif