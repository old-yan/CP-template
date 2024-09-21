/*
最后修改:
20240829
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTCOMPRESSEDTREE__
#define __OY_PERSISTENTCOMPRESSEDTREE__

#include <functional>
#include <limits>
#include <numeric>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace PerCPTREE {
        using size_type = uint32_t;
        template <typename Tp, Tp Identity, typename Operation>
        struct BaseMonoid {
            using value_type = Tp;
            static constexpr Tp identity() { return Identity; }
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        struct DefaultFilter {
            static constexpr bool get_left(size_type w) { return false; }
            template <typename Tp>
            void set_low_high(Tp lca, size_type &w, Tp &low, Tp &high) const {
                static constexpr Tp mask = Tp(1) << ((sizeof(Tp) << 3) - 1);
                w = std::countl_zero(lca), low = lca << w ^ mask, high = (lca + 1) << w ^ mask;
            }
        };
        template <typename Monoid, bool Lock = false, typename SizeType = uint64_t, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Tree {
            static_assert(std::is_unsigned<SizeType>::value, "SizeType Must Be Unsiged");
        public:
            static constexpr SizeType mask = SizeType(1) << ((sizeof(SizeType) << 3) - 1);
            using group = Monoid;
            using value_type = typename group::value_type;
            struct node {
                value_type m_val;
                SizeType m_lca;
                size_type m_lc, m_rc;
                SizeType key() const { return m_lca ^ mask; }
                bool is_leaf() const { return m_lca & mask; }
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using tree_type = Tree<group, Lock, SizeType, BufferType>;
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            static void lock() { s_lock = true; }
            static void unlock() { s_lock = false; }
        private:
            static bool s_lock;
            size_type m_root;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode() { return buffer_type::newnode(); }
            static size_type _newnode(value_type val, SizeType lca) {
                size_type x = _newnode();
                _ptr(x)->m_val = val, _ptr(x)->m_lca = lca;
                return x;
            }
            static size_type _lchild(size_type cur) {
                if (!_ptr(cur)->m_lc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_lc = c;
                }
                return _ptr(cur)->m_lc;
            }
            static size_type _rchild(size_type cur) {
                if (!_ptr(cur)->m_rc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_rc = c;
                }
                return _ptr(cur)->m_rc;
            }
            static void _pushup(size_type cur) { _ptr(cur)->m_val = group::op(_ptr(cur)->lchild()->m_val, _ptr(cur)->rchild()->m_val); }
            template <typename InitMapping>
            static size_type _initnode(SizeType floor, SizeType ceil, InitMapping &&mapping) {
                if (floor + 1 == ceil) return _newnode(mapping(floor), floor | mask);
                size_type w = std::bit_width(floor ^ (ceil - 1)), rt = _newnode(), lc = _initnode(floor, floor + (SizeType(1) << (w - 1)), mapping), rc = _initnode(floor + (SizeType(1) << (w - 1)), ceil, mapping);
                _ptr(rt)->m_val = group::op(_ptr(lc)->m_val, _ptr(rc)->m_val), _ptr(rt)->m_lca = (floor | mask) >> w, _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = rc;
                return rt;
            }
            static size_type _modify(size_type cur, SizeType i, value_type val) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                SizeType x = (i | mask) >> w;
                if (x < p->m_lca) {
                    size_type rt = _newnode(group::op(val, p->m_val), x >> std::bit_width(x ^ p->m_lca)), lc = _newnode(val, i | mask);
                    _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = cur;
                    return rt;
                }
                if (x > p->m_lca) {
                    size_type rt = _newnode(group::op(p->m_val, val), x >> std::bit_width(x ^ p->m_lca)), rc = _newnode(val, i | mask);
                    _ptr(rt)->m_lc = cur, _ptr(rt)->m_rc = rc;
                    return rt;
                }
                if (!w) return p->m_val = val, cur;
                if (i >> (w - 1) & 1) {
                    size_type rc = p->m_rc ? _modify(_copynode(p->m_rc), i, val) : _newnode(val, i | mask);
                    _ptr(cur)->m_rc = rc;
                } else {
                    size_type lc = p->m_lc ? _modify(_copynode(p->m_lc), i, val) : _newnode(val, i | mask);
                    _ptr(cur)->m_lc = lc;
                }
                return _pushup(cur), cur;
            }
            static size_type _add(size_type cur, SizeType i, value_type val) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                SizeType x = (i | mask) >> w;
                if (x < p->m_lca) {
                    size_type rt = _newnode(group::op(val, p->m_val), x >> std::bit_width(x ^ p->m_lca)), lc = _newnode(val, i | mask);
                    _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = cur;
                    return rt;
                }
                if (x > p->m_lca) {
                    size_type rt = _newnode(group::op(p->m_val, val), x >> std::bit_width(x ^ p->m_lca)), rc = _newnode(val, i | mask);
                    _ptr(rt)->m_lc = cur, _ptr(rt)->m_rc = rc;
                    return rt;
                }
                p->m_val = group::op(p->m_val, val);
                if (w)
                    if (i >> (w - 1) & 1) {
                        size_type rc = p->m_rc ? _add(_copynode(p->m_rc), i, val) : _newnode(val, i | mask);
                        _ptr(cur)->m_rc = rc;
                    } else {
                        size_type lc = p->m_lc ? _add(_copynode(p->m_lc), i, val) : _newnode(val, i | mask);
                        _ptr(cur)->m_lc = lc;
                    }
                return cur;
            }
            static bool _contains(size_type cur, SizeType i) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                if (((i | mask) >> w) != p->m_lca) return false;
                if (!w) return true;
                if (i >> (w - 1) & 1)
                    return p->m_rc && _contains(p->m_rc, i);
                else
                    return p->m_lc && _contains(p->m_lc, i);
            }
            static value_type _query(size_type cur, SizeType i) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                if (((i | mask) >> w) != p->m_lca) return group::identity();
                if (!w) return p->m_val;
                if (i >> (w - 1) & 1)
                    return p->m_rc ? _query(p->m_rc, i) : group::identity();
                else
                    return p->m_lc ? _query(p->m_lc, i) : group::identity();
            }
            template <typename Filter = DefaultFilter>
            static value_type _query(size_type cur, SizeType key_low, SizeType key_high, Filter &&filter) {
                node *p = _ptr(cur);
                size_type w;
                SizeType low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                key_low = std::max(key_low, low), key_high = std::min(key_high, --high);
                if (key_low > key_high) return group::identity();
                if (key_low == low && key_high == high) return p->m_val;
                if (!(key_high >> (w - 1) & 1))
                    return _query(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key_low, key_high, filter);
                else if (key_low >> (w - 1) & 1)
                    return _query(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key_low, key_high, filter);
                else
                    return group::op(_query(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key_low, key_high, filter), _query(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key_low, key_high, filter));
            }
            template <typename Filter = DefaultFilter, typename Judger>
            static SizeType _max_right(size_type cur, SizeType start, SizeType lim, value_type &val, Filter &&filter, Judger &&judge) {
                if (!cur) return lim;
                node *p = _ptr(cur);
                size_type w;
                SizeType low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                if (start >= high) return lim;
                if (start <= low) {
                    auto a = group::op(val, p->m_val);
                    if (judge(a))
                        return val = a, lim;
                    else if (!w)
                        return low;
                }
                SizeType mid = (low + high) >> 1;
                if (start <= mid) {
                    SizeType res = _max_right(_ptr(cur)->m_lc, start, mid, val, filter, judge);
                    if (res != mid) return res;
                }
                return _max_right(_ptr(cur)->m_rc, start, lim, val, filter, judge);
            }
            template <typename Filter = DefaultFilter, typename Judger>
            static SizeType _min_left(size_type cur, SizeType start, SizeType lim, value_type &val, Filter &&filter, Judger &&judge) {
                if (!cur) return lim;
                node *p = _ptr(cur);
                size_type w;
                SizeType low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                if (start <= low) return lim;
                if (start >= high) {
                    auto a = group::op(p->m_val, val);
                    if (judge(a))
                        return val = a, lim;
                    else if (!w)
                        return high;
                }
                SizeType mid = (low + high) >> 1;
                if (start > mid) {
                    SizeType res = _min_left(_ptr(cur)->m_rc, start, mid, val, filter, judge);
                    if (res != mid) return res;
                }
                return _min_left(_ptr(cur)->m_lc, start, lim, val, filter, judge);
            }
            template <typename Callback>
            static void _dfs(size_type cur, Callback &&call) {
                node *p = _ptr(cur);
                if (p->is_leaf())
                    call(p->key(), p->m_val);
                else {
                    if (p->m_lc) _dfs(p->m_lc, call);
                    if (p->m_rc) _dfs(p->m_rc, call);
                }
            }
            static size_type _copynode(size_type old) {
                if (!old) return 0;
                if constexpr (Lock)
                    if (s_lock) return old;
                size_type x = buffer_type::newnode();
                *_ptr(x) = *_ptr(old);
                return x;
            }
            node *_root() const { return _ptr(m_root); }
        public:
            Tree() { _ptr(m_root = 0)->m_val = group::identity(); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) : Tree() { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) : Tree() { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                clear();
                if (length) m_root = _initnode(0, length, mapping);
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            tree_type copy() const {
                tree_type res;
                if (m_root) {
                    res.m_root = buffer_type::newnode();
                    *res._root() = *_root();
                }
                return res;
            }
            void clear() { m_root = {}; }
            bool empty() const { return !m_root; }
            void add(SizeType i, value_type val) { m_root = m_root ? _add(m_root, i, val) : _newnode(val, i | mask); }
            void modify(SizeType i, value_type val) { m_root = m_root ? _modify(m_root, i, val) : _newnode(val, i | mask); }
            value_type query(SizeType i) const { return m_root ? _query(m_root, i) : group::identity(); }
            value_type query(SizeType left, SizeType right) const { return m_root ? _query(m_root, left, right, {}) : group::identity(); }
            value_type query_all() const { return _root()->m_val; }
            template <typename Judge>
            SizeType max_right(SizeType left, Judge &&judge) const {
                value_type val = group::identity();
                if (!judge(val)) return left - 1;
                return _max_right(m_root, left, mask, val, {}, judge) - 1;
            }
            template <typename Judge>
            SizeType min_left(SizeType right, Judge &&judge) const {
                value_type val = group::identity();
                if (!judge(val)) return right + 1;
                return _min_left(m_root, right, 0, val, {}, judge) + 1;
            }
            bool contains(SizeType i) const { return _contains(m_root, i); }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_root) _dfs(m_root, call);
            }
        };
        template <typename Monoid, bool Lock, typename SizeType, template <typename> typename BufferType>
        bool Tree<Monoid, Lock, SizeType, BufferType>::s_lock = true;
        template <typename Ostream, typename Monoid, bool Lock, typename SizeType, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Monoid, Lock, SizeType, BufferType> &x) {
            out << "[";
            auto call = [&, started = false](SizeType pos, typename Monoid::value_type val) mutable {
                if (started)
                    out << ',';
                else
                    started = true;
                out << pos << ':' << val;
            };
            x.enumerate(call);
            return out << "]";
        }
    }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), bool Lock = false, typename SizeType = uint64_t>
    using VectorPerCompressedMaxTree = PerCPTREE::Tree<PerCPTREE::BaseMonoid<Tp, Minimum, PerCPTREE::ChoiceByCompare<Tp, std::less<Tp>>>, Lock, SizeType>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), bool Lock = false, typename SizeType = uint64_t>
    using VectorPerCompressedMinTree = PerCPTREE::Tree<PerCPTREE::BaseMonoid<Tp, Maximum, PerCPTREE::ChoiceByCompare<Tp, std::greater<Tp>>>, Lock, SizeType>;
    template <typename Tp, bool Lock = false, typename SizeType = uint64_t>
    using VectorPerCompressedGcdTree = PerCPTREE::Tree<PerCPTREE::BaseMonoid<Tp, Tp(0), PerCPTREE::FpTransfer<Tp, std::gcd<Tp>>>, Lock, SizeType>;
    template <typename Tp, bool Lock = false, typename SizeType = uint64_t>
    using VectorPerCompressedLcmTree = PerCPTREE::Tree<PerCPTREE::BaseMonoid<Tp, Tp(1), PerCPTREE::FpTransfer<Tp, std::lcm<Tp>>>, Lock, SizeType>;
    template <typename Tp, Tp OneMask = Tp(-1), bool Lock = false, typename SizeType = uint64_t>
    using VectorPerCompressedBitAndTree = PerCPTREE::Tree<PerCPTREE::BaseMonoid<Tp, OneMask, std::bit_and<Tp>>, Lock, SizeType>;
    template <typename Tp, Tp ZeroMask = 0, bool Lock = false, typename SizeType = uint64_t>
    using VectorPerCompressedBitOrTree = PerCPTREE::Tree<PerCPTREE::BaseMonoid<Tp, ZeroMask, std::bit_or<Tp>>, Lock, SizeType>;
    template <typename Tp, Tp ZeroMask = 0, bool Lock = false, typename SizeType = uint64_t>
    using VectorPerCompressedBitXorTree = PerCPTREE::Tree<PerCPTREE::BaseMonoid<Tp, ZeroMask, std::bit_xor<Tp>>, Lock, SizeType>;
    template <typename Tp, Tp Zero = Tp(), bool Lock = false, typename SizeType = uint64_t>
    using VectorPerCompressedSumTree = PerCPTREE::Tree<PerCPTREE::BaseMonoid<Tp, Zero, std::plus<Tp>>, Lock, SizeType>;
}

#endif