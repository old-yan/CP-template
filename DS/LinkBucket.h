/*
最后修改:
20240424
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
        template <typename Tp>
        class LinkBucket {
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
            struct Bucket {
                LinkBucket *m_lbc;
                size_type m_buc_id;
                Bucket(LinkBucket *lbc, size_type buc_id) : m_lbc(lbc), m_buc_id(buc_id) {}
                bool empty() const { return !~m_lbc->m_bucket[m_buc_id]; }
                Tp &front() { return m_lbc->m_item[m_lbc->m_bucket[m_buc_id]].m_value; }
                void push_front(const Tp &val) {
                    m_lbc->m_item[m_lbc->m_cursor].m_value = val;
                    m_lbc->m_item[m_lbc->m_cursor].m_next = m_lbc->m_bucket[m_buc_id];
                    m_lbc->m_bucket[m_buc_id] = m_lbc->m_cursor++;
                }
                void pop_front() { m_lbc->m_bucket[m_buc_id] = m_lbc->m_item[m_lbc->m_bucket[m_buc_id]].m_next; }
                iterator begin() const { return iterator(m_lbc->m_item.data(), m_lbc->m_bucket[m_buc_id]); }
                iterator end() const { return iterator(m_lbc->m_item.data(), -1); }
            };
            std::vector<size_type> m_bucket;
            std::vector<node> m_item;
            size_type m_bucket_cnt, m_cursor;
        public:
            LinkBucket(size_type bucket_cnt = 0, size_type item_cnt = 0) { resize(bucket_cnt, item_cnt); }
            void resize(size_type bucket_cnt, size_type item_cnt) {
                if (!(m_bucket_cnt = bucket_cnt)) return;
                m_bucket.assign(m_bucket_cnt, -1), m_item.resize(item_cnt), m_cursor = 0;
            }
            Bucket operator[](size_type buc_id) { return Bucket(this, buc_id); }
            Bucket operator[](size_type buc_id) const { return Bucket((LinkBucket<Tp> *)this, buc_id); }
            iterator bucket_begin(size_type buc_id) { return iterator(m_item, m_bucket[buc_id]); }
            iterator bucket_end(size_type = 0) { return iterator(m_item, -1); }
        };
    }
};

#endif