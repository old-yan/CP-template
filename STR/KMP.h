/*
最后修改:
20231212
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_KMP__
#define __OY_KMP__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Sequence>
    struct KMP {
        using value_type = typename Sequence::value_type;
        using size_type = uint32_t;
        Sequence m_seq;
        std::vector<size_type> m_pi;
        void _init() { m_seq.push_back({}), m_pi.push_back(0); }
        KMP() { clear(); }
        template <typename InitMapping>
        KMP(size_type length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        KMP(Iterator first, Iterator last) { reset(first, last); }
        KMP(const Sequence &seq) : KMP(seq.begin(), seq.end()) {}
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
            m_seq.push_back(elem);
            if (size() > 1) {
                size_type pi = jump(m_pi.back(), elem);
                m_pi.push_back(pi + (m_seq[pi + 1] == elem));
            } else
                m_pi.push_back(0);
        }
        void pop_back() { m_seq.pop_back(), m_pi.pop_back(); }
        size_type size() const { return m_seq.size() - 1; }
        size_type jump(size_type last_pi, const value_type &elem) const {
            size_type len = last_pi;
            while (len && (len == size() || m_seq[len + 1] != elem)) len = m_pi[len];
            return len;
        }
        template <typename Iterator>
        size_type contained_by(Iterator first, Iterator last) const {
            if (!size()) return 0;
            size_type len = 0;
            for (auto it = first; it != last; ++it) {
                const value_type &elem = *it;
                while (len && m_seq[len + 1] != elem) len = m_pi[len];
                if (m_seq[len + 1] == elem) len++;
                if (len == size()) return (it - first) - len + 1;
            }
            return -1;
        }
        template <typename Callback>
        void do_for_each_border(size_type init_border, Callback &&call) {
            size_type pi = init_border;
            while (pi) call(pi), pi = query_Pi(pi - 1);
        }
        size_type query_Pi(size_type i) const { return m_pi[i + 1]; }
    };
    using KMP_string = KMP<std::string>;
    template <typename ValueType>
    using KMP_vector = KMP<std::vector<ValueType>>;
}

#endif