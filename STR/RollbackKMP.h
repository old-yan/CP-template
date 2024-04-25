/*
最后修改:
20231212
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ROLLBACKKMP__
#define __OY_ROLLBACKKMP__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Sequence>
    struct RollbackKMP {
        using value_type = typename Sequence::value_type;
        using size_type = uint32_t;
        Sequence m_seq;
        std::vector<size_type> m_pi;
        void _init() { m_seq.push_back({}), m_pi.push_back(0); }
        RollbackKMP() { clear(); }
        template <typename InitMapping>
        RollbackKMP(size_type length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        RollbackKMP(Iterator first, Iterator last) { reset(first, last); }
        RollbackKMP(const Sequence &seq) : RollbackKMP(seq.begin(), seq.end()) {}
        template <typename InitMapping>
        void resize(size_type length, InitMapping mapping) {
            reserve(length);
            for (size_type i = 0; i != length; i++) push_back(mapping(i));
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](size_type i) { return *(first + i); });
        }
        void reserve(size_type length) { clear(), m_seq.reserve(length), m_pi.reserve(length); }
        void clear() { m_seq.clear(), m_pi.clear(), _init(); }
        void push_back(const value_type &elem) {
            size_type len = size(), pre_len = std::numeric_limits<size_type>::max() / 2;
            while (len && m_seq[m_pi[len] + 1] != elem)
                if (len * 3 > pre_len * 2) {
                    size_type d = pre_len - len, q = len / d, r = len - q * d;
                    pre_len = len, len = d + r;
                } else
                    pre_len = len, len = m_pi[len];
            m_seq.push_back(elem);
            len = m_pi[len] + 1;
            m_pi.push_back(size() == 1 || m_seq[len] != elem ? 0 : len);
        }
        void pop_back() { m_seq.pop_back(), m_pi.pop_back(); }
        size_type size() const { return m_seq.size() - 1; }
        template <typename Iterator>
        size_type contained_by(Iterator first, Iterator last) const {
            if (!size()) return 0;
            size_type len = 0;
            for (auto it = first; it != last; ++it) {
                const value_type &elem = *it;
                size_type pre_len = std::numeric_limits<size_type>::max() / 2;
                while (len && m_seq[len + 1] != elem)
                    if (len * 3 > pre_len * 2)
                        len = pre_len % (pre_len - len) + pre_len - len;
                    else
                        pre_len = len, len = m_pi[len];
                if (m_seq[len + 1] == elem) len++;
                if (len == size()) return (it - first) - len + 1;
            }
            return -1;
        }
        size_type jump(size_type last_pi, const value_type &elem) const {
            size_type len = last_pi;
            while (len && (len == size() || m_seq[len + 1] != elem)) len = m_pi[len];
            return len;
        }
        template <typename Callback>
        void do_for_each_distinct_border(size_type init_border, Callback &&call) {
            size_type pre_pi = std::numeric_limits<size_type>::max() / 2, pi = init_border;
            while (pi) {
                call(pi);
                if (pi * 3 > pre_pi * 2)
                    pi = (pre_pi - pi) + pre_pi % (pre_pi - pi);
                else
                    pre_pi = pi, pi = query_Pi(pi - 1);
            }
        }
        size_type query_Pi(size_type i) const { return m_pi[i + 1]; }
    };
    using RollbackKMP_string = RollbackKMP<std::string>;
    template <typename ValueType>
    using RollbackKMP_vector = RollbackKMP<std::vector<ValueType>>;
}

#endif