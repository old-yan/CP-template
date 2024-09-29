/*
最后修改:
20240929
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LEVELEDSEGMANIPULATOR__
#define __OY_LEVELEDSEGMANIPULATOR__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace LSM {
        using size_type = uint32_t;
        template <typename SizeType = uint64_t>
        class Table {
            SizeType m_size, m_cap;
        public:
            Table() = default;
            Table(SizeType length) { resize(length); }
            void resize(SizeType length) {
                if (!(m_size = length)) return;
                m_cap = std::bit_ceil(m_size);
            }
            template <typename Callback>
            void modify_in_tables(SizeType i, Callback &&call) {
                for (SizeType len = 1; len <= m_cap; len <<= 1) call(i & ~(len - 1), i | (len - 1));
            }
            template <typename Callback>
            void query_in_tables(SizeType left, SizeType right, Callback &&call) const {
                if (left == right) return void(call(left, left));
                right++;
                if (left)
                    while (true) {
                        SizeType j = std::countr_zero(left), left2 = left + (SizeType(1) << j);
                        if (left2 > right) break;
                        call(left, left2 - 1), left = left2;
                    }
                while (left < right) {
                    SizeType j = std::bit_width(left ^ right), left2 = left + (SizeType(1) << (j - 1));
                    call(left, left2 - 1);
                    left = left2;
                }
            }
            template <typename Judger>
            SizeType max_right(SizeType left, Judger &&judge) const {
                SizeType cur = left;
                size_type j = -1;
                if (cur)
                    while (true) {
                        j = std::countr_zero(cur);
                        SizeType next = cur + (SizeType(1) << j);
                        if (next > m_size || !judge(cur, next - 1)) break;
                        cur = next;
                    }
                if (cur != m_size) {
                    j = std::min<size_type>(j, std::bit_width(m_size - cur));
                    while (~--j) {
                        SizeType next = cur + (SizeType(1) << j);
                        if (next > m_size || !judge(cur, next - 1)) continue;
                        cur = next;
                    }
                }
                return cur - 1;
            }
            template <typename Judger>
            SizeType min_left(SizeType right, Judger &&judge) const {
                SizeType cur = right;
                size_type j = -1;
                do {
                    j = std::countr_zero(~cur);
                    SizeType next = cur - (SizeType(1) << j);
                    if (!judge(next + 1, cur)) break;
                    cur = next;
                } while (~cur);
                if (~cur) {
                    j = std::min<size_type>(j, std::bit_width(cur + 1));
                    while (~--j) {
                        SizeType next = cur - (SizeType(1) << j);
                        if (!judge(next + 1, cur)) continue;
                        cur = next;
                    }
                }
                return cur + 1;
            }
        };
    }
    using LSM32 = LSM::Table<uint32_t>;
    using LSM64 = LSM::Table<uint64_t>;
}

#endif