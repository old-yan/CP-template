/*
最后修改:
20241015
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TAGSEGTREE__
#define __OY_TAGSEGTREE__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"
#include "VectorBufferWithCollect.h"

namespace OY {
    namespace TAGSEG {
        using size_type = uint32_t;
        template <typename Tp>
        struct AddMonoid {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            static Tp pow(const Tp &x, size_type n) { return x * n; }
        };
        template <typename Tp>
        struct BitxorMonoid {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x ^ y; }
            static Tp pow(const Tp &x, size_type n) { return n & 1 ? x : Tp{}; }
        };
        template <typename Tp, Tp Identity, typename Operation, typename Pow>
        struct BaseMonoid {
            using value_type = Tp;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
            static Tp pow(const Tp &x, size_type n) { return Pow()(x, n); }
        };
        template <typename Tp, Tp Identity, typename Operation>
        struct SelfPowMonoid {
            using value_type = Tp;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
            static Tp pow(const Tp &x, size_type) { return x; }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename CommutativeMonoid, typename SizeType = uint64_t, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
        public:
            using tree_type = Tree<CommutativeMonoid, SizeType, BufferType>;
            using group = CommutativeMonoid;
            using value_type = typename group::value_type;
            struct node {
                value_type m_val, m_tag;
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            struct Initializer {
                Initializer() { _ptr(0)->m_val = _ptr(0)->m_tag = group::identity(); }
            };
            node *_root() const { return _ptr(m_rt); }
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_rt{};
            SizeType m_sz;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode() {
                size_type x = buffer_type::newnode();
                _ptr(x)->m_val = _ptr(x)->m_tag = group::identity();
                return x;
            }
            static void _collect(size_type x) { _ptr(x)->m_lc = _ptr(x)->m_rc = 0, buffer_type::collect(x); }
            static void _collect_all(size_type cur) {
                if constexpr (buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(cur);
                }
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
            size_type _rt() {
                if (!m_rt) m_rt = _newnode();
                return m_rt;
            }
            static void _compound(value_type &x, const value_type &y) { x = group::op(x, y); }
            template <typename Callback>
            static size_type _init(SizeType floor, SizeType ceil, Callback &&call) {
                if (floor == ceil) {
                    size_type x = _newnode();
                    _ptr(x)->m_val = call(floor);
                    return x;
                } else {
                    SizeType mid = (floor + ceil) >> 1;
                    size_type x = _newnode(), lc = _init(floor, mid, call), rc = _init(mid + 1, ceil, call);
                    _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc, _ptr(x)->m_val = group::op(_ptr(lc)->m_val, _ptr(rc)->m_val);
                    return x;
                }
            }
            static value_type _query(size_type cur, SizeType floor, SizeType ceil, SizeType i) {
                if (!cur) return group::identity();
                auto p = _ptr(cur);
                if (floor == ceil) return p->m_val;
                SizeType mid = (floor + ceil) >> 1;
                return group::op(p->m_tag, i <= mid ? _query(p->m_lc, floor, mid, i) : _query(p->m_rc, mid + 1, ceil, i));
            }
            static value_type _query(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) {
                if (!cur) return group::identity();
                auto p = _ptr(cur);
                if (floor == left && ceil == right) return p->m_val;
                SizeType mid = (floor + ceil) >> 1;
                value_type res = group::pow(p->m_tag, right - left + 1);
                if (left <= mid) res = group::op(res, _query(p->m_lc, floor, mid, left, std::min(mid, right)));
                if (right > mid) res = group::op(res, _query(p->m_rc, mid + 1, ceil, std::max(mid + 1, left), right));
                return res;
            }
            static void _add(size_type cur, SizeType floor, SizeType ceil, SizeType i, value_type inc) {
                auto p = _ptr(cur);
                _compound(p->m_val, inc);
                if (floor == ceil) return;
                SizeType mid = (floor + ceil) >> 1;
                return i <= mid ? _add(_lchild(cur), floor, mid, i, inc) : _add(_rchild(cur), mid + 1, ceil, i, inc);
            }
            static void _add(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, value_type inc) {
                auto p = _ptr(cur);
                _compound(p->m_val, group::pow(inc, right - left + 1));
                if (floor == left && ceil == right)
                    _compound(p->m_tag, inc);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _add(_lchild(cur), floor, mid, left, std::min(mid, right), inc);
                    if (right > mid) _add(_rchild(cur), mid + 1, ceil, std::max(mid + 1, left), right, inc);
                }
            }
            template <typename Judger>
            static SizeType _max_right(size_type cur, SizeType floor, SizeType ceil, SizeType start, value_type tag, value_type &val, Judger &&judge) {
                const node *p = _ptr(cur);
                auto get = [&] { return group::op(group::pow(tag, ceil - floor + 1), p->m_val); };
                if (start <= floor) {
                    auto a = group::op(val, get());
                    if (judge(a))
                        return val = a, ceil;
                    else if (floor == ceil)
                        return floor - 1;
                }
                _compound(tag, p->m_tag);
                SizeType mid = (floor + ceil) >> 1;
                if (start <= mid) {
                    SizeType res = _max_right(_lchild(cur), floor, mid, start, tag, val, judge);
                    if (res != mid) return res;
                }
                return _max_right(_rchild(cur), mid + 1, ceil, start, tag, val, judge);
            }
            template <typename Judger>
            static SizeType _min_left(size_type cur, SizeType floor, SizeType ceil, SizeType start, value_type tag, value_type &val, Judger &&judge) {
                const node *p = _ptr(cur);
                auto get = [&] { return group::op(group::pow(tag, ceil - floor + 1), p->m_val); };
                if (start >= ceil) {
                    auto a = group::op(get(), val);
                    if (judge(a))
                        return val = a, floor;
                    else if (floor == ceil)
                        return ceil + 1;
                }
                _compound(tag, p->m_tag);
                SizeType mid = (floor + ceil) >> 1;
                if (start > mid) {
                    SizeType res = _min_left(_ptr(cur)->m_rc, mid + 1, ceil, start, tag, val, judge);
                    if (res != mid + 1) return res;
                }
                return _min_left(_ptr(cur)->m_lc, floor, mid, start, tag, val, judge);
            }
            template <typename Callback>
            static void _dfs(size_type cur, SizeType floor, SizeType ceil, value_type tag, Callback &&call) {
                if (!cur)
                    for (SizeType i = floor; i <= ceil; i++) call(tag);
                else {
                    auto p = _ptr(cur);
                    if (floor == ceil)
                        call(group::op(tag, p->m_val));
                    else {
                        SizeType mid = (floor + ceil) >> 1;
                        _compound(tag, p->m_tag), _dfs(p->m_lc, floor, mid, tag, call), _dfs(p->m_rc, mid + 1, ceil, tag, call);
                    }
                }
            }
        public:
            Tree() { static Initializer _init; }
            Tree(SizeType length) : Tree() { resize(length); }
            template <typename InitMapping>
            Tree(SizeType length, InitMapping mapping) : Tree() { resize(length, mapping); }
            Tree(const tree_type &rhs) = delete;
            Tree(tree_type &&rhs) noexcept { std::swap(m_rt, rhs.m_rt), std::swap(m_sz, rhs.m_sz); }
            ~Tree() { clear(); }
            tree_type &operator=(const tree_type &rhs) = delete;
            tree_type &operator=(tree_type &&rhs) noexcept {
                std::swap(m_rt, rhs.m_rt), std::swap(m_sz, rhs.m_sz);
                return *this;
            }
            void clear() {
                if (m_rt) _collect_all(m_rt), m_rt = 0;
            }
            SizeType size() const { return m_sz; }
            void resize(SizeType length) { m_sz = length, m_rt = 0; }
            template <typename InitMapping>
            void resize(SizeType length, InitMapping mapping) { m_sz = length, m_rt = _init(0, m_sz - 1, mapping); }
            void add(SizeType i, value_type inc) { _add(_rt(), 0, m_sz - 1, i, inc); }
            void add(SizeType left, SizeType right, value_type inc) { _add(_rt(), 0, m_sz - 1, left, right, inc); }
            value_type query(SizeType i) const { return _query(m_rt, 0, m_sz - 1, i); }
            value_type query(size_type left, size_type right) const { return _query(m_rt, 0, m_sz - 1, left, right); }
            value_type query_all() const { return _root()->m_val; }
            template <typename Judger>
            SizeType max_right(SizeType left, Judger &&judge) const { return _max_right(_rt(), 0, m_sz - 1, left, group::identity(), group::identity(), judge); }
            template <typename Judger>
            SizeType min_left(SizeType right, Judger &&judge) const { return _min_left(_rt(), 0, m_sz - 1, right, group::identity(), group::identity(), judge); }
            template <typename Callback>
            void enumerate(Callback &&call) const { _dfs(m_rt, 0, m_sz - 1, group::identity(), call); }
        };
        template <typename Ostream, typename CommutativeMonoid, typename SizeType, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<CommutativeMonoid, SizeType, BufferType> &x) {
            out << "[";
            for (SizeType i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, Tp Identity, typename SizeType, typename Operation, typename Pow, template <typename> typename BufferType = VectorBufferWithCollect, typename TreeType = TAGSEG::Tree<TAGSEG::BaseMonoid<Tp, Identity, Operation, Pow>, SizeType, BufferType>>
    auto make_TagSeg(SizeType length, Operation op, Pow pow) -> TreeType { return TreeType(length); }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), typename SizeType = uint64_t>
    using VectorTagMaxSeg = TAGSEG::Tree<TAGSEG::SelfPowMonoid<Tp, Minimum, TAGSEG::ChoiceByCompare<Tp, std::less<Tp>>>, SizeType>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), typename SizeType = uint64_t>
    using VectorTagMinSeg = TAGSEG::Tree<TAGSEG::SelfPowMonoid<Tp, Maximum, TAGSEG::ChoiceByCompare<Tp, std::greater<Tp>>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorTagGcdSeg = TAGSEG::Tree<TAGSEG::SelfPowMonoid<Tp, 0, TAGSEG::FpTransfer<Tp, std::gcd<Tp>>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorTagLcmSeg = TAGSEG::Tree<TAGSEG::SelfPowMonoid<Tp, 1, TAGSEG::FpTransfer<Tp, std::lcm<Tp>>>, SizeType>;
    template <typename Tp, Tp OneMask = Tp(-1), typename SizeType = uint64_t>
    using VectorTagBitAndSeg = TAGSEG::Tree<TAGSEG::SelfPowMonoid<Tp, OneMask, std::bit_and<Tp>>, SizeType>;
    template <typename Tp, Tp ZeroMask = Tp(0), typename SizeType = uint64_t>
    using VectorTagBitOrSeg = TAGSEG::Tree<TAGSEG::SelfPowMonoid<Tp, ZeroMask, std::bit_or<Tp>>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorTagBitXorSeg = TAGSEG::Tree<TAGSEG::BitxorMonoid<Tp>, SizeType>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorTagSumSeg = TAGSEG::Tree<TAGSEG::AddMonoid<Tp>, SizeType>;
}

#endif