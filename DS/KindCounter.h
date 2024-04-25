/*
最后修改:
20240426
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_KINDCOUNTER__
#define __OY_KINDCOUNTER__

#include "WaveLet.h"

#include <map>
#include <unordered_map>
#include <utility>

namespace OY {
    namespace KC {
        using size_type = uint32_t;
        struct MapTag {
            template <typename Tp>
            using type = std::map<Tp, size_type>;
        };
        struct UmapTag {
            template <typename Tp>
            using type = std::unordered_map<Tp, size_type>;
        };
        template <size_type MAX_VALUE>
        struct ArrayTag {
            template <typename Tp>
            using type = size_type[MAX_VALUE + 1];
        };
        template <typename Tag>
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
                typename Tag::type<Tp> mp{};
                m_table.resize(length, [&](size_type i) -> size_type {
                    size_type j = i + 1;
                    std::swap(mp[mapping(i)], j);
                    return j;
                });
                m_max = m_table.maximum(0, length - 1);
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type query(size_type left, size_type right) const { return m_max <= left ? right - left + 1 : m_table.rank(left, right, left + 1); }
        };
    }
    using MapKindCounter = KC::Table<KC::MapTag>;
    using UmapKindCounter = KC::Table<KC::UmapTag>;
    template <KC::size_type MAX_VALUE>
    using ArrayKindCounter = KC::Table<KC::ArrayTag<MAX_VALUE>>;
}

#endif