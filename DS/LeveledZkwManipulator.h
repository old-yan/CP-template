/*
最后修改:
20240926
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LEVELEDZKWMANIPULATOR__
#define __OY_LEVELEDZKWMANIPULATOR__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace LZM {
        using size_type = uint32_t;
        template <typename InnerTable>
        class Table {
            size_type m_size, m_cap;
            std::vector<InnerTable> m_sub;
        public:
            Table() = default;
            Table(size_type length) { resize(length); }
            template <typename Initer>
            Table(size_type length, Initer init) { resize(length, init); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_cap = std::bit_ceil(m_size);
                m_sub.clear(), m_sub.resize(m_cap << 1);
            }
            template <typename Initer>
            void resize(size_type length, Initer init) {
                if (!(m_size = length)) return;
                m_cap = std::bit_ceil(m_size);
                m_sub.clear(), m_sub.resize(m_cap << 1);
                auto sub = m_sub.data();
                for (size_type i = 0; i != m_cap; i++) init(sub[m_cap + i], i, i);
                for (size_type len = m_cap / 2, w = 2; len; len >>= 1, w <<= 1)
                    for (size_type i = len, cur = 0; i != len << 1; i++, cur += w) init(sub[i], cur, cur + w - 1);
            }
            template <typename Callback>
            void modify_in_tables(size_type i, Callback &&call) {
                auto sub = m_sub.data();
                for (i += m_cap; i; i >>= 1) call(sub[i]);
            }
            template <typename Callback>
            void query_in_table(size_type i, Callback &&call) const { call(m_sub[m_cap + i]); }
            template <typename Callback>
            void query_in_tables(size_type left, size_type right, Callback &&call) const {
                auto sub = m_sub.data();
                if (left == right) return call(sub[m_cap + left]);
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        call(sub[(m_cap + left) >> j]);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    call(sub[(m_cap + left) >> (j - 1)]);
                    left = left2;
                }
            }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                auto sub = m_sub.data();
                size_type cur = left, j = -1;
                if (cur)
                    while (true) {
                        j = std::countr_zero(cur);
                        size_type next = cur + (size_type(1) << j);
                        if (next > m_size || !judge(sub[(m_cap + cur) >> j])) break;
                        cur = next;
                    }
                if (cur != m_size) {
                    j = std::min<size_type>(j, std::bit_width(m_size - cur));
                    while (~--j) {
                        size_type next = cur + (size_type(1) << j);
                        if (next > m_size || !judge(sub[(m_cap + cur) >> j])) continue;
                        cur = next;
                    }
                }
                return cur - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                auto sub = m_sub.data();
                size_type cur = right, j = -1;
                do {
                    j = std::countr_zero(~cur);
                    size_type next = cur - (1 << j);
                    if (!judge(sub[(m_cap + cur) >> j])) break;
                    cur = next;
                } while (~cur);
                if (~cur) {
                    j = std::min<size_type>(j, std::bit_width(cur + 1));
                    while (~--j) {
                        size_type next = cur - (1 << j);
                        if (!judge(sub[(m_cap + cur) >> j])) continue;
                        cur = next;
                    }
                }
                return cur + 1;
            }
        };
    }
}

#endif