/*
最后修改:
20240613
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
#include <vector>

namespace OY {
    namespace LazyBitset {
        using index_type = uint32_t;
        template <typename SizeType, bool MaintainLongest>
        struct NodeWrap {
            template <typename Node>
            struct type {
                using info = void;
            };
        };
        template <typename SizeType>
        struct NodeWrap<SizeType, true> {
            template <typename Node>
            struct type {
                struct info {
                    SizeType m_l0, m_l1, m_r0, m_r1, m_max0, m_max1;
                    static info zero(SizeType len) {
                        info x;
                        x.set_zero(len);
                        return x;
                    }
                    static info one(SizeType len) {
                        info x;
                        x.set_one(len);
                        return x;
                    }
                    void set_zero(SizeType len) {
                        m_l0 = m_r0 = m_max0 = len;
                        m_l1 = m_r1 = m_max1 = 0;
                    }
                    void set_one(SizeType len) {
                        m_l1 = m_r1 = m_max1 = len;
                        m_l0 = m_r0 = m_max0 = 0;
                    }
                    void flip() {
                        std::swap(m_l0, m_l1);
                        std::swap(m_r0, m_r1);
                        std::swap(m_max0, m_max1);
                    }
                    info operator+(const info &rhs) const {
                        info res;
                        res.m_l0 = m_max1 ? m_l0 : m_l0 + rhs.m_l0;
                        res.m_l1 = m_max0 ? m_l1 : m_l1 + rhs.m_l1;
                        res.m_r0 = rhs.m_max1 ? rhs.m_r0 : m_r0 + rhs.m_r0;
                        res.m_r1 = rhs.m_max0 ? rhs.m_r1 : m_r1 + rhs.m_r1;
                        res.m_max0 = std::max({m_max0, rhs.m_max0, m_r0 + rhs.m_l0});
                        res.m_max1 = std::max({m_max1, rhs.m_max1, m_r1 + rhs.m_l1});
                        return res;
                    }
                };
                info m_info;
                void _set_zero(SizeType len) { m_info.set_zero(len); }
                void _set_one(SizeType len) { m_info.set_one(len); }
                void _flip() { m_info.flip(); }
                void _pushup(Node *lchild, Node *rchild, SizeType len1, SizeType len2) { m_info = (lchild->is_null() ? info::zero(len1) : lchild->m_info) + (rchild->is_null() ? info::zero(len2) : rchild->m_info); }
            };
        };
        template <index_type MAX_NODE>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                static Node s_buf[MAX_NODE];
                static index_type s_use_cnt;
                static constexpr Node *data() { return s_buf; }
                static index_type newnode() { return s_use_cnt++; }
            };
        };
        template <index_type MAX_NODE>
        template <typename Node>
        Node StaticBufferWrap<MAX_NODE>::type<Node>::s_buf[MAX_NODE];
        template <index_type MAX_NODE>
        template <typename Node>
        index_type StaticBufferWrap<MAX_NODE>::type<Node>::s_use_cnt = 1;
        template <typename Node>
        struct VectorBuffer {
            static std::vector<Node> s_buf;
            static Node *data() { return s_buf.data(); }
            static index_type newnode() {
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
        };
        template <typename Node>
        std::vector<Node> VectorBuffer<Node>::s_buf{Node{}};
        template <typename SizeType, bool MaintainLongest, template <typename> typename BufferType>
        struct Tree {
            struct node : NodeWrap<SizeType, MaintainLongest>::template type<node> {
                SizeType m_sum;
                bool m_flipped;
                index_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(lc); }
                node *rchild() const { return _ptr(rc); }
                void set_one(SizeType len) {
                    m_sum = len;
                    if constexpr (MaintainLongest) this->_set_one(len);
                }
                void set_zero(SizeType len) {
                    m_sum = 0;
                    if constexpr (MaintainLongest) this->_set_zero(len);
                }
                void flip(SizeType len) {
                    m_sum = len - m_sum, m_flipped = !m_flipped;
                    if constexpr (MaintainLongest) this->_flip();
                }
                void pushup(node *lchild, node *rchild, SizeType len) {
                    m_sum = lchild->m_sum + rchild->m_sum, m_flipped = false;
                    if constexpr (MaintainLongest) this->_pushup(lchild, rchild, (len + 1) >> 1, len >> 1);
                }
            };
            using buffer_type = BufferType<node>;
            using info = typename node::info;
            index_type m_root;
            SizeType m_size;
            static void _pushdown_one(index_type cur, SizeType floor, SizeType ceil) {
                SizeType len = ceil - floor + 1;
                _lchild(cur, (len + 1) >> 1)->set_one((len + 1) >> 1), _rchild(cur, len >> 1)->set_one(len >> 1);
            }
            static void _pushdown_zero(index_type cur, SizeType floor, SizeType ceil) {
                SizeType len = ceil - floor + 1;
                if (buffer_type::data()[cur].m_lc) buffer_type::data()[cur].lchild()->set_zero((len + 1) >> 1);
                if (buffer_type::data()[cur].m_rc) buffer_type::data()[cur].rchild()->set_zero(len >> 1);
            }
            static void _pushdown_flip(index_type cur, SizeType floor, SizeType ceil) {
                buffer_type::data()[cur].m_flipped = false;
                SizeType len = ceil - floor + 1;
                _lchild(cur, (len + 1) >> 1)->flip((len + 1) >> 1), _rchild(cur, len >> 1)->flip(len >> 1);
            }
            static void _pushdown(index_type cur, SizeType floor, SizeType ceil) {
                if (buffer_type::data()[cur].m_sum == ceil - floor + 1)
                    _pushdown_one(cur, floor, ceil);
                else if (!buffer_type::data()[cur].m_sum)
                    _pushdown_zero(cur, floor, ceil);
                else if (buffer_type::data()[cur].m_flipped)
                    _pushdown_flip(cur, floor, ceil);
            }
            static node *_lchild(index_type cur, SizeType len) { return _ptr(__lchild(cur, len)); }
            static node *_rchild(index_type cur, SizeType len) { return _ptr(__rchild(cur, len)); }
            static index_type __lchild(index_type cur, SizeType len) {
                if (!buffer_type::data()[cur].m_lc) {
                    index_type c = buffer_type::newnode();
                    buffer_type::data()[cur].m_lc = c;
                    if constexpr (MaintainLongest) buffer_type::data()[c].m_info = info::zero(len);
                }
                return buffer_type::data()[cur].m_lc;
            }
            static index_type __rchild(index_type cur, SizeType len) {
                if (!buffer_type::data()[cur].m_rc) {
                    index_type c = buffer_type::newnode();
                    buffer_type::data()[cur].m_rc = c;
                    if constexpr (MaintainLongest) buffer_type::data()[c].m_info = info::zero(len);
                }
                return buffer_type::data()[cur].m_rc;
            }
            static void _pushup(index_type cur, SizeType len) {
                node *p = _ptr(cur);
                p->pushup(p->lchild(), p->rchild(), len);
            }
            static void _set(index_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return;
                if (left <= floor && right >= ceil)
                    p->set_one(ceil - floor + 1);
                else {
                    if (!p->m_sum)
                        _pushdown_zero(cur, floor, ceil);
                    else if (p->m_flipped)
                        _pushdown_flip(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _set(__lchild(cur, mid - floor + 1), floor, mid, left, right);
                    if (right > mid) _set(__rchild(cur, ceil - mid), mid + 1, ceil, left, right);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            static void _set(index_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return;
                if (floor == ceil)
                    p->set_one(1);
                else {
                    if (!p->m_sum)
                        _pushdown_zero(cur, floor, ceil);
                    else if (p->m_flipped)
                        _pushdown_flip(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _set(__lchild(cur, mid - floor + 1), floor, mid, i);
                    else
                        _set(__rchild(cur, ceil - mid), mid + 1, ceil, i);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            static void _reset(index_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (!p->m_sum) return;
                if (left <= floor && right >= ceil)
                    p->set_zero(ceil - floor + 1);
                else {
                    if (p->m_sum == ceil - floor + 1)
                        _pushdown_one(cur, floor, ceil), p = _ptr(cur);
                    else if (p->m_flipped)
                        _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _reset(p->m_lc, floor, mid, left, right), p = _ptr(cur);
                    if (right > mid) _reset(p->m_rc, mid + 1, ceil, left, right);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            static void _reset(index_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (!p->m_sum) return;
                if (floor == ceil)
                    p->set_zero(ceil - floor + 1);
                else {
                    if (p->m_sum == ceil - floor + 1)
                        _pushdown_one(cur, floor, ceil), p = _ptr(cur);
                    else if (p->m_flipped)
                        _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _reset(p->m_lc, floor, mid, i);
                    else
                        _reset(p->m_rc, mid + 1, ceil, i);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            static void _flip(index_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (left <= floor && right >= ceil)
                    p->flip(ceil - floor + 1);
                else {
                    _pushdown(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _flip(__lchild(cur, mid - floor + 1), floor, mid, left, right);
                    if (right > mid) _flip(__rchild(cur, ceil - mid), mid + 1, ceil, left, right);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            static void _flip(index_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (floor == ceil)
                    p->flip(1);
                else {
                    _pushdown(cur, floor, ceil);
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _flip(__lchild(cur, mid - floor + 1), floor, mid, i);
                    else
                        _flip(__rchild(cur, ceil - mid), mid + 1, ceil, i);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            static SizeType _count(index_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (!p->m_sum) return 0;
                if (p->m_sum == ceil - floor + 1) return std::min(ceil, right) - std::max(floor, left) + 1;
                if (left <= floor && right >= ceil) return p->m_sum;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1, res = 0;
                if (left <= mid) res += _count(p->m_lc, floor, mid, left, right), p = _ptr(cur);
                if (right > mid) res += _count(p->m_rc, mid + 1, ceil, left, right);
                return res;
            }
            static info _longest(index_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                static_assert(MaintainLongest, "MaintainLongest Must Be True");
                node *p = _ptr(cur);
                if (!p->m_sum) return info::zero(std::min(ceil, right) - std::max(floor, left) + 1);
                if (p->m_sum == ceil - floor + 1) return info::one(std::min(ceil, right) - std::max(floor, left) + 1);
                if (left <= floor && right >= ceil) return p->m_info;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                info res{};
                if (left <= mid) res = res + _longest(p->m_lc, floor, mid, left, right), p = _ptr(cur);
                if (right > mid) res = res + _longest(p->m_rc, mid + 1, ceil, left, right);
                return res;
            }
            static bool _any(index_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (!p->m_sum) return false;
                if (p->m_sum == ceil - floor + 1) return true;
                if (left <= floor && right >= ceil) return true;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (left <= mid && _any(p->m_lc, floor, mid, left, right)) return true;
                return right > mid && _any(_ptr(cur)->m_rc, mid + 1, ceil, left, right);
            }
            static bool _all(index_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                node *p = _ptr(cur);
                if (!p->m_sum) return 0;
                if (p->m_sum == ceil - floor + 1) return true;
                if (left <= floor && right >= ceil) return false;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1, res = 0;
                if (left <= mid && !_all(p->m_lc, floor, mid, left, right)) return false;
                return right <= mid || _all(_ptr(cur)->m_rc, mid + 1, ceil, left, right);
            }
            static SizeType _first_one(index_type cur, SizeType floor, SizeType ceil) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return floor;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (p->lchild()->m_sum) return _first_one(p->m_lc, floor, mid);
                return _first_one(p->m_rc, mid + 1, ceil);
            }
            static SizeType _last_one(index_type cur, SizeType floor, SizeType ceil) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return ceil;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (p->rchild()->m_sum) return _last_one(p->m_rc, mid + 1, ceil);
                return _last_one(p->m_lc, floor, mid);
            }
            static SizeType _first_zero(index_type cur, SizeType floor, SizeType ceil) {
                node *p = _ptr(cur);
                if (!p->m_sum) return floor;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (p->lchild()->m_sum != mid - floor + 1) return _first_zero(p->m_lc, floor, mid);
                return _first_zero(p->m_rc, mid + 1, ceil);
            }
            static SizeType _last_zero(index_type cur, SizeType floor, SizeType ceil) {
                node *p = _ptr(cur);
                if (!p->m_sum) return ceil;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (p->rchild()->m_sum != ceil - mid) return _last_zero(p->m_rc, mid + 1, ceil);
                return _last_zero(p->m_lc, floor, mid);
            }
            static SizeType _prev_one(index_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (!p->m_sum) return -1;
                if (p->m_sum == ceil - floor + 1) return std::min(i - 1, ceil);
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i > mid + 1) {
                    SizeType res = _prev_one(p->m_rc, mid + 1, ceil, i);
                    if (~res) return res;
                }
                return _prev_one(p->m_lc, floor, mid, i);
            }
            static SizeType _next_one(index_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (!p->m_sum) return -1;
                if (p->m_sum == ceil - floor + 1) return std::max(i + 1, floor);
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i + 1 <= mid) {
                    SizeType res = _next_one(p->m_lc, floor, mid, i);
                    if (~res) return res;
                }
                return _next_one(p->m_rc, mid + 1, ceil, i);
            }
            static SizeType _prev_zero(index_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return -1;
                if (!p->m_sum) return std::min(i - 1, ceil);
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i > mid + 1) {
                    SizeType res = _prev_zero(p->m_rc, mid + 1, ceil, i);
                    if (~res) return res;
                }
                return _prev_zero(p->m_lc, floor, mid, i);
            }
            static SizeType _next_zero(index_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return -1;
                if (!p->m_sum) return std::max(i + 1, floor);
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i + 1 <= mid) {
                    SizeType res = _next_zero(p->m_lc, floor, mid, i);
                    if (~res) return res;
                }
                return _next_zero(p->m_rc, mid + 1, ceil, i);
            }
            static bool _at(index_type cur, SizeType floor, SizeType ceil, SizeType i) {
                node *p = _ptr(cur);
                if (!p->m_sum) return false;
                if (p->m_sum == ceil - floor + 1) return true;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (i <= mid)
                    return _at(p->m_lc, floor, mid, i);
                else
                    return _at(p->m_rc, mid + 1, ceil, i);
            }
            static SizeType _kth(index_type cur, SizeType floor, SizeType ceil, SizeType k) {
                node *p = _ptr(cur);
                if (p->m_sum == ceil - floor + 1) return floor + k;
                if (p->m_flipped) _pushdown_flip(cur, floor, ceil), p = _ptr(cur);
                SizeType mid = (floor + ceil) >> 1;
                if (k < p->lchild()->m_sum)
                    return _kth(p->m_lc, floor, mid, k);
                else
                    return _kth(p->m_rc, mid + 1, ceil, k - p->lchild()->m_sum);
            }
            static node *_ptr(index_type cur) { return buffer_type::data() + cur; }
            static void _reserve(index_type capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node>>::value, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            Tree(SizeType length = 0) { resize(length); }
            void resize(SizeType length) {
                if (m_size = length) m_root = buffer_type::newnode();
            }
            SizeType size() const { return m_size; }
            void set() { _ptr(m_root)->m_sum = m_size; }
            void set(SizeType i) { _set(m_root, 0, m_size - 1, i); }
            void set(SizeType left, SizeType right) { _set(m_root, 0, m_size - 1, left, right); }
            void reset() { _ptr(m_root)->m_sum = 0; }
            void reset(SizeType i) { _reset(m_root, 0, m_size - 1, i); }
            void reset(SizeType left, SizeType right) { _reset(m_root, 0, m_size - 1, left, right); }
            void flip() { _ptr(m_root)->flip(m_size); }
            void flip(SizeType i) { _flip(m_root, 0, m_size - 1, i); }
            void flip(SizeType left, SizeType right) { _flip(m_root, 0, m_size - 1, left, right); }
            SizeType count() const { return _ptr(m_root)->m_sum; }
            SizeType count(SizeType left, SizeType right) const { return _count(m_root, 0, m_size - 1, left, right); }
            bool any() const { return _ptr(m_root)->m_sum; }
            bool any(SizeType left, SizeType right) const { return _any(m_root, 0, m_size - 1, left, right); }
            bool all() const { return _ptr(m_root)->m_sum == m_size; }
            bool all(SizeType left, SizeType right) const { return _all(m_root, 0, m_size - 1, left, right); }
            SizeType first_one() const { return _ptr(m_root)->m_sum ? _first_one(m_root, 0, m_size - 1) : -1; }
            SizeType last_one() const { return _ptr(m_root)->m_sum ? _last_one(m_root, 0, m_size - 1) : -1; }
            SizeType first_zero() const { return _ptr(m_root)->m_sum < m_size ? _first_zero(m_root, 0, m_size - 1) : -1; }
            SizeType last_zero() const { return _ptr(m_root)->m_sum < m_size ? _last_zero(m_root, 0, m_size - 1) : -1; }
            SizeType prev_one(SizeType i) const { return i ? _prev_one(m_root, 0, m_size - 1, i) : -1; }
            SizeType next_one(SizeType i) const { return i + 1 < m_size ? _next_one(m_root, 0, m_size - 1, i) : -1; }
            SizeType prev_zero(SizeType i) const { return i ? _prev_zero(m_root, 0, m_size - 1, i) : -1; }
            SizeType next_zero(SizeType i) const { return i + 1 < m_size ? _next_zero(m_root, 0, m_size - 1, i) : -1; }
            bool at(SizeType i) const { return _at(m_root, 0, m_size - 1, i); }
            bool operator[](SizeType i) const { return at(i); }
            SizeType kth(SizeType k, SizeType pos = 0) const { return _kth(m_root, 0, m_size - 1, (pos ? count(0, pos - 1) : 0) + k); }
            SizeType longest_ones(SizeType left, SizeType right) const { return _longest(m_root, 0, m_size - 1, left, right).m_max1; }
            SizeType longest_zeros(SizeType left, SizeType right) const { return _longest(m_root, 0, m_size - 1, left, right).m_max0; }
        };
        template <typename Ostream, typename SizeType, bool MaintainLongest, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, Tree<SizeType, MaintainLongest, BufferType> &x) {
            out << "[";
            for (SizeType i = 0; i < x.count(); i++) {
                if (i) out << ", ";
                out << x.kth(i);
            }
            return out << "]";
        }
    }
    template <typename SizeType, bool MaintainLongest = false, LazyBitset::index_type MAX_NODE = 1 << 22>
    using StaticLazyBitset = LazyBitset::Tree<SizeType, MaintainLongest, LazyBitset::StaticBufferWrap<MAX_NODE>::template type>;
    template <typename SizeType, bool MaintainLongest = false>
    using VectorLazyBitset = LazyBitset::Tree<SizeType, MaintainLongest, LazyBitset::VectorBuffer>;
}

#endif