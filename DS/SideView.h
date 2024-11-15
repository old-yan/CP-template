/*
最后修改:
20241114
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SIDEVIEW__
#define __OY_SIDEVIEW__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace SV {
        using size_type = uint32_t;
        template <typename Tp, typename Compare, Tp Minimum>
        struct BasePoset {
            using value_type = Tp;
            using compare_type = Compare;
            static bool comp(const value_type &x, const value_type &y) { return Compare()(x, y); }
            static constexpr value_type min() { return Minimum; }
        };
        template <typename Poset>
        class Table {
            using poset = Poset;
            using value_type = typename poset::value_type;
            struct node {
                value_type m_mx;
                size_type m_val;
            };
            size_type m_cap, m_size;
            std::vector<node> m_data;
            size_type _get(const node *data, size_type i, const value_type &lmx) const {
                if (!poset::comp(lmx, data[i].m_mx)) return 0;
                size_type ans = 1;
                while (i < m_cap)
                    if (poset::comp(lmx, data[i * 2].m_mx))
                        ans += data[i].m_val - data[i * 2].m_val, i *= 2;
                    else
                        i = i * 2 + 1;
                return ans;
            }
        public:
            Table() = default;
            Table(size_type length) { resize(length); }
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size - 1));
                m_data.assign(m_cap * 2, node{poset::min(), 0});
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size - 1));
                m_data.resize(m_cap * 2);
                node *data = m_data.data();
                for (size_type i = 0; i != m_size; i++) data[m_cap + i] = node{value_type(mapping(i)), 1};
                for (size_type i = m_size; i != m_cap; i++) data[m_cap + i] = node{poset::min(), 0};
                for (size_type len = m_cap / 2; len; len >>= 1)
                    for (size_type i = len; i != len << 1; i++) {
                        data[i].m_mx = std::max(data[i * 2].m_mx, data[i * 2 + 1].m_mx, poset::comp);
                        data[i].m_val = data[i * 2].m_val + _get(data, i * 2 + 1, data[i * 2].m_mx);
                    }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, const value_type &val) {
                node *data = m_data.data();
                data[i += m_cap].m_mx = val;
                data[i].m_val = poset::comp(poset::min(), val);
                while (i >>= 1) {
                    data[i].m_mx = std::max(data[i * 2].m_mx, data[i * 2 + 1].m_mx, poset::comp);
                    data[i].m_val = data[i * 2].m_val + _get(data, i * 2 + 1, data[i * 2].m_mx);
                }
            }
            size_type presum(size_type i, value_type lmx = poset::min()) const {
                auto data = m_data.data();
                size_type res{}, left = 0, right = i + 1;
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    res += _get(data, (m_cap + left) >> (j - 1), lmx);
                    lmx = std::max(lmx, data[(m_cap + left) >> (j - 1)].m_mx, poset::comp);
                    left = left2;
                }
                return res;
            }
            size_type query(size_type left, size_type right, value_type lmx = poset::min()) const {
                auto data = m_data.data();
                size_type res{};
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        res += _get(data, (m_cap + left) >> j, lmx);
                        lmx = std::max(lmx, data[(m_cap + left) >> j].m_mx, poset::comp);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    res += _get(data, (m_cap + left) >> (j - 1), lmx);
                    lmx = std::max(lmx, data[(m_cap + left) >> (j - 1)].m_mx, poset::comp);
                    left = left2;
                }
                return res;
            }
            size_type query_all() const { return m_data[1].m_val; }
            value_type mx_presum(size_type i) const {
                auto data = m_data.data();
                size_type left = 0, right = i + 1;
                value_type lmx = poset::min();
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    lmx = std::max(lmx, data[(m_cap + left) >> (j - 1)].m_mx, poset::comp);
                    left = left2;
                }
                return lmx;
            }
            value_type mx_query(size_type left, size_type right) const {
                auto data = m_data.data();
                value_type lmx = poset::min();
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        lmx = std::max(lmx, data[(m_cap + left) >> j].m_mx, poset::comp);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    lmx = std::max(lmx, data[(m_cap + left) >> (j - 1)].m_mx, poset::comp);
                    left = left2;
                }
                return lmx;
            }
            value_type mx_query_all() const { return m_data[1].m_mx; }
        };
    }
    template <typename Tp, typename Compare = std::less<Tp>, Tp Minimum = std::numeric_limits<Tp>::min()>
    using AscendingSideView = SV::Table<SV::BasePoset<Tp, Compare, Minimum>>;
    template <typename Tp, typename Compare = std::greater<Tp>, Tp Maximum = std::numeric_limits<Tp>::max()>
    using DescendingSideView = SV::Table<SV::BasePoset<Tp, Compare, Maximum>>;
    template <typename Tp, typename Compare = std::greater_equal<Tp>, Tp Maximum = std::numeric_limits<Tp>::max()>
    using NonAscendingSideView = SV::Table<SV::BasePoset<Tp, Compare, Maximum>>;
    template <typename Tp, typename Compare = std::less_equal<Tp>, Tp Minimum = std::numeric_limits<Tp>::min()>
    using NonDescendingSideView = SV::Table<SV::BasePoset<Tp, Compare, Minimum>>;
}

#endif