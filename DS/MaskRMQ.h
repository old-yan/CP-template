/*
最后修改:
20240425
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MASKRMQ__
#define __OY_MASKRMQ__

#include "STTable.h"

namespace OY {
    namespace MASKRMQ {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        using ST::Ignore;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        template <typename ValueType, typename Compare = std::less<ValueType>>
        struct BaseNode {
            using value_type = ValueType;
            static bool comp(const value_type &x, const value_type &y) { return Compare()(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename Node, size_type MAX_LEVEL = 32>
        struct IndexTable {
            using node = Node;
            using value_type = typename node::value_type;
            struct item {
                size_type m_index;
                value_type m_val;
            };
            struct inter_node {
                using value_type = item;
                static item op(const item &x, const item &y) { return !node::comp(x.m_val, y.m_val) ? x : y; }
                item m_item;
                const item &get() const { return m_item; }
                void set(const item &i) { m_item = i; }
            };
            using inter_table_type = ST::Table<inter_node, MAX_LEVEL>;
            std::vector<node> m_raw;
            std::vector<mask_type> m_mask;
            size_type m_size;
            inter_table_type m_inter_table;
            static size_type _inner_query(mask_type mask) { return std::countr_zero(mask); }
            static size_type _inner_query(mask_type mask, size_type l) { return std::countr_zero(mask & -(mask_type(1) << l)); }
            item _make_item(size_type i) const { return {i, m_raw[i].get()}; }
            template <typename InitMapping = Ignore>
            IndexTable(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            IndexTable(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_raw.resize(m_size), m_mask.resize(m_size);
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    node *raw = m_raw.data();
                    mask_type *mask_ptr = m_mask.data();
                    for (size_type i = 0; i != m_size; i++) raw[i].set(mapping(i));
                    size_type stack[MASK_SIZE], i;
                    for (i = 0; i + MASK_SIZE <= m_size; i += MASK_SIZE) {
                        node *cur_raw = raw + i;
                        mask_type *cur_mask = mask_ptr + i, mask = 0;
                        size_type len = 0;
                        for (size_type j = 0; j != MASK_SIZE; j++) {
                            while (len && node::comp(cur_raw[stack[len - 1]].get(), cur_raw[j].get())) mask &= ~(mask_type(1) << stack[--len]);
                            stack[len++] = j, mask |= mask_type(1) << j;
                            cur_mask[j] = mask;
                        }
                    }
                    if (i != m_size) {
                        node *cur_raw = raw + i;
                        mask_type *cur_mask = mask_ptr + i, mask = 0;
                        size_type len = 0;
                        for (size_type j = 0, rem = m_size - i; j != rem; j++) {
                            while (len && node::comp(cur_raw[stack[len - 1]].get(), cur_raw[j].get())) mask &= ~(mask_type(1) << stack[--len]);
                            stack[len++] = j, mask |= mask_type(1) << j;
                            cur_mask[j] = mask;
                        }
                    }
                    size_type tot = (m_size + MASK_SIZE - 1) / MASK_SIZE;
                    m_inter_table.resize(tot, [&](size_type i) { return _make_item(i * MASK_SIZE + _inner_query(mask_ptr[std::min((i + 1) * MASK_SIZE, m_size) - 1])); });
                } else {
                    size_type tot = (m_size + MASK_SIZE - 1) / MASK_SIZE;
                    m_inter_table.resize(tot, [&](size_type i) { return _make_item(i * MASK_SIZE); });
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type query(size_type i) const { return i; }
            size_type query(size_type left, size_type right) const {
                auto choose = [raw = m_raw.data()](size_type i, size_type j) { return !node::comp(raw[i].get(), raw[j].get()) ? i : j; };
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
            const value_type &get(size_type i) const { return m_raw[i].get(); }
            void modify(size_type i, const value_type &val) {
                size_type stack[MASK_SIZE], len = 0, k = i / MASK_SIZE;
                node *raw = m_raw.data(), *cur_raw = raw + k * MASK_SIZE;
                mask_type *cur_mask = m_mask.data() + k * MASK_SIZE, mask = 0;
                raw[i].set(val);
                if (i + 1 == m_size)
                    for (size_type j = i + 1; j % MASK_SIZE; j++) raw[j].set(val);
                for (size_type j = 0; j < MASK_SIZE; j++) {
                    while (len && cur_raw[stack[len - 1]].get() < cur_raw[j].get()) mask &= ~(mask_type(1) << stack[--len]);
                    stack[len++] = j, mask |= mask_type(1) << j;
                    cur_mask[j] = mask;
                }
                m_inter_table.modify(k, _make_item(k * MASK_SIZE + _inner_query(cur_mask[MASK_SIZE - 1])));
            }
        };
        template <typename Node, size_type MAX_LEVEL>
        struct ValueTable {
            using index_table = IndexTable<Node, MAX_LEVEL>;
            using node = typename index_table::node;
            using value_type = typename index_table::value_type;
            index_table m_table;
            template <typename InitMapping = Ignore>
            ValueTable(size_type length = 0, InitMapping mapping = InitMapping()) : m_table(length, mapping) {}
            template <typename Iterator>
            ValueTable(Iterator first, Iterator last) : m_table(first, last) {}
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) { m_table.resize(length, mapping); }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) { m_table.reset(first, last); }
            value_type query(size_type i) const { return m_table.get(i); }
            value_type query(size_type left, size_type right) const { return m_table.get(m_table.query(left, right)); }
            value_type query_all() const { return m_table.get(m_table.query_all()); }
            void modify(size_type i, const value_type &val) { m_table.modify(i, val); }
        };
        template <typename Ostream, typename Node, size_type MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const IndexTable<Node, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i != x.m_size; i++) {
                if (i) out << ", ";
                out << x.get(x.query(i));
            }
            return out << "]";
        }
        template <typename Ostream, typename Node, size_type MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const ValueTable<Node, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i != x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, MASKRMQ::size_type MAX_LEVEL, typename Compare = std::less<Tp>, typename InitMapping = MASKRMQ::Ignore, typename TreeType = MASKRMQ::IndexTable<MASKRMQ::BaseNode<Tp, Compare>, MAX_LEVEL>>
    auto make_MaskRMQ_IndexTable(MASKRMQ::size_type length, Compare comp = Compare(), InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <MASKRMQ::size_type MAX_LEVEL, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Compare = std::less<Tp>, typename TreeType = MASKRMQ::IndexTable<MASKRMQ::BaseNode<Tp, Compare>, MAX_LEVEL>>
    auto make_MaskRMQ_IndexTable(Iterator first, Iterator last, Compare comp = Compare()) -> TreeType { return TreeType(first, last); }
    template <typename Tp, MASKRMQ::size_type MAX_LEVEL, typename Compare = std::less<Tp>, typename InitMapping = MASKRMQ::Ignore, typename TreeType = MASKRMQ::ValueTable<MASKRMQ::BaseNode<Tp, Compare>, MAX_LEVEL>>
    auto make_MaskRMQ_ValueTable(MASKRMQ::size_type length, Compare comp = Compare(), InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <MASKRMQ::size_type MAX_LEVEL, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Compare = std::less<Tp>, typename TreeType = MASKRMQ::ValueTable<MASKRMQ::BaseNode<Tp, Compare>, MAX_LEVEL>>
    auto make_MaskRMQ_ValueTable(Iterator first, Iterator last, Compare comp = Compare()) -> TreeType { return TreeType(first, last); }
    template <typename Tp, MASKRMQ::size_type MAX_LEVEL = 32 - MASKRMQ::MASK_WIDTH>
    using MaskRMQMaxIndexTable = MASKRMQ::IndexTable<MASKRMQ::BaseNode<Tp, std::less<Tp>>, MAX_LEVEL>;
    template <typename Tp, MASKRMQ::size_type MAX_LEVEL = 32 - MASKRMQ::MASK_WIDTH>
    using MaskRMQMinIndexTable = MASKRMQ::IndexTable<MASKRMQ::BaseNode<Tp, std::greater<Tp>>, MAX_LEVEL>;
    template <typename Tp, MASKRMQ::size_type MAX_LEVEL = 32 - MASKRMQ::MASK_WIDTH>
    using MaskRMQMaxValueTable = MASKRMQ::ValueTable<MASKRMQ::BaseNode<Tp, std::less<Tp>>, MAX_LEVEL>;
    template <typename Tp, MASKRMQ::size_type MAX_LEVEL = 32 - MASKRMQ::MASK_WIDTH>
    using MaskRMQMinValueTable = MASKRMQ::ValueTable<MASKRMQ::BaseNode<Tp, std::greater<Tp>>, MAX_LEVEL>;
}

#endif