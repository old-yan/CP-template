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
        template <typename Tp>
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
                LinkBucket *m_lbc;
                size_type m_buc_id;
                Bucket(LinkBucket *lbc, size_type buc_id) : m_lbc(lbc), m_buc_id(buc_id) {}
                bool empty() const { return !~m_lbc->m_bucket[m_buc_id]; }
                template <typename Modify = Tp>
                void push_back(Modify modify) {
                    if constexpr (Can_call<Modify, node *>::value)
                        modify(m_lbc->m_item.data() + m_lbc->m_cursor);
                    else
                        m_lbc->m_item[m_lbc->m_cursor].m_value = modify;
                    m_lbc->m_item[m_lbc->m_cursor].m_next = m_lbc->m_bucket[m_buc_id];
                    m_lbc->m_bucket[m_buc_id] = m_lbc->m_cursor++;
                }
                iterator begin() const { return iterator(m_lbc->m_item.data(), m_lbc->m_bucket[m_buc_id]); }
                iterator end() const { return iterator(m_lbc->m_item.data(), -1); }
            };
            std::vector<size_type> m_bucket;
            std::vector<node> m_item;
            size_type m_bucket_cnt, m_cursor;
            LinkBucket(size_type bucket_cnt = 0, size_type item_cnt = 0) { resize(bucket_cnt, item_cnt); }
            void resize(size_type bucket_cnt, size_type item_cnt) {
                if (!(m_bucket_cnt = bucket_cnt)) return;
                m_bucket.assign(m_bucket_cnt, -1), m_item.resize(item_cnt), m_cursor = 0;
            }
            Bucket operator[](size_type buc_id) { return Bucket(this, buc_id); }
            iterator bucket_begin(size_type buc_id) { return iterator(m_item, m_bucket[buc_id]); }
            iterator bucket_end(size_type = 0) { return iterator(m_item, -1); }
        };
    }
};

#endif