/*
最后修改:
20240504
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_KINDCOUNTER__
#define __OY_KINDCOUNTER__

#include <utility>

#include "GlobalHashMap.h"
#include "WaveLet.h"

namespace OY {
    namespace KC {
        using size_type = uint32_t;
        template <size_type MAX_VALUE>
        struct ArrayTag {
            template <typename Tp>
            using type = size_type[MAX_VALUE + 1];
        };
        template <bool MakeRecord, size_type BUFFER>
        struct HashmapTag {
            template <typename Tp>
            struct type {
                using hash_table = GHASH::UnorderedMap<Tp, size_type, MakeRecord, BUFFER>;
                using node = typename hash_table::node;
                static hash_table s_hashmap;
                ~type() {
                    if constexpr (MakeRecord) s_hashmap.do_for_each([](node *p) { p->m_mapped = 0; }), s_hashmap.clear();
                }
                size_type &operator[](const Tp &key) { return s_hashmap.insert(key).m_ptr->m_mapped; }
            };
        };
        template <bool MakeRecord, size_type BUFFER>
        template <typename Tp>
        GHASH::UnorderedMap<Tp, size_type, MakeRecord, BUFFER> HashmapTag<MakeRecord, BUFFER>::type<Tp>::s_hashmap;
        template <typename Tag = void>
        struct Table {
            using inner_table = WaveLet::Table<size_type>;
            inner_table m_table;
            size_type m_max;
            Table() = default;
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                using Tp = typename std::decay<decltype(mapping(0))>::type;
                if constexpr (std::is_void<Tag>::value) {
                    struct pair {
                        Tp m_val;
                        size_type m_index;
                        bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                    };
                    std::vector<pair> ps(length);
                    for (size_type i = 0; i != length; i++) ps[i] = {Tp(mapping(i)), i};
                    std::sort(ps.begin(), ps.end());
                    std::vector<size_type> items(length);
                    size_type id = 0;
                    for (size_type i = 0; i != length; i++) {
                        if (i && ps[i - 1].m_val < ps[i].m_val) id++;
                        items[ps[i].m_index] = id;
                    }
                    std::vector<size_type> mp(id + 1);
                    m_table.resize(length, [&](size_type i) -> size_type {
                        size_type j = i + 1;
                        std::swap(mp[items[i]], j);
                        return j;
                    });
                } else {
                    typename Tag::template type<Tp> mp{};
                    m_table.resize(length, [&](size_type i) -> size_type {
                        size_type j = i + 1;
                        std::swap(mp[mapping(i)], j);
                        return j;
                    });
                }
                m_max = m_table.maximum(0, length - 1);
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type query(size_type left, size_type right) const { return m_max <= left ? right - left + 1 : m_table.rank(left, right, left + 1); }
        };
    }
    template <KC::size_type MAX_VALUE>
    using ArrayKindCounter = KC::Table<KC::ArrayTag<MAX_VALUE>>;
    template <bool MakeRecord, KC::size_type BUFFER>
    using HashmapKindCounter = KC::Table<KC::HashmapTag<MakeRecord, BUFFER>>;
}

#endif