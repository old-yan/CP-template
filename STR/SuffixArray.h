/*
最后修改:
20231210
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
#include <vector>

namespace OY {
    namespace SA {
        using size_type = uint32_t;
        using char_type = signed char;
        template <typename Sequence, size_type MAX_LEN>
        struct SuffixArray {
            using value_type = typename Sequence::value_type;
            static size_type s_lms_buffer[MAX_LEN], s_cnt_buffer[MAX_LEN << 1];
            static char_type s_type_buffer[MAX_LEN << 1];
            size_type m_length;
            std::vector<size_type> m_sa, m_rank, m_height;
            Sequence m_text;
            template <typename Text>
            void _sais_core(size_type length, size_type alpha, Text &&text, char_type *type, size_type *lms, size_type *cnt) {
                size_type n1 = 0, ch = -1;
                type[length - 1] = 1;
                for (size_type i = length - 2; ~i; i--) type[i] = text[i] == text[i + 1] ? type[i + 1] : text[i] < text[i + 1];
                std::fill_n(cnt, alpha, 0);
                for (size_type i = 0; i != length; i++) cnt[text[i]]++;
                std::partial_sum(cnt, cnt + alpha, cnt);
                for (size_type i = 1; i != length; i++)
                    if (!type[i - 1] && type[i] == 1) type[i] = 2, lms[n1++] = i;
                auto induced_sort = [&](size_type *v) {
                    size_type *cur = cnt + alpha;
                    auto push_S = [&](size_type x) { m_sa[--cur[text[x]]] = x; };
                    auto push_L = [&](size_type x) { m_sa[cur[text[x]]++] = x; };
                    std::fill_n(m_sa.data(), length, 0);
                    std::copy_n(cnt, alpha, cur);
                    for (size_type i = n1 - 1; ~i; i--) push_S(v[i]);
                    std::copy_n(cnt, alpha - 1, cur + 1);
                    for (size_type i = 0; i != length; i++)
                        if (m_sa[i] > 0 && type[m_sa[i] - 1] == 0) push_L(m_sa[i] - 1);
                    std::copy_n(cnt, alpha, cur);
                    for (size_type i = length - 1; ~i; i--)
                        if (m_sa[i] > 0 && type[m_sa[i] - 1]) push_S(m_sa[i] - 1);
                };
                auto lms_equal = [&](size_type x, size_type y) {
                    if (text[x] == text[y])
                        while (text[++x] == text[++y] && type[x] == type[y])
                            if (type[x] == 2 || type[y] == 2) return true;
                    return false;
                };
                induced_sort(lms);
                auto s1 = std::remove_if(m_sa.data(), m_sa.data() + length, [&](size_type x) { return type[x] != 2; });
                for (size_type i = 0; i != n1; i++) s1[m_sa[i] >> 1] = (ch += (int(ch) <= 0 || !lms_equal(m_sa[i], m_sa[i - 1])));
                for (size_type i = 0; i != n1; i++) s1[i] = s1[lms[i] >> 1];
                if (ch + 1 < n1)
                    _sais_core(n1, ch + 1, s1, type + length, lms + n1, cnt + alpha);
                else
                    for (size_type i = 0; i != n1; i++) m_sa[s1[i]] = i;
                for (size_type i = 0; i != n1; i++) lms[n1 + i] = lms[m_sa[i]];
                induced_sort(lms + n1);
            }
            template <typename InitMapping>
            SuffixArray(size_type length, InitMapping &&mapping) { resize(length, mapping); }
            template <typename Iterator>
            SuffixArray(Iterator first, Iterator last, size_type alpha = -1) { reset(first, last, alpha); }
            SuffixArray(const std::string &seq) : SuffixArray(seq.begin(), seq.end()) {}
            template <typename InitMapping>
            void resize(size_type length, InitMapping &&mapping) {
                m_length = length;
                m_text.reserve(m_length + 1);
                size_type Mx = 0;
                for (size_type i = 0; i != m_length; i++) {
                    auto elem = mapping(i);
                    Mx = std::max(Mx, elem);
                    m_text.push_back(elem);
                }
                m_text.push_back(0);
                m_sa.resize(m_length + 1);
                _sais_core(m_length + 1, Mx + 1, m_text, s_type_buffer, s_lms_buffer, s_cnt_buffer);
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last, size_type alpha = -1) {
                m_length = last - first;
                m_text.reserve(m_length + 1);
                m_text.assign(first, last);
                m_text.push_back(0);
                m_sa.resize(m_length + 1);
                _sais_core(m_length + 1, ~alpha ? alpha : *std::max_element(m_text.begin(), m_text.end()) + 1, m_text, s_type_buffer, s_lms_buffer, s_cnt_buffer);
            }
            void get_rank() {
                m_rank.resize(m_length);
                for (size_type i = 0; i != m_length; i++) m_rank[m_sa[i + 1]] = i;
            }
            void get_height() {
                m_height.resize(m_length);
                for (size_type i = 0, h = 0; i != m_length; i++) {
                    if (h) h--;
                    if (m_rank[i])
                        while (m_sa[m_rank[i]] + h < m_length && m_text[i + h] == m_text[m_sa[m_rank[i]] + h]) h++;
                    m_height[m_rank[i]] = h;
                }
            }
            size_type query_sa(size_type rank) const { return m_sa[rank + 1]; }
            size_type query_rank(size_type pos) const { return m_rank[pos]; }
            size_type query_height(size_type rank) const { return m_height[rank]; }
        };
        template <typename Sequence, size_type MAX_LEN>
        size_type SuffixArray<Sequence, MAX_LEN>::s_lms_buffer[MAX_LEN];
        template <typename Sequence, size_type MAX_LEN>
        size_type SuffixArray<Sequence, MAX_LEN>::s_cnt_buffer[MAX_LEN << 1];
        template <typename Sequence, size_type MAX_LEN>
        char_type SuffixArray<Sequence, MAX_LEN>::s_type_buffer[MAX_LEN << 1];
    }
    template <SA::size_type MAX_LEN>
    using SA_string = SA::SuffixArray<std::string, MAX_LEN>;
}

#endif