/*
最后修改:
20231218
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SUFFIXARRAY__
#define __OY_SUFFIXARRAY__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

namespace OY {
    namespace SA {
        using size_type = uint32_t;
        template <bool Rank, bool Height, size_type MAX_LEN>
        struct SuffixArray {
            size_type m_length;
            std::vector<size_type> m_sa, m_rank, m_height;
            static bool s_bool_buffer[MAX_LEN << 1];
            static size_type s_lms_map_buffer[MAX_LEN + 1], s_buffer[MAX_LEN * 6];
            template <typename Sequence>
            void _sa_is(const Sequence &seq, size_type length, size_type alpha, bool *ls, size_type *buffer) {
                if (length == 1) {
                    m_sa[0] = 0;
                    return;
                }
                if (length == 2) {
                    if (seq[0] < seq[1])
                        m_sa[0] = 0, m_sa[1] = 1;
                    else
                        m_sa[0] = 1, m_sa[1] = 0;
                    return;
                }
                ls[length - 1] = false;
                for (size_type i = length - 2; ~i; i--) ls[i] = (seq[i] == seq[i + 1]) ? ls[i + 1] : (seq[i] < seq[i + 1]);
                size_type *buf = buffer, *sum_l = buffer + alpha, *sum_s = buffer + alpha * 2;
                std::fill_n(sum_l, alpha, 0);
                std::fill_n(sum_s, alpha, 0);
                for (size_type i = 0; i != length; i++)
                    if (!ls[i])
                        sum_s[seq[i]]++;
                    else
                        sum_l[seq[i] + 1]++;
                for (size_type i = 0; i != alpha; i++) {
                    sum_s[i] += sum_l[i];
                    if (i + 1 != alpha) sum_l[i + 1] += sum_s[i];
                }
                auto induce = [&](size_type *lms, size_type *lms_end) {
                    std::fill_n(m_sa.begin(), length, -1);
                    std::copy_n(sum_s, alpha, buf);
                    for (auto it = lms; it != lms_end; ++it) {
                        auto d = *it;
                        if (d == length) continue;
                        m_sa[buf[seq[d]]++] = d;
                    }
                    std::copy_n(sum_l, alpha, buf);
                    m_sa[buf[seq[length - 1]]++] = length - 1;
                    for (size_type i = 0; i != length; i++) {
                        typename std::make_signed<size_type>::type v = m_sa[i];
                        if (v >= 1 && !ls[v - 1]) m_sa[buf[seq[v - 1]]++] = v - 1;
                    }
                    std::copy_n(sum_l, alpha, buf);
                    for (size_type i = length - 1; ~i; i--) {
                        typename std::make_signed<size_type>::type v = m_sa[i];
                        if (v >= 1 && ls[v - 1]) m_sa[--buf[seq[v - 1] + 1]] = v - 1;
                    }
                };
                size_type *lms = buffer + alpha * 3, *lms_end = lms;
                s_lms_map_buffer[0] = s_lms_map_buffer[length] = -1;
                size_type m = 0;
                for (size_type i = 1; i != length; i++)
                    if (!ls[i - 1] && ls[i])
                        s_lms_map_buffer[i] = m++, *lms_end++ = i;
                    else
                        s_lms_map_buffer[i] = -1;
                induce(lms, lms_end);
                if (m) {
                    size_type *sorted_lms = lms_end, *sorted_lms_end = sorted_lms;
                    for (size_type i = 0; i != length; i++) {
                        size_type v = m_sa[i];
                        if (~s_lms_map_buffer[v]) *sorted_lms_end++ = v;
                    }
                    size_type *rec_s = sorted_lms_end, rec_alpha = 1;
                    rec_s[s_lms_map_buffer[sorted_lms[0]]] = 0;
                    for (size_type i = 1; i != m; i++) {
                        size_type l = sorted_lms[i - 1], r = sorted_lms[i], end_l = (s_lms_map_buffer[l] + 1 < m) ? lms[s_lms_map_buffer[l] + 1] : length, end_r = (s_lms_map_buffer[r] + 1 < m) ? lms[s_lms_map_buffer[r] + 1] : length;
                        if (end_l - l != end_r - r)
                            rec_alpha++;
                        else {
                            while (l < end_l && seq[l] == seq[r]) l++, r++;
                            if (l == length || seq[l] != seq[r]) rec_alpha++;
                        }
                        rec_s[s_lms_map_buffer[sorted_lms[i]]] = rec_alpha - 1;
                    }
                    _sa_is(rec_s, m, rec_alpha, ls + length, rec_s + m);
                    for (size_type i = 0; i != m; i++) sorted_lms[i] = lms[m_sa[i]];
                    induce(lms_end, sorted_lms_end);
                }
            }
            void _get_rank() {
                m_rank.resize(m_length);
                for (size_type i = 0; i != m_length; i++) m_rank[m_sa[i]] = i;
            }
            template <typename Sequence>
            void _get_height(const Sequence &seq) {
                m_height.resize(m_length);
                for (size_type i = 0, h = 0; i != m_length; i++) {
                    if (h) h--;
                    if (m_rank[i])
                        while (m_sa[m_rank[i] - 1] + h < m_length && seq[i + h] == seq[m_sa[m_rank[i] - 1] + h]) h++;
                    m_height[m_rank[i]] = h;
                }
            }
            SuffixArray() = default;
            template <typename InitMapping>
            SuffixArray(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            SuffixArray(Iterator first, Iterator last, size_type alpha = 0) { reset(first, last, alpha); }
            SuffixArray(const std::string &seq) : SuffixArray(seq.begin(), seq.end()) {}
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                using value_type = decltype(mapping(0));
                m_length = length;
                std::vector<value_type> text;
                text.reserve(m_length);
                size_type Mx = 0;
                bool less_than_zero = false;
                for (size_type i = 0; i != m_length; i++) {
                    auto elem = mapping(i);
                    Mx = std::max<size_type>(Mx, elem);
                    less_than_zero |= elem < 0;
                    text.push_back(elem);
                }
                m_sa.resize(m_length);
                if (Mx < MAX_LEN && !less_than_zero)
                    _sa_is(text, m_length, Mx + 1, s_bool_buffer, s_buffer);
                else {
                    std::vector<value_type> items(text);
                    std::sort(items.begin(), items.end());
                    items.erase(std::unique(items.begin(), items.end()), items.end());
                    std::vector<size_type> ord(m_length);
                    for (size_type i = 0; i != m_length; i++) ord[i] = std::lower_bound(items.begin(), items.end(), text[i]) - items.begin();
                    _sa_is(ord, m_length, items.size(), s_bool_buffer, s_buffer);
                }
                if constexpr (Rank) {
                    _get_rank();
                    if constexpr (Height) _get_height(text);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last, uint32_t alpha = 0) {
                using value_type = typename std::decay<decltype(*first)>::type;
                m_length = last - first;
                m_sa.resize(m_length);
                if (alpha)
                    _sa_is(first, m_length, alpha, s_bool_buffer, s_buffer);
                else {
                    size_type Mx = 0;
                    bool less_than_zero = false;
                    for (auto it = first; it != last; ++it) {
                        auto &&elem = *it;
                        Mx = std::max<size_type>(Mx, elem);
                        less_than_zero |= elem < 0;
                    }
                    if (Mx < MAX_LEN && !less_than_zero)
                        _sa_is(first, m_length, Mx + 1, s_bool_buffer, s_buffer);
                    else {
                        std::vector<value_type> items(first, last);
                        std::sort(items.begin(), items.end());
                        items.erase(std::unique(items.begin(), items.end()), items.end());
                        std::vector<size_type> ord(m_length);
                        for (size_type i = 0; i != m_length; i++) ord[i] = std::lower_bound(items.begin(), items.end(), *(first + i)) - items.begin();
                        _sa_is(ord, m_length, items.size(), s_bool_buffer, s_buffer);
                    }
                }
                if constexpr (Rank) {
                    _get_rank();
                    if constexpr (Height) _get_height(first);
                }
            }
            size_type query_sa(size_type rank) const { return m_sa[rank]; }
            size_type query_rank(size_type pos) const {
                static_assert(Rank, "Rank Must Be True");
                return m_rank[pos];
            }
            size_type query_height(size_type rank) const {
                static_assert(Height, "Height Must Be True");
                return m_height[rank];
            }
        };
        template <bool Rank, bool Height, size_type MAX_LEN>
        bool SuffixArray<Rank, Height, MAX_LEN>::s_bool_buffer[MAX_LEN << 1];
        template <bool Rank, bool Height, size_type MAX_LEN>
        size_type SuffixArray<Rank, Height, MAX_LEN>::s_lms_map_buffer[MAX_LEN + 1];
        template <bool Rank, bool Height, size_type MAX_LEN>
        size_type SuffixArray<Rank, Height, MAX_LEN>::s_buffer[MAX_LEN * 6];
    }
}

#endif