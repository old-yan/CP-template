/*
最后修改:
20241220
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LINKBUCKET__
#define __OY_LINKBUCKET__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace LBC {
        using size_type = uint32_t;
        template <bool MaintainSize>
        struct Bucket {
            size_type m_first;
        };
        template <>
        struct Bucket<true> {
            size_type m_first, m_size;
        };
        template <typename Tp, bool MaintainSize = false>
        class Container {
        public:
            struct node {
                Tp m_value;
                size_type m_next;
            };
            struct iterator {
                node *m_item;
                size_type m_id;
                iterator(node *item, size_type id) : m_item(item), m_id(id) {}
                iterator &operator++() {
                    m_id = m_item[m_id].m_next;
                    return *this;
                }
                iterator operator++(int) {
                    iterator old(*this);
                    m_id = m_item[m_id].m_next;
                    return old;
                }
                Tp &operator*() const { return m_item[m_id].m_value; }
                bool operator==(const iterator &rhs) const { return m_id == rhs.m_id; }
                bool operator!=(const iterator &rhs) const { return m_id != rhs.m_id; }
            };
            struct bucket {
                Container<Tp, MaintainSize> *m_lbc;
                size_type m_buc_id;
                bucket(Container<Tp, MaintainSize> *lbc, size_type buc_id) : m_lbc(lbc), m_buc_id(buc_id) {}
                size_type size() const {
                    static_assert(MaintainSize, "MaintainSize Must Be True");
                    return m_lbc->m_buckets[m_buc_id].m_size;
                }
                bool empty() const { return !~m_lbc->m_buckets[m_buc_id].m_first; }
                Tp &front() { return m_lbc->m_item[m_lbc->m_buckets[m_buc_id].m_first].m_value; }
                void push_front(const Tp &val) {
                    m_lbc->m_item[m_lbc->m_cursor].m_value = val;
                    m_lbc->m_item[m_lbc->m_cursor].m_next = m_lbc->m_buckets[m_buc_id].m_first;
                    m_lbc->m_buckets[m_buc_id].m_first = m_lbc->m_cursor++;
                    if constexpr (MaintainSize) m_lbc->m_buckets[m_buc_id].m_size++;
                }
                void pop_front() {
                    m_lbc->m_buckets[m_buc_id].m_first = m_lbc->m_item[m_lbc->m_buckets[m_buc_id].m_first].m_next;
                    if constexpr (MaintainSize) m_lbc->m_buckets[m_buc_id].m_size--;
                }
                iterator begin() const { return iterator(m_lbc->m_item.data(), m_lbc->m_buckets[m_buc_id].m_first); }
                iterator end() const { return iterator(m_lbc->m_item.data(), -1); }
            };
        private:
            std::vector<Bucket<MaintainSize>> m_buckets;
            std::vector<node> m_item;
            size_type m_bucket_cnt, m_cursor;
        public:
            Container(size_type bucket_cnt = 0, size_type item_cnt = 0) { resize(bucket_cnt, item_cnt); }
            void resize(size_type bucket_cnt, size_type item_cnt) {
                if (!(m_bucket_cnt = bucket_cnt)) return;
                Bucket<MaintainSize> id{};
                id.m_first = -1;
                m_buckets.assign(m_bucket_cnt, id), m_item.resize(item_cnt), m_cursor = 0;
            }
            bucket operator[](size_type buc_id) { return bucket(this, buc_id); }
            bucket operator[](size_type buc_id) const { return bucket((Container<Tp, MaintainSize> *)this, buc_id); }
            void swap(size_type buc_id1, size_type buc_id2) { std::swap(m_buckets[buc_id1], m_buckets[buc_id2]); }
            void merge(size_type buc_id, size_type rhs) {
                size_type cur = m_buckets[rhs].m_first;
                if (!~cur) return;
                while (~m_item[cur].m_next) cur = m_item[cur].m_next;
                m_item[cur].m_next = m_buckets[buc_id].m_first, m_buckets[buc_id].m_first = m_buckets[rhs].m_first;
                if constexpr (MaintainSize) m_buckets[buc_id].m_size += m_buckets[rhs].m_size, m_buckets[rhs].m_size = 0;
                m_buckets[rhs].m_first = -1;
            }
            iterator bucket_begin(size_type buc_id) { return iterator(m_item, m_buckets[buc_id].m_first); }
            iterator bucket_end(size_type = 0) { return iterator(m_item, -1); }
        };
    }
}

#endif