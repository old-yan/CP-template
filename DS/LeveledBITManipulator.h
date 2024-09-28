/*
最后修改:
20240926
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LEVELEDBITMANIPULATOR__
#define __OY_LEVELEDBITMANIPULATOR__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace LBM {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        inline size_type meet(size_type a, size_type b) { return ((a + 1) & -(size_type(1) << std::bit_width((a + 1) ^ (b + 1)))) - 1; }
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
                m_sub.clear(), m_sub.resize(m_cap);
            }
            template <typename Initer>
            void resize(size_type length, Initer init) {
                if (!(m_size = length)) return;
                m_cap = std::bit_ceil(m_size);
                m_sub.clear(), m_sub.resize(m_cap);
                auto sub = m_sub.data();
                for (size_type i = 0; i != m_cap; i++) init(sub[i], i - lowbit(i + 1) + 1, i);
            }
            template <typename Callback>
            void modify_in_tables(size_type i, Callback &&call) {
                auto sub = m_sub.data();
                for (; i < m_size; i += lowbit(i + 1)) call(sub[i]);
            }
            template <typename Callback>
            void presum_in_tables(size_type i, Callback &&call) const {
                auto sub = m_sub.data();
                for (; ~i; i -= lowbit(i + 1)) call(sub[i]);
            }
            template <typename PlusCallback, typename MinusCallback>
            void query_in_tables(size_type left, size_type right, PlusCallback &&plus_call, MinusCallback &&minus_call) const {
                auto sub = m_sub.data();
                auto mt = meet(left - 1, right);
                for (size_type j = right; j != mt; j -= lowbit(j + 1)) plus_call(sub[j]);
                for (size_type j = left - 1; j != mt; j -= lowbit(j + 1)) minus_call(sub[j]);
            }
            template <typename Judger>
            size_type max_right(Judger &&judge) const {
                auto sub = m_sub.data();
                size_type cur = 0, len = m_cap;
                for (; len; len >>= 1)
                    if (cur + len <= m_size && judge(sub[cur + len - 1])) cur += len;
                return cur - 1;
            }
        };
    }
}

#endif