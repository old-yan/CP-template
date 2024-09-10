/*
最后修改:
20240529
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RANGEMODE__
#define __OY_RANGEMODE__

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    template <typename Tp, bool Minimize = false>
    class RangeMode {
        using size_type = uint32_t;
        template <typename Fp>
        struct Pair {
            Fp m_val;
            size_type m_cnt;
            bool operator<(const Pair<Fp> &rhs) const { return m_cnt < rhs.m_cnt; }
        };
        size_type m_size, m_block_size, m_block_cnt;
        std::vector<Tp> m_sorted;
        std::vector<size_type> m_arr, m_pos, m_start, m_pos2;
        std::vector<Pair<size_type>> m_ps;
    public:
        template <typename InitMapping>
        RangeMode(size_type length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        RangeMode(Iterator first, Iterator last) { reset(first, last); }
        template <typename InitMapping>
        void resize(size_type length, InitMapping mapping) {
            if (!(m_size = length)) return;
            std::vector<Pair<Tp>> ps(m_size);
            for (size_type i = 0; i != m_size; i++) ps[i] = {Tp(mapping(i)), i};
            std::sort(ps.begin(), ps.end(), [](const Pair<Tp> &x, const Pair<Tp> &y) { return x.m_val < y.m_val; });
            m_sorted.clear(), m_sorted.reserve(m_size);
            m_arr.resize(m_size);
            for (size_type i = 0; i != m_size; i++) {
                if (!i || ps[i].m_val != ps[i - 1].m_val) m_sorted.push_back(ps[i].m_val);
                m_arr[ps[i].m_cnt] = m_sorted.size() - 1;
            }
            m_pos.resize(m_size);
            std::vector<size_type> cnt(m_size + 1);
            for (size_type i = 0; i != m_size; i++) cnt[m_arr[i] + 1]++;
            for (size_type i = 0; i != m_sorted.size(); i++) cnt[i + 1] += cnt[i];
            m_start = cnt;
            m_pos2.resize(m_size);
            for (size_type i = 0; i != m_size; i++) {
                size_type pos = cnt[m_arr[i]]++;
                m_pos[pos] = i, m_pos2[i] = pos;
            }
            std::fill_n(cnt.data(), m_sorted.size(), 0);
            m_block_size = m_sorted.size() <= 64 ? std::bit_ceil<size_type>(std::sqrt(m_size)) : std::bit_floor<size_type>(std::sqrt(m_size)), m_block_cnt = (m_size + m_block_size - 1) / m_block_size;
            m_ps.resize(m_block_cnt * m_block_cnt);
            for (size_type l = 0; l != m_block_cnt; l++) {
                Pair<size_type> p{};
                std::fill_n(cnt.data(), m_sorted.size(), 0);
                for (size_type r = l; r != m_block_cnt; r++) {
                    size_type p1 = r * m_block_size, p2 = std::min((r + 1) * m_block_size, m_size);
                    for (size_type i = p1; i != p2; i++) {
                        Pair<size_type> q{m_arr[i], ++cnt[m_arr[i]]};
                        if constexpr (Minimize) {
                            if (q.m_cnt > p.m_cnt || (q.m_cnt == p.m_cnt && q.m_val < p.m_val)) p = q;
                        } else if (q.m_cnt > p.m_cnt)
                            p = q;
                    }
                    m_ps[l * m_block_cnt + r] = p;
                }
            }
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](size_type i) { return *(first + i); });
        }
        Pair<Tp> query(size_type left, size_type right) const {
            size_type l = (left + m_block_size - 1) / m_block_size, r = (right + 1) / m_block_size - 1, elem{}, cnt{};
            if (l < r + 1) {
                elem = m_ps[l * m_block_cnt + r].m_val, cnt = m_ps[l * m_block_cnt + r].m_cnt;
                size_type start = std::lower_bound(m_pos.begin() + m_start[elem], m_pos.begin() + m_start[elem + 1], left) - m_pos.begin(), end = start + cnt - 1;
                while (start > m_start[elem] && m_pos[start - 1] >= left) start--, cnt++;
                while (end + 1 < m_start[elem + 1] && m_pos[end + 1] <= right) end++, cnt++;
                if (cnt * 2 >= right - left + 1) return {m_sorted[elem], cnt};
            }
            for (size_type i = left, j = std::min(l * m_block_size, right + 1); i != j; i++) {
                size_type c = m_arr[i], pos = m_pos2[i];
                if (pos + cnt < m_start[c + 1] && m_pos[pos + cnt] <= right) {
                    do cnt++;
                    while (pos + cnt < m_start[c + 1] && m_pos[pos + cnt] <= right);
                    elem = c;
                    if (cnt * 2 >= right - left + 1) return {m_sorted[elem], cnt};
                }
            }
            if (right >= l * m_block_size)
                for (size_type i = right, j = std::max((r + 1) * m_block_size - 1, left - 1); i != j; i--) {
                    size_type c = m_arr[i], pos = m_pos2[i];
                    if (cnt <= pos - m_start[c] && m_pos[pos - cnt] >= left) {
                        do cnt++;
                        while (cnt <= pos - m_start[c] && m_pos[pos - cnt] >= left);
                        elem = c;
                        if (cnt * 2 >= right - left + 1) return {m_sorted[elem], cnt};
                    }
                }
            return {m_sorted[elem], cnt};
        }
        Pair<Tp> query_min(size_type left, size_type right) const {
            static_assert(Minimize, "Minimize Must Be True");
            size_type l = (left + m_block_size - 1) / m_block_size, r = (right + 1) / m_block_size - 1, elem{}, cnt{};
            if (l < r + 1) {
                elem = m_ps[l * m_block_cnt + r].m_val, cnt = m_ps[l * m_block_cnt + r].m_cnt;
                size_type start = std::lower_bound(m_pos.begin() + m_start[elem], m_pos.begin() + m_start[elem + 1], left) - m_pos.begin(), end = start + cnt - 1;
                while (start > m_start[elem] && m_pos[start - 1] >= left) start--, cnt++;
                while (end + 1 < m_start[elem + 1] && m_pos[end + 1] <= right) end++, cnt++;
                if (cnt * 2 > right - left + 1) return {m_sorted[elem], cnt};
            }
            for (size_type i = left, j = std::min(l * m_block_size, right + 1); i != j; i++) {
                size_type c = m_arr[i], pos = m_pos2[i];
                if (pos + cnt < m_start[c + 1] && m_pos[pos + cnt] <= right) {
                    do cnt++;
                    while (pos + cnt < m_start[c + 1] && m_pos[pos + cnt] <= right);
                    elem = c;
                    if (cnt * 2 > right - left + 1) return {m_sorted[elem], cnt};
                } else if (c < elem && pos + cnt <= m_start[c + 1] && m_pos[pos + cnt - 1] <= right)
                    elem = c;
            }
            if (right >= l * m_block_size)
                for (size_type i = right, j = std::max((r + 1) * m_block_size - 1, left - 1); i != j; i--) {
                    size_type c = m_arr[i], pos = m_pos2[i];
                    if (cnt <= pos - m_start[c] && m_pos[pos - cnt] >= left) {
                        do cnt++;
                        while (cnt <= pos - m_start[c] && m_pos[pos - cnt] >= left);
                        elem = c;
                        if (cnt * 2 > right - left + 1) return {m_sorted[elem], cnt};
                    } else if (c < elem && cnt <= pos - m_start[c] + 1 && m_pos[pos - cnt + 1] >= left)
                        elem = c;
                }
            return {m_sorted[elem], cnt};
        }
    };
}

#endif