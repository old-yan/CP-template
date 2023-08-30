/*
最后修改:
20230830
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LAZYBITSET__
#define __OY_LAZYBITSET__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace LazyBitset {
        using index_type = uint32_t;
        template <typename SizeType = uint64_t, index_type MAX_NODE = 1 << 20>
        struct Tree {
            struct node {
                SizeType m_sum;
                bool m_flipped;
                index_type m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
                void flip(SizeType size) { m_sum = size - m_sum, m_flipped = !m_flipped; }
            };
            static node s_buffer[MAX_NODE];
            static index_type s_use_count;
            node *m_root;
            SizeType m_size;
            static void _pushdown_one(node *cur, SizeType floor, SizeType ceil) {
                SizeType len = ceil - floor + 1;
                if (!cur->m_lchild) cur->m_lchild = s_use_count++;
                cur->lchild()->m_sum = (len + 1) >> 1;
                if (!cur->m_rchild) cur->m_rchild = s_use_count++;
                cur->rchild()->m_sum = len >> 1;
            }
            static void _pushdown_zero(node *cur, SizeType floor, SizeType ceil) { cur->lchild()->m_sum = cur->rchild()->m_sum = 0; }
            static void _pushdown_flip(node *cur, SizeType floor, SizeType ceil) {
                cur->m_flipped = false;
                SizeType len = ceil - floor + 1;
                if (!cur->m_lchild) cur->m_lchild = s_use_count++;
                cur->lchild()->flip((len + 1) >> 1);
                if (!cur->m_rchild) cur->m_rchild = s_use_count++;
                cur->rchild()->flip(len >> 1);
            }
            static void _pushdown(node *cur, SizeType floor, SizeType ceil) {
                if (cur->m_sum == ceil - floor + 1)
                    _pushdown_one(cur, floor, ceil);
                else if (!cur->m_sum)
                    _pushdown_zero(cur, floor, ceil);
                else if (cur->m_flipped)
                    _pushdown_flip(cur, floor, ceil);
            }
            static node *_lchild(node *cur) {
                if (!cur->m_lchild) cur->m_lchild = s_use_count++;
                return cur->lchild();
            }
            static node *_rchild(node *cur) {
                if (!cur->m_rchild) cur->m_rchild = s_use_count++;
                return cur->rchild();
            }
            static void _pushup(node *cur) { cur->m_sum = cur->lchild()->m_sum + cur->rchild()->m_sum, cur->m_flipped = false; }
            static void _set(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (cur->m_sum == ceil - floor + 1) return;
                if (left <= floor && right >= ceil)
                    cur->m_sum = ceil - floor + 1;
                else {
                    if (!cur->m_sum)
                        _pushdown_zero(cur, floor, ceil);
                    else if (cur->m_flipped)
                        _pushdown_flip(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _set(_lchild(cur), floor, mid, left, right);
                    if (right > mid) _set(_rchild(cur), mid + 1, ceil, left, right);
                    _pushup(cur);
                }
            }
            static void _set(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (cur->m_sum == ceil - floor + 1) return;
                if (floor == ceil)
                    cur->m_sum = 1;
                else {
                    if (!cur->m_sum)
                        _pushdown_zero(cur, floor, ceil);
                    else if (cur->m_flipped)
                        _pushdown_flip(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _set(_lchild(cur), floor, mid, i);
                    else
                        _set(_rchild(cur), mid + 1, ceil, i);
                    _pushup(cur);
                }
            }
            static void _reset(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (!cur->m_sum) return;
                if (left <= floor && right >= ceil)
                    cur->m_sum = 0;
                else {
                    if (cur->m_sum == ceil - floor + 1)
                        _pushdown_one(cur, floor, ceil);
                    else if (cur->m_flipped)
                        _pushdown_flip(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _reset(cur->lchild(), floor, mid, left, right);
                    if (right > mid) _reset(cur->rchild(), mid + 1, ceil, left, right);
                    _pushup(cur);
                }
            }
            static void _reset(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (!cur->m_sum) return;
                if (floor == ceil)
                    cur->m_sum = 0;
                else {
                    if (cur->m_sum == ceil - floor + 1)
                        _pushdown_one(cur, floor, ceil);
                    else if (cur->m_flipped)
                        _pushdown_flip(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _reset(cur->lchild(), floor, mid, i);
                    else
                        _reset(cur->rchild(), mid + 1, ceil, i);
                    _pushup(cur);
                }
            }
            static void _flip(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (left <= floor && right >= ceil)
                    cur->flip(ceil - floor + 1);
                else {
                    _pushdown(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _flip(_lchild(cur), floor, mid, left, right);
                    if (right > mid) _flip(_rchild(cur), mid + 1, ceil, left, right);
                    _pushup(cur);
                }
            }
            static void _flip(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (floor == ceil)
                    cur->flip(1);
                else {
                    _pushdown(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _flip(_lchild(cur), floor, mid, i);
                    else
                        _flip(_rchild(cur), mid + 1, ceil, i);
                    _pushup(cur);
                }
            }
            static SizeType _count(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (!cur->m_sum) return 0;
                if (cur->m_sum == ceil - floor + 1) return std::min(ceil, right) - std::max(floor, left) + 1;
                if (left <= floor && right >= ceil) return cur->m_sum;
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1, res = 0;
                if (left <= mid) res += _count(cur->lchild(), floor, mid, left, right);
                if (right > mid) res += _count(cur->rchild(), mid + 1, ceil, left, right);
                return res;
            }
            static bool _any(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (!cur->m_sum) return false;
                if (cur->m_sum == ceil - floor + 1) return true;
                if (left <= floor && right >= ceil) return true;
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (left <= mid && _any(cur->lchild(), floor, mid, left, right)) return true;
                return right > mid && _any(cur->rchild(), mid + 1, ceil, left, right);
            }
            static bool _all(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (!cur->m_sum) return 0;
                if (cur->m_sum == ceil - floor + 1) return true;
                if (left <= floor && right >= ceil) return false;
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1, res = 0;
                if (left <= mid && !_all(cur->lchild(), floor, mid, left, right)) return false;
                return right <= mid || _all(cur->rchild(), mid + 1, ceil, left, right);
            }
            static SizeType _first_one(node *cur, SizeType floor, SizeType ceil) {
                if (cur->m_sum == ceil - floor + 1) return floor;
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (cur->lchild()->m_sum) return _first_one(cur->lchild(), floor, mid);
                return _first_one(cur->rchild(), mid + 1, ceil);
            }
            static SizeType _last_one(node *cur, SizeType floor, SizeType ceil) {
                if (cur->m_sum == ceil - floor + 1) return ceil;
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (cur->rchild()->m_sum) return _last_one(cur->rchild(), mid + 1, ceil);
                return _last_one(cur->lchild(), floor, mid);
            }
            static SizeType _first_zero(node *cur, SizeType floor, SizeType ceil) {
                if (!cur->m_sum) return floor;
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (cur->lchild()->m_sum != mid - floor + 1) return _first_zero(cur->lchild(), floor, mid);
                return _first_zero(cur->rchild(), mid + 1, ceil);
            }
            static SizeType _last_zero(node *cur, SizeType floor, SizeType ceil) {
                if (!cur->m_sum) return ceil;
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (cur->rchild()->m_sum != ceil - mid) return _last_zero(cur->rchild(), mid + 1, ceil);
                return _last_zero(cur->lchild(), floor, mid);
            }
            static SizeType _prev_one(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (!cur->m_sum) return -1;
                if (cur->m_sum == ceil - floor + 1) return std::min(i - 1, ceil);
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (i > mid + 1) {
                    SizeType res = _prev_one(cur->rchild(), mid + 1, ceil, i);
                    if (~res) return res;
                }
                return _prev_one(cur->lchild(), floor, mid, i);
            }
            static SizeType _next_one(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (!cur->m_sum) return -1;
                if (cur->m_sum == ceil - floor + 1) return std::max(i + 1, floor);
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (i + 1 <= mid) {
                    SizeType res = _next_one(cur->lchild(), floor, mid, i);
                    if (~res) return res;
                }
                return _next_one(cur->rchild(), mid + 1, ceil, i);
            }
            static SizeType _prev_zero(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (cur->m_sum == ceil - floor + 1) return -1;
                if (!cur->m_sum) return std::min(i - 1, ceil);
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (i > mid + 1) {
                    SizeType res = _prev_zero(cur->rchild(), mid + 1, ceil, i);
                    if (~res) return res;
                }
                return _prev_zero(cur->lchild(), floor, mid, i);
            }
            static SizeType _next_zero(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (cur->m_sum == ceil - floor + 1) return -1;
                if (!cur->m_sum) return std::max(i + 1, floor);
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (i + 1 <= mid) {
                    SizeType res = _next_zero(cur->lchild(), floor, mid, i);
                    if (~res) return res;
                }
                return _next_zero(cur->rchild(), mid + 1, ceil, i);
            }
            static bool _at(node *cur, SizeType floor, SizeType ceil, SizeType i) {
                if (!cur->m_sum) return false;
                if (cur->m_sum == ceil - floor + 1) return true;
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (i <= mid)
                    return _at(cur->lchild(), floor, mid, i);
                else
                    return _at(cur->rchild(), mid + 1, ceil, i);
            }
            static SizeType _kth(node *cur, SizeType floor, SizeType ceil, SizeType k) {
                if (cur->m_sum == ceil - floor + 1) return floor + k;
                if (cur->m_flipped) _pushdown_flip(cur, floor, ceil);
                SizeType mid = (floor + ceil) >> 1;
                if (k < cur->lchild()->m_sum)
                    return _kth(cur->lchild(), floor, mid, k);
                else
                    return _kth(cur->rchild(), mid + 1, ceil, k - cur->lchild()->m_sum);
            }
            Tree(SizeType length = 0) { resize(length); }
            void resize(SizeType length) {
                if (m_size = length) m_root = s_buffer + s_use_count++;
            }
            void set() { m_root->m_sum = m_size; }
            void set(SizeType i) { _set(m_root, 0, m_size - 1, i); }
            void set(SizeType left, SizeType right) { _set(m_root, 0, m_size - 1, left, right); }
            void reset() { m_root->m_sum = 0; }
            void reset(SizeType i) { _reset(m_root, 0, m_size - 1, i); }
            void reset(SizeType left, SizeType right) { _reset(m_root, 0, m_size - 1, left, right); }
            void flip() { m_root->flip(m_size); }
            void flip(SizeType i) { _flip(m_root, 0, m_size - 1, i); }
            void flip(SizeType left, SizeType right) { _flip(m_root, 0, m_size - 1, left, right); }
            SizeType count() const { return m_root->m_sum; }
            SizeType count(SizeType left, SizeType right) const { return _count(m_root, 0, m_size - 1, left, right); }
            bool any() const { return m_root->m_sum; }
            bool any(SizeType left, SizeType right) const { return _any(m_root, 0, m_size - 1, left, right); }
            bool all() const { return m_root->m_sum == m_size; }
            bool all(SizeType left, SizeType right) const { return _all(m_root, 0, m_size - 1, left, right); }
            SizeType first_one() const { return m_root->m_sum ? _first_one(m_root, 0, m_size - 1) : -1; }
            SizeType last_one() const { return m_root->m_sum ? _last_one(m_root, 0, m_size - 1) : -1; }
            SizeType first_zero() const { return m_root->m_sum < m_size ? _first_zero(m_root, 0, m_size - 1) : -1; }
            SizeType last_zero() const { return m_root->m_sum < m_size ? _last_zero(m_root, 0, m_size - 1) : -1; }
            SizeType prev_one(SizeType i) const { return i ? _prev_one(m_root, 0, m_size - 1, i) : -1; }
            SizeType next_one(SizeType i) const { return i + 1 < m_size ? _next_one(m_root, 0, m_size - 1, i) : -1; }
            SizeType prev_zero(SizeType i) const { return i ? _prev_zero(m_root, 0, m_size - 1, i) : -1; }
            SizeType next_zero(SizeType i) const { return i + 1 < m_size ? _next_zero(m_root, 0, m_size - 1, i) : -1; }
            bool at(SizeType i) const { return _at(m_root, 0, m_size - 1, i); }
            bool operator[](SizeType i) const { return at(i); }
            SizeType kth(SizeType k) const { return _kth(m_root, 0, m_size - 1, k); }
        };
        template <typename Ostream, typename SizeType, index_type MAX_NODE>
        Ostream &operator<<(Ostream &out, Tree<SizeType, MAX_NODE> &x) {
            out << "[";
            for (SizeType i = 0; i < x.count(); i++) {
                if (i) out << ", ";
                out << x.kth(i);
            }
            return out << "]";
        }
        template <typename SizeType, index_type MAX_NODE>
        typename Tree<SizeType, MAX_NODE>::node Tree<SizeType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename SizeType, index_type MAX_NODE>
        index_type Tree<SizeType, MAX_NODE>::s_use_count = 1;
    }
}

#endif