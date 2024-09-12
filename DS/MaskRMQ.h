/*
最后修改:
20240904
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MASKRMQ__
#define __OY_MASKRMQ__

#include "SparseTable.h"

namespace OY {
    namespace MASKRMQ {
        using ST::size_type;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        template <typename Tp, typename Compare, size_t MAX_LEVEL = 30 - MASK_WIDTH>
        class IndexTable {
            struct item {
                size_type m_index;
                Tp m_val;
                bool operator<(const item &rhs) const { return Compare()(m_val, rhs.m_val); }
            };
            using inner_table = ST::Table<ST::BaseSemiGroup<item, ST::ChoiceByCompare<item, std::less<item>>>, MAX_LEVEL>;
            std::vector<Tp> m_raw;
            std::vector<mask_type> m_mask;
            size_type m_size;
            inner_table m_inter_table;
            static bool _comp(const Tp &x, const Tp &y) { return Compare()(x, y); }
            static size_type _inner_query(mask_type mask) { return std::countr_zero(mask); }
            static size_type _inner_query(mask_type mask, size_type l) { return std::countr_zero(mask & -(mask_type(1) << l)); }
            item _make_item(size_type i) const { return {i, m_raw[i]}; }
        public:
            IndexTable() = default;
            template <typename InitMapping>
            IndexTable(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            IndexTable(Iterator first, Iterator last) { reset(first, last); }
            size_type size() const { return m_size; }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_raw.resize(m_size), m_mask.resize(m_size);
                auto raw = m_raw.data();
                mask_type *mask_ptr = m_mask.data();
                for (size_type i = 0; i != m_size; i++) raw[i] = mapping(i);
                size_type stack[MASK_SIZE], i;
                for (i = 0; i + MASK_SIZE <= m_size; i += MASK_SIZE) {
                    auto cur_raw = raw + i;
                    mask_type *cur_mask = mask_ptr + i, mask = 0;
                    size_type len = 0;
                    for (size_type j = 0; j != MASK_SIZE; j++) {
                        while (len && _comp(cur_raw[stack[len - 1]], cur_raw[j])) mask &= ~(mask_type(1) << stack[--len]);
                        stack[len++] = j, mask |= mask_type(1) << j;
                        cur_mask[j] = mask;
                    }
                }
                if (i != m_size) {
                    auto cur_raw = raw + i;
                    mask_type *cur_mask = mask_ptr + i, mask = 0;
                    size_type len = 0;
                    for (size_type j = 0, rem = m_size - i; j != rem; j++) {
                        while (len && _comp(cur_raw[stack[len - 1]], cur_raw[j])) mask &= ~(mask_type(1) << stack[--len]);
                        stack[len++] = j, mask |= mask_type(1) << j;
                        cur_mask[j] = mask;
                    }
                }
                size_type tot = (m_size + MASK_SIZE - 1) / MASK_SIZE;
                m_inter_table.resize(tot, [&](size_type i) { return _make_item(i * MASK_SIZE + _inner_query(mask_ptr[std::min((i + 1) * MASK_SIZE, m_size) - 1])); });
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type query(size_type i) const { return i; }
            size_type query(size_type left, size_type right) const {
                auto choose = [raw = m_raw.data()](size_type i, size_type j) { return !_comp(raw[i], raw[j]) ? i : j; };
                const mask_type *mask = m_mask.data();
                size_type l = left / MASK_SIZE, r = right / MASK_SIZE;
                if (l == r)
                    return l * MASK_SIZE + _inner_query(mask[right], left % MASK_SIZE);
                else if (l + 1 == r) {
                    size_type a = l * MASK_SIZE + _inner_query(mask[(l + 1) * MASK_SIZE - 1], left % MASK_SIZE), b = r * MASK_SIZE + _inner_query(mask[right]);
                    return choose(a, b);
                } else {
                    size_type a = l * MASK_SIZE + _inner_query(mask[(l + 1) * MASK_SIZE - 1], left % MASK_SIZE), b = m_inter_table.query(l + 1, r - 1).m_index, c = r * MASK_SIZE + _inner_query(mask[right]);
                    return choose(choose(a, b), c);
                }
            }
            size_type query_all() const { return m_inter_table.query_all().m_index; }
            const Tp &get(size_type i) const { return m_raw[i]; }
            void modify(size_type i, Tp val) {
                size_type stack[MASK_SIZE], len = 0, k = i / MASK_SIZE;
                auto raw = m_raw.data(), cur_raw = raw + k * MASK_SIZE;
                mask_type *cur_mask = m_mask.data() + k * MASK_SIZE, mask = 0;
                raw[i] = val;
                if (i + 1 == m_size)
                    for (size_type j = i + 1; j % MASK_SIZE; j++) raw[j] = val;
                for (size_type j = 0; j < MASK_SIZE; j++) {
                    while (len && _comp(cur_raw[stack[len - 1]], cur_raw[j])) mask &= ~(mask_type(1) << stack[--len]);
                    stack[len++] = j, mask |= mask_type(1) << j;
                    cur_mask[j] = mask;
                }
                m_inter_table.modify(k, _make_item(k * MASK_SIZE + _inner_query(cur_mask[MASK_SIZE - 1])));
            }
        };
        template <typename Tp, typename Compare, size_t MAX_LEVEL = 30 - MASK_WIDTH>
        struct ValueTable {
            using index_table = IndexTable<Tp, Compare, MAX_LEVEL>;
            index_table m_table;
            ValueTable() = default;
            template <typename InitMapping>
            ValueTable(size_type length, InitMapping mapping) : m_table(length, mapping) {}
            template <typename Iterator>
            ValueTable(Iterator first, Iterator last) : m_table(first, last) {}
            size_type size() const { return m_table.size(); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping = InitMapping()) { m_table.resize(length, mapping); }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) { m_table.reset(first, last); }
            Tp query(size_type i) const { return m_table.get(i); }
            Tp query(size_type left, size_type right) const { return m_table.get(m_table.query(left, right)); }
            Tp query_all() const { return m_table.get(m_table.query_all()); }
            void modify(size_type i, Tp val) { m_table.modify(i, val); }
        };
        template <typename Ostream, typename Tp, typename Compare, size_t MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const IndexTable<Tp, Compare, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.get(x.query(i));
            }
            return out << "]";
        }
        template <typename Ostream, typename Tp, typename Compare, size_t MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const ValueTable<Tp, Compare, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, size_t MAX_LEVEL = 30 - MASKRMQ::MASK_WIDTH>
    using MaskRMQMaxIndexTable = MASKRMQ::IndexTable<Tp, std::less<Tp>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30 - MASKRMQ::MASK_WIDTH>
    using MaskRMQMinIndexTable = MASKRMQ::IndexTable<Tp, std::greater<Tp>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30 - MASKRMQ::MASK_WIDTH>
    using MaskRMQMaxValueTable = MASKRMQ::ValueTable<Tp, std::less<Tp>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30 - MASKRMQ::MASK_WIDTH>
    using MaskRMQMinValueTable = MASKRMQ::ValueTable<Tp, std::greater<Tp>, MAX_LEVEL>;
}

#endif