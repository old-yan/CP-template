/*
最后修改:
20240905
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ROLLBACKSPARSETABLE__
#define __OY_ROLLBACKSPARSETABLE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace RollbackST {
        using size_type = uint32_t;
        template <typename Tp, typename Operation>
        struct BaseSemiGroup {
            using value_type = Tp;
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename SemiGroup, size_t MAX_LEVEL = 32>
        class Table {
        public:
            using group = SemiGroup;
            using value_type = typename group::value_type;
        private:
            std::vector<value_type> m_sub[MAX_LEVEL];
            size_type m_size, m_depth;
            void _update(size_type i) {
                if (m_depth) {
                    auto pre = m_sub[0].data();
                    for (size_type j = 1, k = 2; j != m_depth; j++, k <<= 1) {
                        auto cur = m_sub[j].data();
                        for (size_type l1 = i < 1 << j ? 0 : i - (1 << j) + 1, l2 = std::min(i + 1, m_size - (1 << j) + 1); l1 != l2; l1++) cur[l1] = group::op(pre[l1], pre[l1 + (k >> 1)]);
                        pre = cur;
                    }
                }
            }
        public:
            Table() : m_size(0), m_depth(0) {}
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return m_depth = 0, m_sub[0].clear();
                m_depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                m_sub[0].resize(m_size);
                auto sub = m_sub[0].data();
                for (size_type i = 0; i != m_size; i++) sub[i] = mapping(i);
                for (size_type j = 1; j != m_depth; j++) {
                    m_sub[j].resize(m_size - (1 << j) + 1);
                    auto cur = m_sub[j].data(), pre = m_sub[j - 1].data();
                    for (size_type k = 0, l = 1 << (j - 1), end = m_sub[j].size(); k != end; k++, l++) cur[k] = group::op(pre[k], pre[l]);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type capacity) {
                size_type depth = capacity <= 1 ? 1 : std::bit_width(capacity - 1);
                m_sub[0].reserve(capacity);
                for (size_type i = 1; i != depth; i++) m_sub[i].reserve(capacity - (1 << i) + 1);
            }
            size_type size() const { return m_size; }
            bool empty() const { return !m_size; }
            void add(size_type i, const value_type &inc) { m_sub[0][i] = group::op(inc, m_sub[0][i]), _update(i); }
            void modify(size_type i, const value_type &val) { m_sub[0][i] = val, _update(i); }
            void push_back(const value_type &val) {
                m_sub[0].resize(m_size + 1);
                m_sub[0][m_size] = val;
                if (!m_size)
                    m_depth++;
                else {
                    auto pre = m_sub[0].data();
                    for (size_type i = 1, j = 1, index = m_size - 1; i != m_depth; i++, j <<= 1, index -= j) {
                        m_sub[i].resize(index + 1);
                        auto cur = m_sub[i].data();
                        cur[index] = group::op(pre[index], pre[index + j]), pre = cur;
                    }
                    if (!(m_size & (m_size + 1))) {
                        m_sub[m_depth].resize(1);
                        m_sub[m_depth][0] = group::op(pre[0], pre[1 << (m_depth - 1)]);
                        m_depth++;
                    }
                }
                m_size++;
            }
            void pop_back() {
                for (size_type i = 0; i != m_depth; i++) m_sub[i].pop_back();
                if (!(m_size & (m_size - 1))) m_depth--;
                m_size--;
            }
            value_type query(size_type i) const { return m_sub[0][i]; }
            value_type query(size_type left, size_type right) const {
                size_type d = std::bit_width((right - left) >> 1);
                return group::op(m_sub[d][left], m_sub[d][right - (1 << d) + 1]);
            }
            value_type query_all() const { return query(0, m_size - 1); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[0][left];
                if (!judge(val)) return left - 1;
                size_type d = std::bit_width(m_size - left - 1);
                if (d) {
                    value_type a = group::op(val, m_sub[d - 1][left + 1]);
                    if (judge(a))
                        val = a, d = std::bit_width(m_size - left - 1 - (1 << (d - 1))), left = m_size - (1 << d);
                    else
                        d--;
                }
                for (; d; d--) {
                    value_type a = group::op(val, m_sub[d - 1][left + 1]);
                    if (judge(a)) val = a, left += 1 << (d - 1);
                }
                return left;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[0][right];
                if (!judge(val)) return right + 1;
                size_type d = std::bit_width(right);
                if (d) {
                    value_type a = group::op(m_sub[d - 1][right - (1 << (d - 1))], val);
                    if (judge(a))
                        val = a, d = std::bit_width(right - (1 << (d - 1))), right = (1 << d) - 1;
                    else
                        d--;
                }
                for (; d; d--) {
                    value_type a = group::op(m_sub[d - 1][right - (1 << (d - 1))], val);
                    if (judge(a)) val = a, right -= 1 << (d - 1);
                }
                return right;
            }
        };
        template <typename Ostream, typename SemiGroup, size_t MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const Table<SemiGroup, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, size_t MAX_LEVEL = 30, typename Operation, typename InitMapping, typename TreeType = RollbackST::Table<RollbackST::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_RollbackSTTable(RollbackST::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <size_t MAX_LEVEL = 30, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = RollbackST::Table<RollbackST::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_RollbackSTTable(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackSTMaxTable = RollbackST::Table<RollbackST::BaseSemiGroup<Tp, RollbackST::ChoiceByCompare<Tp, std::less<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackSTMinTable = RollbackST::Table<RollbackST::BaseSemiGroup<Tp, RollbackST::ChoiceByCompare<Tp, std::greater<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackSTGcdTable = RollbackST::Table<RollbackST::BaseSemiGroup<Tp, RollbackST::FpTransfer<Tp, std::gcd<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackSTLcmTable = RollbackST::Table<RollbackST::BaseSemiGroup<Tp, RollbackST::FpTransfer<Tp, std::lcm<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackSTBitAndTable = RollbackST::Table<RollbackST::BaseSemiGroup<Tp, std::bit_and<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using RollbackSTBitOrTable = RollbackST::Table<RollbackST::BaseSemiGroup<Tp, std::bit_or<Tp>>, MAX_LEVEL>;
}

#endif