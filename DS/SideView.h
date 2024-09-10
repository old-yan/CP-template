/*
最后修改:
20240831
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
        template <typename Tp, typename Compare = std::less<Tp>>
        class Table {
            struct node {
                Tp m_max;
                size_type m_val;
            };
            size_type m_cap, m_size;
            std::vector<node> m_data;
            size_type _get(node *data, size_type i, const Tp &lmx) const {
                if (!Compare()(lmx, data[i].m_max)) return 0;
                size_type ans = 1;
                while (data[i].m_val)
                    if (Compare()(lmx, data[i * 2].m_max))
                        ans += data[i].m_val - data[i * 2].m_val, i *= 2;
                    else
                        i = i * 2 + 1;
                return ans;
            }
        public:
            Table() = default;
            Table(size_type length, const Tp &min = std::numeric_limits<Tp>::min()) { resize(length, min); }
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping, const Tp &min) { resize(length, mapping, min); }
            template <typename Iterator>
            Table(Iterator first, Iterator last, const Tp &min = std::numeric_limits<Tp>::min()) { reset(first, last, min); }
            void resize(size_type length, const Tp &min = std::numeric_limits<Tp>::min()) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size));
                m_data.assign(m_cap * 2, node{min, 0});
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping, const Tp &min) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size));
                m_data.resize(m_cap * 2);
                node *data = m_data.data();
                data[m_cap] = node{min, 0};
                for (size_type i = 0; i != m_size; i++) data[m_cap + i + 1] = node{Tp(mapping(i)), 0};
                for (size_type i = m_size + 1; i != m_cap; i++) data[m_cap + i] = node{min, 0};
                for (size_type len = m_cap / 2, w = 2; len; len >>= 1, w <<= 1)
                    for (size_type i = len; i != len << 1; i++) {
                        data[i].m_max = std::max(data[i * 2].m_max, data[i * 2 + 1].m_max, Compare());
                        data[i].m_val = data[i * 2].m_val + _get(data, i * 2 + 1, data[i * 2].m_max);
                    }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last, const Tp &min = std::numeric_limits<Tp>::min()) {
                resize(last - first, [&](size_type i) { return *(first + i); }, min);
            }
            void modify(size_type i, const Tp &val) {
                node *data = m_data.data();
                data[i += m_cap + 1].m_max = val;
                while (i >>= 1) {
                    data[i].m_max = std::max(data[i * 2].m_max, data[i * 2 + 1].m_max, Compare());
                    data[i].m_val = data[i * 2].m_val + _get(data, i * 2 + 1, data[i * 2].m_max);
                }
            }
            size_type presum(size_type i) const {
                auto *data = (node *)m_data.data();
                size_type left = 0, right = i + 2, res{};
                Tp lmx = data[m_cap].m_max;
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    res += _get(data, (m_cap + left) >> (j - 1), lmx);
                    lmx = std::max(lmx, data[(m_cap + left) >> (j - 1)].m_max, Compare());
                    left = left2;
                }
                return res;
            }
            size_type query(size_type left, size_type right) const {
                auto *data = (node *)m_data.data();
                left = left ? left + 1 : left, right += 2;
                size_type res{};
                Tp lmx = data[m_cap].m_max;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        res += _get(data, (m_cap + left) >> j, lmx);
                        lmx = std::max(lmx, data[(m_cap + left) >> j].m_max, Compare());
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    res += _get(data, (m_cap + left) >> (j - 1), lmx);
                    lmx = std::max(lmx, data[(m_cap + left) >> (j - 1)].m_max, Compare());
                    left = left2;
                }
                return res;
            }
            size_type query_all() const { return m_data[1].m_val; }
        };
    }
}

#endif