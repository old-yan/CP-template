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
    struct SqrtDecomposition {
        struct iterator {
            struct _range {
                uint64_t m_quot, m_left, m_right;
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const _range &x) { return out << '(' << x.m_quot << ": " << x.m_left << '~' << x.m_right << ')'; }
            } m_range;
            uint64_t m_val;
            iterator &operator--() {
                if (m_range.m_quot == 1)
                    --m_range.m_quot;
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
                if (m_range.m_quot + 1 < m_range.m_left) {
                    m_range.m_right = m_range.m_left - 1;
                    m_range.m_left = m_val / (m_range.m_quot + 2) + 1;
                    m_range.m_quot++;
                } else if (--m_range.m_left)
                    m_range.m_quot = m_val / (m_range.m_right = m_range.m_left);
                else
                    m_range.m_quot = -1;
                return *this;
            }
            const _range &operator*() const { return m_range; }
            const _range *operator->() const { return &m_range; }
            bool operator!=(const iterator &rhs) const { return m_range.m_quot != rhs.m_range.m_quot; }
            bool operator==(const iterator &rhs) const { return m_range.m_quot == rhs.m_range.m_quot; }
            static iterator null(uint64_t val) { return {{uint64_t(-1)}, val}; }
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
            static reverse_iterator null(uint64_t val) {
                reverse_iterator res;
                res.m_range = {0, val + 1, uint64_t(-1)}, res.m_val = val;
                return res;
            }
        };
        uint64_t m_val;
        SqrtDecomposition(uint64_t val) : m_val(val) {}
        uint64_t size() const {
            uint64_t r = std::sqrt((long double)m_val);
            return r * 2 - (m_val * 2 + 1 < (r + 1) * r * 2);
        }
        iterator lower_bound(uint64_t quot) const {
            if (!quot) return rend().base();
            if (quot > m_val) return end();
            uint64_t right = m_val / quot;
            if (quot < right) {
                uint64_t left = m_val / (quot + 1) + 1;
                return iterator{{quot, left, right}, m_val};
            } else
                return iterator{{m_val / right, right, right}, m_val};
        }
        iterator upper_bound(uint64_t quot) const { return lower_bound(quot + 1); }
        iterator find_by_divisor(uint64_t divisor) const {
            if (!divisor) return end();
            if (divisor > m_val) return rend().base();
            uint64_t quot = m_val / divisor;
            if (divisor <= quot) return iterator{{quot, divisor, divisor}, m_val};
            uint64_t right = m_val / quot;
            uint64_t left = m_val / (quot + 1) + 1;
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
    template <typename Ostream>
    Ostream &operator<<(Ostream &out, const SqrtDecomposition &x) {
        out << '{';
        for (auto i : x) {
            if (i.m_quot != 1) out << ", ";
            out << i;
        }
        return out << '}';
    }
}

#endif