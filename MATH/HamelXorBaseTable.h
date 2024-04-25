/*
最后修改:
20231203
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HAMELXORBASETABLE__
#define __OY_HAMELXORBASETABLE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "HamelXorBase.h"

namespace OY {
    namespace HAMELTABLE {
        using size_type = uint32_t;
        template <typename Tp, size_type MAX_WIDTH>
        struct HamelTableNodes {
            Tp m_val[MAX_WIDTH];
            size_type m_pos[MAX_WIDTH];
            size_type count(size_type left) const {
                return std::count_if(m_pos, m_pos + MAX_WIDTH, [&](size_type pos) { return pos > left; });
            }
        };
        template <typename Tp>
        struct HamelTableNodes<Tp, 0> {
            static size_type s_width;
            std::vector<Tp> m_val = std::vector<Tp>(s_width);
            std::vector<size_type> m_pos = std::vector<size_type>(s_width);
            size_type count(size_type left) const {
                return std::count_if(m_val.begin(), m_val.end(), [&](size_type pos) { return pos > left; });
            }
        };
        template <typename Tp>
        size_type HamelTableNodes<Tp, 0>::s_width = sizeof(Tp) << 3;
        template <typename Tp, size_type MAX_WIDTH>
        struct HamelXorBaseTable {
            using base_type = HAMEL::HamelXorBase<Tp, MAX_WIDTH>;
            std::vector<HamelTableNodes<Tp, MAX_WIDTH>> m_masks;
            static void set_width(size_type w) {
                static_assert(!MAX_WIDTH, "MAX_WIDTH Must Be 0");
                HamelTableNodes<Tp, MAX_WIDTH>::s_width = w;
                base_type::set_width(w);
            }
            static size_type width() {
                if constexpr (MAX_WIDTH)
                    return MAX_WIDTH;
                else
                    return HamelTableNodes<Tp, MAX_WIDTH>::s_width;
            }
            HamelXorBaseTable() { m_masks.push_back({}); }
            template <typename InitMapping>
            HamelXorBaseTable(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            HamelXorBaseTable(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_masks.resize(length + 1);
                for (size_type j = 0; j != width(); j++) m_masks[0].m_val[j] = m_masks[0].m_pos[j] = 0;
                for (size_type i = 0; i != length; i++) {
                    m_masks[i + 1] = m_masks[i];
                    Tp mask = mapping(i);
                    size_type pos = i + 1;
                    for (size_type j = width() - 1; ~j; j--)
                        if (mask >> j & 1) {
                            if (!m_masks[i + 1].m_pos[j]) {
                                m_masks[i + 1].m_val[j] = mask, m_masks[i + 1].m_pos[j] = pos;
                                break;
                            }
                            if (m_masks[i + 1].m_pos[j] <= pos) std::swap(mask, m_masks[i + 1].m_val[j]), std::swap(pos, m_masks[i + 1].m_pos[j]);
                            mask ^= m_masks[i + 1].m_val[j];
                        }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type length) { m_masks.clear(), m_masks.reserve(length + 1), m_masks.push_back({}); }
            void clear() { m_masks.clear(), m_masks.push_back({}); }
            void push_back(Tp mask) {
                size_type pos = m_masks.size(), i = pos - 1;
                m_masks.push_back(m_masks.back());
                for (size_type j = width() - 1; ~j; j--)
                    if (mask >> j & 1) {
                        if (!m_masks[i + 1].m_pos[j]) {
                            m_masks[i + 1].m_val[j] = mask, m_masks[i + 1].m_pos[j] = pos;
                            break;
                        }
                        if (m_masks[i + 1].m_pos[j] <= pos) std::swap(mask, m_masks[i + 1].m_val[j]), std::swap(pos, m_masks[i + 1].m_pos[j]);
                        mask ^= m_masks[i + 1].m_val[j];
                    }
            }
            void pop_back(Tp mask) { m_masks.pop_back(); }
            template <typename Callback>
            void enumerate_base(size_type left, size_type right, Callback &&call) const {
                auto &node = m_masks[right + 1];
                for (uint32_t i = width() - 1; ~i; i--)
                    if (node.m_pos[i] > left) call(node.m_val[i]);
            }
            base_type to_base_type(size_type left, size_type right) const {
                base_type res{};
                enumerate_base(left, right, [&](Tp mask) { res.insert(mask); });
                return res;
            }
            Tp query_max_bitxor(size_type left, size_type right, Tp base = 0) const {
                auto &node = m_masks[right + 1];
                Tp ans = base;
                enumerate_base(left, right, [&](Tp mask) { if ((ans ^ mask) > ans) ans ^= mask; });
                return ans;
            }
        };
    }
    template <HAMELTABLE::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBaseTable32 = HAMELTABLE::HamelXorBaseTable<uint32_t, MAX_WIDTH>;
    template <HAMELTABLE::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBaseTable64 = HAMELTABLE::HamelXorBaseTable<uint64_t, MAX_WIDTH>;
    using DynamicHamelXorBaseTable32 = HAMELTABLE::HamelXorBaseTable<uint32_t, 0>;
    using DynamicHamelXorBaseTable64 = HAMELTABLE::HamelXorBaseTable<uint64_t, 0>;
}

#endif