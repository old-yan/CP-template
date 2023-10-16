/*
最后修改:
20231016
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MASKRMQ__
#define __OY_MASKRMQ__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "CatTree.h"

namespace OY {
    namespace MaskRMQ {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType, typename Compare = std::less<ValueType>>
        struct BaseNode {
            using value_type = ValueType;
            static bool comp(const value_type &x, const value_type &y) { return Compare()(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename Node, typename MaskType, size_type MAX_NODE>
        struct IndexTable {
            using node = Node;
            using value_type = typename node::value_type;
            struct inter_node {
                using value_type = size_type;
                static size_type op(size_type x, size_type y) { return !node::comp(s_buffer[x].get(), s_buffer[y].get()) ? x : y; }
                size_type m_index;
                size_type get() const { return m_index; }
                void set(size_type index) { m_index = index; }
            };
            using inter_table_type = Cat::Table<inter_node, MAX_NODE>;
            static constexpr size_type block_size = sizeof(MaskType) << 3;
            static node s_buffer[MAX_NODE];
            static MaskType s_mask_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_raw;
            MaskType *m_mask;
            size_type m_size;
            inter_table_type m_inter_table;
            static size_type _inner_query(MaskType mask) { return std::countr_zero(mask); }
            static size_type _inner_query(MaskType mask, size_type l) { return std::countr_zero(mask & -(MaskType(1) << l)); }
            size_type _choose(size_type i, size_type j) const { return !node::comp(m_raw[i].get(), m_raw[j].get()) ? i : j; }
            template <typename InitMapping = Ignore>
            IndexTable(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            IndexTable(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_raw = s_buffer + s_use_count, m_mask = s_mask_buffer + s_use_count;
                s_use_count += m_size;
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0; i != m_size; i++) m_raw[i].set(mapping(i));
                    size_type stack[block_size], i;
                    for (i = 0; i + block_size <= m_size; i += block_size) {
                        node *cur_raw = m_raw + i;
                        MaskType *cur_mask = m_mask + i, mask = 0;
                        size_type len = 0;
                        for (size_type j = 0; j != block_size; j++) {
                            while (len && node::comp(cur_raw[stack[len - 1]].get(), cur_raw[j].get())) mask &= ~(MaskType(1) << stack[--len]);
                            stack[len++] = j, mask |= MaskType(1) << j;
                            cur_mask[j] = mask;
                        }
                    }
                    if (i != m_size) {
                        node *cur_raw = m_raw + i;
                        MaskType *cur_mask = m_mask + i, mask = 0;
                        size_type len = 0;
                        for (size_type j = 0, rem = m_size - i; j != rem; j++) {
                            while (len && node::comp(cur_raw[stack[len - 1]].get(), cur_raw[j].get())) mask &= ~(MaskType(1) << stack[--len]);
                            stack[len++] = j, mask |= MaskType(1) << j;
                            cur_mask[j] = mask;
                        }
                    }
                    size_type tot = (m_size + block_size - 1) / block_size;
                    m_inter_table.resize(tot, [&](size_type i) { return m_raw - s_buffer + i * block_size + _inner_query(m_mask[std::min((i + 1) * block_size, m_size) - 1]); });
                } else
                    m_inter_table.resize((m_size + block_size - 1) / block_size, [&](size_type i) { return i * block_size; });
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type query(size_type i) const { return i; }
            size_type query(size_type left, size_type right) const {
                size_type l = left / block_size, r = right / block_size;
                if (l == r)
                    return l * block_size + _inner_query(m_mask[right], left % block_size);
                else if (l + 1 == r) {
                    size_type a = l * block_size + _inner_query(m_mask[(l + 1) * block_size - 1], left % block_size), b = r * block_size + _inner_query(m_mask[right]);
                    return _choose(a, b);
                } else {
                    size_type a = l * block_size + _inner_query(m_mask[(l + 1) * block_size - 1], left % block_size), b = m_inter_table.query(l + 1, r - 1) - (m_raw - s_buffer), c = r * block_size + _inner_query(m_mask[right]);
                    return _choose(_choose(a, b), c);
                }
            }
            size_type query_all() const { return m_inter_table.query_all(); }
            const value_type &get(size_type i) const { return m_raw[i].get(); }
            void modify(size_type i, const value_type &val) {
                size_type stack[block_size], len = 0, k = i / block_size;
                node *cur_raw = m_raw + k * block_size;
                MaskType *cur_mask = m_mask + k * block_size, mask = 0;
                m_raw[i].set(val);
                if (i + 1 == m_size)
                    for (size_type j = i + 1; j % block_size; j++) m_raw[j].set(val);
                for (size_type j = 0; j < block_size; j++) {
                    while (len && cur_raw[stack[len - 1]].get() < cur_raw[j].get()) mask &= ~(MaskType(1) << stack[--len]);
                    stack[len++] = j, mask |= MaskType(1) << j;
                    cur_mask[j] = mask;
                }
                m_inter_table.modify(k, m_raw - s_buffer + k * block_size + _inner_query(m_mask[(k + 1) * block_size - 1]));
            }
        };
        template <typename Node, typename MaskType, size_type MAX_NODE>
        struct ValueTable {
            using node = typename IndexTable<Node, MaskType, MAX_NODE>::node;
            using value_type = typename IndexTable<Node, MaskType, MAX_NODE>::value_type;
            IndexTable<Node, MaskType, MAX_NODE> m_table;
            value_type _choose(const value_type &x, const value_type &y) const { return !node::comp(x, y) ? x : y; }
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
        template <typename Ostream, typename Node, typename MaskType, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const IndexTable<Node, MaskType, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.get(x.query(i));
            }
            return out << "]";
        }
        template <typename Ostream, typename Node, typename MaskType, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const ValueTable<Node, MaskType, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
        template <typename Node, typename MaskType, size_type MAX_NODE>
        typename IndexTable<Node, MaskType, MAX_NODE>::node IndexTable<Node, MaskType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, typename MaskType, size_type MAX_NODE>
        MaskType IndexTable<Node, MaskType, MAX_NODE>::s_mask_buffer[MAX_NODE];
        template <typename Node, typename MaskType, size_type MAX_NODE>
        size_type IndexTable<Node, MaskType, MAX_NODE>::s_use_count;
    }
    template <typename Tp, typename MaskType = uint64_t, MaskRMQ::size_type MAX_NODE = 1 << 20, typename Compare = std::less<Tp>, typename InitMapping = MaskRMQ::Ignore, typename TreeType = MaskRMQ::IndexTable<MaskRMQ::BaseNode<Tp, Compare>, MaskType, MAX_NODE>>
    auto make_MaskRMQ_IndexTable(MaskRMQ::size_type length, Compare comp = Compare(), InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename MaskType = uint64_t, MaskRMQ::size_type MAX_NODE = 1 << 20, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Compare = std::less<Tp>, typename TreeType = MaskRMQ::IndexTable<MaskRMQ::BaseNode<Tp, Compare>, MaskType, MAX_NODE>>
    auto make_MaskRMQ_IndexTable(Iterator first, Iterator last, Compare comp = Compare()) -> TreeType { return TreeType(first, last); }
    template <typename Tp, typename MaskType = uint64_t, MaskRMQ::size_type MAX_NODE = 1 << 20, typename Compare = std::less<Tp>, typename InitMapping = MaskRMQ::Ignore, typename TreeType = MaskRMQ::ValueTable<MaskRMQ::BaseNode<Tp, Compare>, MaskType, MAX_NODE>>
    auto make_MaskRMQ_ValueTable(MaskRMQ::size_type length, Compare comp = Compare(), InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename MaskType = uint64_t, MaskRMQ::size_type MAX_NODE = 1 << 20, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Compare = std::less<Tp>, typename TreeType = MaskRMQ::ValueTable<MaskRMQ::BaseNode<Tp, Compare>, MaskType, MAX_NODE>>
    auto make_MaskRMQ_ValueTable(Iterator first, Iterator last, Compare comp = Compare()) -> TreeType { return TreeType(first, last); }
    template <typename Tp, typename MaskType = uint64_t, MaskRMQ::size_type MAX_NODE = 1 << 20>
    using MaskRMQMaxIndexTable = MaskRMQ::IndexTable<MaskRMQ::BaseNode<Tp, std::less<Tp>>, MaskType, MAX_NODE>;
    template <typename Tp, typename MaskType = uint64_t, MaskRMQ::size_type MAX_NODE = 1 << 20>
    using MaskRMQMinIndexTable = MaskRMQ::IndexTable<MaskRMQ::BaseNode<Tp, std::greater<Tp>>, MaskType, MAX_NODE>;
    template <typename Tp, typename MaskType = uint64_t, MaskRMQ::size_type MAX_NODE = 1 << 20>
    using MaskRMQMaxValueTable = MaskRMQ::ValueTable<MaskRMQ::BaseNode<Tp, std::less<Tp>>, MaskType, MAX_NODE>;
    template <typename Tp, typename MaskType = uint64_t, MaskRMQ::size_type MAX_NODE = 1 << 20>
    using MaskRMQMinValueTable = MaskRMQ::ValueTable<MaskRMQ::BaseNode<Tp, std::greater<Tp>>, MaskType, MAX_NODE>;
}

#endif