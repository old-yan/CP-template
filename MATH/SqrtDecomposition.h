/*
最后修改:
20240621
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SQRTDECOMPOSITION__
#define __OY_SQRTDECOMPOSITION__

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace OY {
    template <typename Tp, typename = typename std::enable_if<std::is_unsigned<Tp>::value>::type>
    struct SqrtDecomposition {
        struct iterator {
            struct _range {
                Tp m_quot, m_left, m_right;
                Tp quot() const { return m_quot; }
                Tp left() const { return m_left; }
                Tp right() const { return m_right; }
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const _range &x) { return out << '(' << x.m_quot << ": " << x.m_left << '~' << x.m_right << ')'; }
            } m_range;
            Tp m_val;
            iterator &operator--() {
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
            iterator &operator++() {
                m_range.m_right = m_range.m_left - 1;
                if (m_range.m_quot < m_range.m_right)
                    m_range.m_left = m_val / (++m_range.m_quot + 1) + 1;
                else if (m_range.m_right)
                    m_range.m_quot = m_val / --m_range.m_left;
                else
                    m_range.m_quot = -1, m_range.m_left = 0;
                return *this;
            }
            const _range &operator*() const { return m_range; }
            const _range *operator->() const { return &m_range; }
            bool operator!=(const iterator &rhs) const { return m_range.m_quot != rhs.m_range.m_quot; }
            bool operator==(const iterator &rhs) const { return m_range.m_quot == rhs.m_range.m_quot; }
            static iterator null(Tp val) { return {{Tp(-1)}, val}; }
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
            static reverse_iterator null(Tp val) {
                reverse_iterator res;
                res.m_range = {0, val + 1, Tp(-1)}, res.m_val = val;
                return res;
            }
        };
        Tp m_val;
        SqrtDecomposition(Tp val) : m_val(val) {}
        Tp size() const {
            Tp r;
            if constexpr (sizeof(Tp) == 8)
                r = std::sqrt((long double)m_val);
            else
                r = std::sqrt((double)m_val);
            return r * 2 - (m_val * 2 + 1 < (r + 1) * r * 2);
        }
        iterator lower_bound(Tp quot) const {
            if (!quot) return rend().base();
            if (quot > m_val) return end();
            Tp right = m_val / quot;
            if (quot < right) {
                Tp left = m_val / (quot + 1) + 1;
                return iterator{{quot, left, right}, m_val};
            } else
                return iterator{{m_val / right, right, right}, m_val};
        }
        iterator upper_bound(Tp quot) const { return lower_bound(quot + 1); }
        iterator find_by_divisor(Tp divisor) const {
            if (!divisor) return end();
            if (divisor > m_val) return rend().base();
            Tp quot = m_val / divisor;
            if (divisor <= quot) return iterator{{quot, divisor, divisor}, m_val};
            Tp right = m_val / quot;
            Tp left = m_val / (quot + 1) + 1;
            return iterator{{quot, left, right}, m_val};
        }
        iterator begin() const { return iterator{{1, m_val / 2 + 1, m_val}, m_val}; }
        iterator end() const { return iterator::null(m_val); }
        reverse_iterator rbegin() const {
            reverse_iterator res;
            res.m_range = {m_val, 1, 1}, res.m_val = m_val;
            return res;
        }
        reverse_iterator rend() const { return reverse_iterator::null(m_val); }
    };
    template <typename Ostream, typename Tp>
    Ostream &operator<<(Ostream &out, const SqrtDecomposition<Tp> &x) {
        out << '{';
        for (auto i : x) {
            if (i.m_quot != 1) out << ", ";
            out << i;
        }
        return out << '}';
    }
}

#endif