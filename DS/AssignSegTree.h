/*
最后修改:
20241010
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ASSEGTREE__
#define __OY_ASSEGTREE__

#include <limits>
#include <numeric>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace ASSEG {
        using size_type = uint32_t;
        template <typename Tp, Tp Identity>
        struct NoOp {
            using value_type = Tp;
            static value_type identity() { return Identity; }
        };
        template <typename Tp>
        struct AddMonoid {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            template <typename SizeType>
            static Tp pow(const Tp &x, SizeType n) { return x * n; }
        };
        template <typename Tp>
        struct BitxorMonoid {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x ^ y; }
            template <typename SizeType>
            static Tp pow(const Tp &x, SizeType n) { return n & 1 ? x : Tp{}; }
        };
        template <typename Tp, Tp Identity, typename Operation>
        struct ValLazyMonoid {
            using value_type = Tp;
            static constexpr bool val_is_lazy = true;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
        };
        template <typename Tp, Tp Identity, typename Operation, typename Pow>
        struct FastPowMonoid {
            using value_type = Tp;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
            template <typename SizeType>
            static Tp pow(const Tp &x, SizeType n) { return Pow()(x, n); }
        };
        template <typename Tp, Tp Identity, typename Operation>
        struct LazyMonoid {
            using value_type = Tp;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Tp, typename ValueType, typename SizeType, typename = void>
        struct Has_Pow : std::false_type {};
        template <typename Tp, typename ValueType, typename SizeType>
        struct Has_Pow<Tp, ValueType, SizeType, void_t<decltype(Tp::pow(std::declval<ValueType>(), std::declval<SizeType>()))>> : std::true_type {};
        template <typename Tp, typename ValueType>
        struct Has_Pow<Tp, ValueType, void, void_t<decltype(Tp::pow(std::declval<ValueType>()))>> : std::true_type {};
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Op : std::false_type {};
        template <typename Tp, typename ValueType>
        struct Has_Op<Tp, ValueType, void_t<decltype(Tp::op(std::declval<ValueType>(), std::declval<ValueType>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_val_is_lazy : std::false_type {};
        template <typename Tp>
        struct Has_val_is_lazy<Tp, void_t<decltype(Tp::val_is_lazy)>> : std::integral_constant<bool, Tp::val_is_lazy> {};
        template <typename Tp, bool ValIsLazy, bool FastPow>
        struct AssignNode {
            Tp m_val;
            bool m_cover;
            bool has_lazy() const { return m_cover; }
            const Tp &get_lazy() const { return m_val; }
            void clear_lazy() { m_cover = false; }
        };
        template <typename Tp>
        struct AssignNode<Tp, false, true> {
            Tp m_val, m_lazy;
            bool m_cover;
            bool has_lazy() const { return m_cover; }
            const Tp &get_lazy() const { return m_lazy; }
            void clear_lazy() { m_cover = false; }
        };
        template <typename Tp>
        struct AssignNode<Tp, false, false> {
            Tp m_val;
            const Tp *m_lazy;
            bool has_lazy() const { return m_lazy; }
            const Tp *get_lazy() const { return m_lazy; }
            void clear_lazy() { m_lazy = nullptr; }
        };
        template <typename Tp, size_type BATCH>
        struct LazyPool {
            std::vector<std::vector<Tp>> m_bufs;
            Tp *m_cur, *m_end;
            void malloc(Tp *&ptr, size_type len) {
                if (m_end - m_cur < len) m_bufs.emplace_back(BATCH), m_cur = m_bufs.back().data(), m_end = m_bufs.back().data() + BATCH;
                ptr = m_cur, m_cur += len;
            }
        };
        template <typename Monoid, typename SizeType = uint64_t, size_type BATCH = 1 << 17, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Tree {
        public:
            using group = Monoid;
            using value_type = typename group::value_type;
            static constexpr bool has_op = Has_Op<group, value_type>::value, val_is_lazy = !has_op || Has_val_is_lazy<group>::value, has_fast_pow = Has_Pow<group, value_type, SizeType>::value;
            struct node : AssignNode<value_type, val_is_lazy, has_fast_pow> {
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            struct iterator {
                SizeType m_index;
                node *m_ptr;
            };
            node *_root() const { return _ptr(m_rt); }
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            static LazyPool<value_type, BATCH> s_pool;
            size_type m_rt;
            SizeType m_size, m_cap;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode() {
                size_type x = buffer_type::newnode();
                _ptr(x)->m_val = group::identity();
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
            static value_type _query(size_type cur, SizeType floor, SizeType ceil, SizeType i) {
                auto p = _ptr(cur);
                if (floor + 1 == ceil) return p->m_val;
                SizeType mid = (floor + ceil) >> 1;
                if (i < mid)
                    return p->m_lc ? _pushdown(cur, std::countr_zero(ceil - floor)), _query(_ptr(cur)->m_lc, floor, mid, i) : _under(p, 1);
                else
                    return p->m_rc ? _pushdown(cur, std::countr_zero(ceil - floor)), _query(_ptr(cur)->m_rc, mid, ceil, i) : _under(p, 1);
            }
            static value_type _query(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                auto p = _ptr(cur);
                if (floor == left && ceil == right) return p->m_val;
                SizeType mid = (floor + ceil) >> 1;
                if (right <= mid)
                    return p->m_lc ? _pushdown(cur, std::countr_zero(ceil - floor)), _query(_ptr(cur)->m_lc, floor, mid, left, right) : _under(p, right - left);
                else if (left >= mid)
                    return p->m_rc ? _pushdown(cur, std::countr_zero(ceil - floor)), _query(_ptr(cur)->m_rc, mid, ceil, left, right) : _under(p, right - left);
                else if (!p->m_lc && !p->m_rc)
                    return _under(p, right - left);
                else {
                    _pushdown(cur, std::countr_zero(ceil - floor));
                    return group::op(_query(_ptr(cur)->m_lc, floor, mid, left, mid), _query(_ptr(cur)->m_rc, mid, ceil, mid, right));
                }
            }
            static size_type _modify(size_type cur, SizeType floor, SizeType ceil, SizeType i, const value_type &val) {
                if (!cur) cur = _newnode();
                if (floor + 1 == ceil)
                    _ptr(cur)->m_val = val;
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    _pushdown(cur, std::countr_zero(ceil - floor));
                    if (i < mid) {
                        size_type lc = _modify(_ptr(cur)->m_lc, floor, mid, i, val);
                        _ptr(cur)->m_lc = lc;
                    } else {
                        size_type rc = _modify(_ptr(cur)->m_rc, mid, ceil, i, val);
                        _ptr(cur)->m_rc = rc;
                    }
                    _pushup(_ptr(cur));
                }
                return cur;
            }
            template <typename Lazy>
            static size_type _modify(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, const Lazy &lazy) {
                if (!cur) cur = _newnode();
                if (floor == left && ceil == right)
                    _apply(_ptr(cur), lazy, std::countr_zero(ceil - floor));
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    _pushdown(cur, std::countr_zero(ceil - floor));
                    if (left < mid) {
                        size_type lc = _modify(_ptr(cur)->m_lc, floor, mid, left, std::min(mid, right), lazy);
                        _ptr(cur)->m_lc = lc;
                    }
                    if (right > mid) {
                        size_type rc = _modify(_ptr(cur)->m_rc, mid, ceil, std::max(mid, left), right, lazy);
                        _ptr(cur)->m_rc = rc;
                    }
                    _pushup(_ptr(cur));
                }
                return cur;
            }
            template <typename Judger>
            static SizeType _max_right(size_type cur, SizeType floor, SizeType ceil, SizeType start, value_type &val, Judger &&judge) {
                if (start <= floor) {
                    value_type a = group::op(val, _ptr(cur)->m_val);
                    if (judge(a))
                        return val = a, ceil;
                    else if (floor + 1 == ceil)
                        return floor;
                }
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, std::countr_zero(ceil - floor));
                if (start < mid) {
                    SizeType res = _max_right(_ptr(cur)->m_lc, floor, mid, start, val, judge);
                    if (res != mid) return res;
                }
                return _max_right(_ptr(cur)->m_rc, mid, ceil, start, val, judge);
            }
            template <typename Judger>
            static SizeType _min_left(size_type cur, SizeType floor, SizeType ceil, SizeType start, value_type &val, Judger &&judge) {
                if (start >= ceil) {
                    value_type a = start > ceil ? group::op(_ptr(cur)->m_val, val) : _ptr(cur)->m_val;
                    if (judge(a))
                        return val = a, floor;
                    else if (floor == ceil)
                        return ceil + 1;
                }
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, std::countr_zero(ceil - floor + 1));
                if (start > mid) {
                    SizeType res = _min_left(_ptr(cur)->m_rc, mid + 1, ceil, start, val, judge);
                    if (res != mid + 1) return res;
                }
                return _min_left(_ptr(cur)->m_lc, floor, mid, start, val, judge);
            }
            static void _apply(node *p, const value_type *lazy, size_type level) { p->m_val = lazy[level], p->m_lazy = lazy; }
            static void _apply(node *p, const value_type &lazy, size_type level) {
                if constexpr (has_fast_pow)
                    p->m_val = group::pow(lazy, SizeType(1) << level);
                else
                    p->m_val = lazy;
                if constexpr (!val_is_lazy) p->m_lazy = lazy;
                p->m_cover = true;
            }
            static value_type _under(node *p, SizeType len) {
                if constexpr (has_fast_pow)
                    return group::pow(p->m_lazy, len);
                else if constexpr (val_is_lazy)
                    return p->m_val;
                else {
                    value_type res = group::identity();
                    while (len) {
                        size_type ctz = std::countr_zero(len);
                        res = group::op(res, p->m_lazy[ctz]), len -= SizeType(1) << ctz;
                    }
                    return res;
                }
            }
            static void _pushdown(size_type cur, size_type level) {
                if (!_ptr(cur)->has_lazy()) return;
                auto lazy = _ptr(cur)->get_lazy();
                _apply(_ptr(_lchild(cur)), lazy, level - 1), _apply(_ptr(_rchild(cur)), lazy, level - 1), _ptr(cur)->clear_lazy();
            }
            static void _pushup(node *p) {
                if constexpr (has_op) p->m_val = group::op(p->lchild()->m_val, p->rchild()->m_val);
            }
        public:
            Tree(SizeType length = 0) { resize(length); }
            SizeType size() const { return m_size; }
            void resize(SizeType length) {
                if (!(m_size = length)) return;
                m_cap = std::bit_ceil(m_size), m_rt = 0, modify(0, m_cap - 1, group::identity());
            }
            void modify(size_type i, value_type val) { m_rt = _modify(m_rt, 0, m_cap, i, val); }
            void modify(size_type left, size_type right, value_type val) {
                if (left == right) return modify(left, val);
                if constexpr (val_is_lazy || has_fast_pow)
                    m_rt = _modify(m_rt, 0, m_cap, left, right + 1, val);
                else {
                    value_type *lazy;
                    size_type j = std::bit_width(left ^ right) + 1;
                    s_pool.malloc(lazy, j);
                    lazy[0] = val;
                    for (size_type i = 1; i != j; i++)
                        if constexpr (Has_Pow<group, value_type, void>::value)
                            lazy[i] = group::pow(lazy[i - 1]);
                        else
                            lazy[i] = group::op(lazy[i - 1], lazy[i - 1]);
                    m_rt = _modify(m_rt, 0, m_cap, left, right + 1, lazy);
                }
            }
            template <typename Judger>
            SizeType max_right(SizeType left, Judger &&judge) const {
                value_type val = group::identity();
                return std::min(_max_right(m_rt, 0, m_cap, left, val, judge), m_size) - 1;
            }
            template <typename Judger>
            SizeType min_left(SizeType right, Judger &&judge) const {
                value_type val = group::identity();
                return _min_left(m_rt, 0, m_cap - 1, right, val, judge);
            }
            value_type query(SizeType i) const { return _query(m_rt, 0, m_cap, i); }
            value_type query(SizeType left, SizeType right) const { return _query(m_rt, 0, m_cap, left, right + 1); }
            value_type query_all() const { return _ptr(m_rt)->m_val; }
        };
        template <typename Monoid, typename SizeType, size_type BATCH, template <typename> typename BufferType>
        LazyPool<typename Tree<Monoid, SizeType, BATCH, BufferType>::value_type, BATCH> Tree<Monoid, SizeType, BATCH, BufferType>::s_pool;
        template <typename Ostream, typename Monoid, typename SizeType, size_type BATCH, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Monoid, SizeType, BATCH, BufferType> &x) {
            out << "[";
            for (SizeType i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, Tp Identity, typename SizeType, typename Operation, typename Pow, typename TreeType = ASSEG::Tree<ASSEG::FastPowMonoid<Tp, Identity, Operation, Pow>, SizeType>>
    auto make_fast_pow_AssignSegTree(SizeType length, Operation op, Pow pow) -> TreeType { return TreeType(length); }
    template <typename Tp, Tp Identity, ASSEG::size_type BATCH = 1 << 17, typename SizeType, typename Operation, typename TreeType = ASSEG::Tree<ASSEG::LazyMonoid<Tp, Identity, Operation>, SizeType, BATCH>>
    auto make_lazy_AssignSegTree(SizeType length, Operation op) -> TreeType { return TreeType(length); }
    template <typename Tp, Tp Identity, typename SizeType = uint64_t, ASSEG::size_type BATCH = 1 << 17>
    using VectorAssignSeg = ASSEG::Tree<ASSEG::NoOp<Tp, Identity>, SizeType>;
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), typename SizeType = uint64_t, ASSEG::size_type BATCH = 1 << 17>
    using VectorAssignMaxSeg = ASSEG::Tree<ASSEG::ValLazyMonoid<Tp, Minimum, ASSEG::ChoiceByCompare<Tp, std::less<Tp>>>, SizeType, BATCH>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), typename SizeType = uint64_t, ASSEG::size_type BATCH = 1 << 17>
    using VectorAssignMinSeg = ASSEG::Tree<ASSEG::ValLazyMonoid<Tp, Maximum, ASSEG::ChoiceByCompare<Tp, std::greater<Tp>>>, SizeType, BATCH>;
    template <typename Tp, typename SizeType = uint64_t, ASSEG::size_type BATCH = 1 << 17>
    using VectorAssignGcdSeg = ASSEG::Tree<ASSEG::ValLazyMonoid<Tp, 0, ASSEG::FpTransfer<Tp, std::gcd<Tp>>>, SizeType, BATCH>;
    template <typename Tp, typename SizeType = uint64_t, ASSEG::size_type BATCH = 1 << 17>
    using VectorAssignLcmSeg = ASSEG::Tree<ASSEG::ValLazyMonoid<Tp, 0, ASSEG::FpTransfer<Tp, std::lcm<Tp>>>, SizeType, BATCH>;
    template <typename Tp, Tp OneMask = Tp(-1), typename SizeType = uint64_t, ASSEG::size_type BATCH = 1 << 17>
    using VectorAssignBitAndSeg = ASSEG::Tree<ASSEG::ValLazyMonoid<Tp, OneMask, std::bit_and<Tp>>, SizeType, BATCH>;
    template <typename Tp, Tp ZeroMask = 0, typename SizeType = uint64_t, ASSEG::size_type BATCH = 1 << 17>
    using VectorAssignBitOrSeg = ASSEG::Tree<ASSEG::ValLazyMonoid<Tp, ZeroMask, std::bit_or<Tp>>, SizeType, BATCH>;
    template <typename Tp, typename SizeType = uint64_t, ASSEG::size_type BATCH = 1 << 17>
    using VectorAssignBitxorSeg = ASSEG::Tree<ASSEG::BitxorMonoid<Tp>, SizeType, BATCH>;
    template <typename Tp, typename SizeType = uint64_t, ASSEG::size_type BATCH = 1 << 17>
    using VectorAssignSumSeg = ASSEG::Tree<ASSEG::AddMonoid<Tp>, SizeType, BATCH>;
}

#endif