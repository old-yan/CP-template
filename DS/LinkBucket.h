/*
最后修改:
20230913
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

namespace OY {
    namespace LBC {
        using size_type = uint32_t;
        struct Ignore {};
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Func, typename Para, typename = void>
        struct Can_call : std::false_type {};
        template <typename Func, typename Para>
        struct Can_call<Func, Para, void_t<decltype(std::declval<Func>()(std::declval<Para>()))>> : std::true_type {};
        template <typename Tp, size_type MAX_BUCKET = 1 << 20, size_type MAX_NODE = 1 << 22>
        struct LinkBucket {
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
                const LinkBucket *m_lbc;
                size_type m_buc_id;
                Bucket(const LinkBucket *lbc, size_type buc_id) : m_lbc(lbc), m_buc_id(buc_id) {}
                bool empty() const { return !~m_lbc->m_bucket[m_buc_id]; }
                template <typename Modify = Ignore>
                void push_back(Modify modify = Modify()) {
                    if constexpr (!std::is_same<Modify, Ignore>::value)
                        if constexpr (Can_call<Modify, node *>::value)
                            modify(m_lbc.m_item + m_lbc->m_cursor);
                        else
                            m_lbc->m_item[m_lbc->m_cursor].m_value = modify;
                    m_lbc->m_item[m_lbc->m_cursor].m_next = m_lbc->m_bucket[m_buc_id];
                    m_lbc->m_bucket[m_buc_id] = m_lbc->m_cursor++;
                }
                iterator begin() const { return iterator(m_lbc->m_item, m_lbc->m_bucket[m_buc_id]); }
                iterator end() const { return iterator(m_lbc->m_item, -1); }
            };
            static size_type s_bucket_buffer[MAX_BUCKET], s_bucket_use_count, s_use_count;
            static node s_buffer[MAX_NODE];
            size_type m_bucket_cnt, *m_bucket;
            node *m_item;
            mutable size_type m_cursor;
            LinkBucket(size_type bucket_cnt = 0, size_type item_cnt = 0) { resize(bucket_cnt, item_cnt); }
            void resize(size_type bucket_cnt, size_type item_cnt) {
                if (!(m_bucket_cnt = bucket_cnt)) return;
                m_bucket = s_bucket_buffer + s_bucket_use_count, s_bucket_use_count += m_bucket_cnt;
                m_cursor = 0, m_item = s_buffer + s_use_count, s_use_count += item_cnt;
                std::fill_n(m_bucket, m_bucket_cnt, -1);
            }
            Bucket operator[](size_type buc_id) const { return Bucket(this, buc_id); }
            iterator bucket_begin(size_type buc_id) const { return iterator(m_item, m_bucket[buc_id]); }
            iterator bucket_end(size_type = 0) const { return iterator(m_item, -1); }
        };
        template <typename Tp, size_type MAX_BUCKET, size_type MAX_NODE>
        size_type LinkBucket<Tp, MAX_BUCKET, MAX_NODE>::s_bucket_buffer[MAX_BUCKET];
        template <typename Tp, size_type MAX_BUCKET, size_type MAX_NODE>
        typename LinkBucket<Tp, MAX_BUCKET, MAX_NODE>::node LinkBucket<Tp, MAX_BUCKET, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, size_type MAX_BUCKET, size_type MAX_NODE>
        size_type LinkBucket<Tp, MAX_BUCKET, MAX_NODE>::s_bucket_use_count;
        template <typename Tp, size_type MAX_BUCKET, size_type MAX_NODE>
        size_type LinkBucket<Tp, MAX_BUCKET, MAX_NODE>::s_use_count;
    }
};

#endif