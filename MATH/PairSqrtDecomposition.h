/*
最后修改:
20240626
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PAIRSQRTDECOMPOSITION__
#define __OY_PAIRSQRTDECOMPOSITION__

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace OY {
    template <typename Tp, typename = typename std::enable_if<std::is_unsigned<Tp>::value>::type>
    struct PairSqrtDecomposition {
        struct iterator {
            struct base_iterator {
                struct _range {
                    Tp m_quot, m_left, m_right;
                    template <typename Ostream>
                    friend Ostream &operator<<(Ostream &out, const _range &x) { return out << '(' << x.m_quot << ": " << x.m_left << '~' << x.m_right << ')'; }
                } m_range;
                Tp m_val;
                base_iterator &operator--() {
                    if (m_range.m_quot == 1)
                        m_range = {0, m_val + 1, Tp(-1)};
                    else {
                        m_range.m_left = m_range.m_right + 1;
                        m_range.m_quot = m_val / m_range.m_left;
                        if (m_range.m_quot >= m_range.m_left)
                            m_range.m_right = m_range.m_left;
                        else
                            m_range.m_right = m_val / m_range.m_quot;
                    }
                    return *this;
                }
                base_iterator &operator++() {
                    m_range.m_right = m_range.m_left - 1;
                    if (m_range.m_quot < m_range.m_right)
                        m_range.m_left = m_val / (++m_range.m_quot + 1) + 1;
                    else if (m_range.m_right)
                        m_range.m_quot = m_val / --m_range.m_left;
                    else
                        m_range.m_quot = -1, m_range.m_left = 0;
                    return *this;
                }
                const _range *operator->() const { return &m_range; }
                bool operator!=(const base_iterator &rhs) const { return m_range.m_quot != rhs.m_range.m_quot; }
                bool operator==(const base_iterator &rhs) const { return m_range.m_quot == rhs.m_range.m_quot; }
                static base_iterator null(Tp val) { return {{Tp(-1)}, val}; }
            };
            base_iterator m_iter1, m_iter2;
            Tp left() const { return std::max(m_iter1->m_left, m_iter2->m_left); }
            Tp right() const { return std::min(m_iter1->m_right, m_iter2->m_right); }
            Tp quot1() const { return m_iter1->m_quot; }
            Tp quot2() const { return m_iter2->m_quot; }
            iterator &operator--() {
                if (m_iter1->m_right < m_iter2->m_right)
                    --m_iter1;
                else if (m_iter1->m_right > m_iter2->m_right)
                    --m_iter2;
                else
                    --m_iter1, --m_iter2;
                return *this;
            }
            iterator &operator++() {
                if (m_iter1->m_left > m_iter2->m_left)
                    ++m_iter1;
                else if (m_iter1->m_left < m_iter2->m_left)
                    ++m_iter2;
                else
                    ++m_iter1, ++m_iter2;
                return *this;
            }
            const iterator &operator*() const { return *this; }
            bool operator!=(const iterator &rhs) const { return m_iter1 != rhs.m_iter1 || m_iter2 != rhs.m_iter2; }
            bool operator==(const iterator &rhs) const { return m_iter1 == rhs.m_iter1 && m_iter2 == rhs.m_iter2; }
            static iterator null(Tp val1, Tp val2) { return {base_iterator::null(val1), base_iterator::null(val2)}; }
        };
        struct reverse_iterator : iterator {
            iterator base() const { return *this; }
            reverse_iterator &operator--() {
                iterator::operator++();
                return *this;
            }
            reverse_iterator &operator++() {
                iterator::operator--();
                return *this;
            }
            static reverse_iterator null(Tp val1, Tp val2) {
                reverse_iterator res;
                res.m_iter1 = {{0, val1 + 1, Tp(-1)}, val1}, res.m_iter2 = {{0, val2 + 1, Tp(-1)}, val2};
                return res;
            }
        };
        Tp m_val1, m_val2;
        PairSqrtDecomposition(Tp val1, Tp val2) : m_val1(val1), m_val2(val2) {}
        iterator find_by_divisor(Tp divisor) const {
            if (!divisor) return end();
            Tp q1 = m_val1 / divisor, q2 = m_val2 / divisor;
            if (!q1 && !q2) return rend().base();
            Tp l1 = q1 ? m_val1 / (q1 + 1) + 1 : 0, r1 = q1 ? m_val1 / q1 : Tp(-1);
            Tp l2 = q2 ? m_val2 / (q2 + 1) + 1 : 0, r2 = q2 ? m_val2 / q2 : Tp(-1);
            return iterator{{{q1, l1, r1}, m_val1}, {{q2, l2, r2}, m_val2}};
        }
        iterator begin() const {
            if (m_val1 > m_val2)
                return iterator{{{1, m_val1 / 2 + 1, m_val1}, m_val1}, {{0, m_val2 + 1, Tp(-1)}, m_val2}};
            else if (m_val1 < m_val2)
                return iterator{{{0, m_val1 + 1, Tp(-1)}, m_val1}, {{1, m_val2 / 2 + 1, m_val2}, m_val2}};
            else
                return iterator{{{1, m_val1 / 2 + 1, m_val1}, m_val1}, {{1, m_val2 / 2 + 1, m_val2}, m_val2}};
        }
        iterator end() const { return iterator::null(m_val1, m_val2); }
        reverse_iterator rbegin() const {
            reverse_iterator res;
            res.m_iter1.m_range = {m_val1, 1, 1}, res.m_iter1.m_val = m_val1;
            res.m_iter2.m_range = {m_val2, 1, 1}, res.m_iter2.m_val = m_val2;
            return res;
        }
        reverse_iterator rend() const { return reverse_iterator::null(m_val1, m_val2); }
    };
    template <typename Ostream, typename Tp>
    Ostream &operator<<(Ostream &out, const PairSqrtDecomposition<Tp> &x) {
        out << '{';
        for (auto i : x) {
            if (i.m_quot != 1) out << ", ";
            out << i;
        }
        return out << '}';
    }
}

#endif