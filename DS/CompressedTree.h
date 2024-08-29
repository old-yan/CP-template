/*
最后修改:
20240828
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_COMPRESSEDTREE__
#define __OY_COMPRESSEDTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace CPTREE {
        using size_type = uint32_t;
        template <typename Tp, Tp Val>
        struct ConstexprIdentity {
            template <typename... Args>
            Tp operator()(Args...) const { return Val; }
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
        template <typename Tp>
        struct BitxorFilter {
            Tp m_val;
            Tp get_left(size_type w) const { return m_val >> w & 1; }
            void set_low_high(Tp lca, size_type &w, Tp &low, Tp &high) const {
                static constexpr Tp mask = Tp(1) << ((sizeof(Tp) << 3) - 1);
                w = std::countl_zero(lca), low = (((m_val >> w) ^ lca) << w) ^ mask, high = ((((m_val >> w) ^ lca) + 1) << w) ^ mask;
            }
        };
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                static Node s_buf[BUFFER];
                static size_type s_gc[BUFFER], s_use_cnt, s_gc_cnt;
                static constexpr Node *data() { return s_buf; }
                static size_type newnode() { return s_gc_cnt ? s_gc[--s_gc_cnt] : s_use_cnt++; }
                static void collect(size_type x) { s_buf[x] = {}, s_gc[s_gc_cnt++] = x; }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrap<BUFFER>::type<Node>::s_buf[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_gc[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_use_cnt = 1;
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_gc_cnt = 0;
        template <typename Node>
        struct VectorBuffer {
            static std::vector<Node> s_buf;
            static std::vector<size_type> s_gc;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                if (!s_gc.empty()) {
                    size_type res = s_gc.back();
                    s_gc.pop_back();
                    return res;
                }
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
            static void collect(size_type x) { s_buf[x] = {}, s_gc.push_back(x); }
        };
        template <typename Node>
        std::vector<Node> VectorBuffer<Node>::s_buf{Node{}};
        template <typename Node>
        std::vector<size_type> VectorBuffer<Node>::s_gc;
        template <typename Tp, typename IdentityMapping, typename Operation, typename SizeType = uint64_t, template <typename> typename BufferType = VectorBuffer>
        struct Tree {
            static_assert(std::is_unsigned<SizeType>::value, "SizeType Must Be Unsiged");
            static constexpr SizeType mask_size = sizeof(SizeType) << 3, mask = SizeType(1) << (mask_size - 1);
            struct node {
                Tp m_val;
                SizeType m_lca;
                size_type m_lc, m_rc;
                SizeType key() const { return m_lca ^ mask; }
                bool is_leaf() const { return m_lca & mask; }
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using tree_type = Tree<Tp, IdentityMapping, Operation, SizeType, BufferType>;
            using buffer_type = BufferType<node>;
            size_type m_root;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _reserve(size_type capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node>>::value, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            static size_type _newnode() { return buffer_type::newnode(); }
            static size_type _newnode(Tp val, SizeType lca) {
                size_type x = _newnode();
                _ptr(x)->m_val = val, _ptr(x)->m_lca = lca;
                return x;
            }
            static void _collect(size_type x) { buffer_type::collect(x); }
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
            static void _pushup(size_type cur) { _ptr(cur)->m_val = Operation()(_ptr(cur)->lchild()->m_val, _ptr(cur)->rchild()->m_val); }
            template <typename InitMapping>
            static size_type _initnode(SizeType floor, SizeType ceil, InitMapping &&mapping) {
                if (floor + 1 == ceil) return _newnode(mapping(floor), floor | mask);
                size_type w = std::bit_width(floor ^ (ceil - 1)), rt = _newnode(), lc = _initnode(floor, floor + (SizeType(1) << (w - 1)), mapping), rc = _initnode(floor + (SizeType(1) << (w - 1)), ceil, mapping);
                _ptr(rt)->m_val = Operation()(_ptr(lc)->m_val, _ptr(rc)->m_val), _ptr(rt)->m_lca = (floor | mask) >> w, _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = rc;
                return rt;
            }
            static size_type _modify(size_type cur, SizeType i, Tp val) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                SizeType x = (i | mask) >> w;
                if (x < p->m_lca) {
                    size_type rt = _newnode(Operation()(val, p->m_val), x >> std::bit_width(x ^ p->m_lca)), lc = _newnode(val, i | mask);
                    _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = cur;
                    return rt;
                }
                if (x > p->m_lca) {
                    size_type rt = _newnode(Operation()(p->m_val, val), x >> std::bit_width(x ^ p->m_lca)), rc = _newnode(val, i | mask);
                    _ptr(rt)->m_lc = cur, _ptr(rt)->m_rc = rc;
                    return rt;
                }
                if (!w) return p->m_val = val, cur;
                if (i >> (w - 1) & 1) {
                    size_type rc = p->m_rc ? _modify(p->m_rc, i, val) : _newnode(val, i | mask);
                    _ptr(cur)->m_rc = rc;
                } else {
                    size_type lc = p->m_lc ? _modify(p->m_lc, i, val) : _newnode(val, i | mask);
                    _ptr(cur)->m_lc = lc;
                }
                return _pushup(cur), cur;
            }
            static size_type _add(size_type cur, SizeType i, Tp val) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                SizeType x = (i | mask) >> w;
                if (x < p->m_lca) {
                    size_type rt = _newnode(Operation()(val, p->m_val), x >> std::bit_width(x ^ p->m_lca)), lc = _newnode(val, i | mask);
                    _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = cur;
                    return rt;
                }
                if (x > p->m_lca) {
                    size_type rt = _newnode(Operation()(p->m_val, val), x >> std::bit_width(x ^ p->m_lca)), rc = _newnode(val, i | mask);
                    _ptr(rt)->m_lc = cur, _ptr(rt)->m_rc = rc;
                    return rt;
                }
                p->m_val = Operation()(p->m_val, val);
                if (w)
                    if (i >> (w - 1) & 1) {
                        size_type rc = p->m_rc ? _add(p->m_rc, i, val) : _newnode(val, i | mask);
                        _ptr(cur)->m_rc = rc;
                    } else {
                        size_type lc = p->m_lc ? _add(p->m_lc, i, val) : _newnode(val, i | mask);
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
            static Tp _query(size_type cur, SizeType i) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                if (((i | mask) >> w) != p->m_lca) return IdentityMapping()();
                if (!w) return p->m_val;
                if (i >> (w - 1) & 1)
                    return p->m_rc ? _query(p->m_rc, i) : IdentityMapping()();
                else
                    return p->m_lc ? _query(p->m_lc, i) : IdentityMapping()();
            }
            template <typename Filter = DefaultFilter>
            static Tp _query(size_type cur, SizeType key_low, SizeType key_high, Filter &&filter) {
                node *p = _ptr(cur);
                size_type w;
                SizeType low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                key_low = std::max(key_low, low), key_high = std::min(key_high, --high);
                if (key_low > key_high) return IdentityMapping()();
                if (key_low == low && key_high == high) return p->m_val;
                if (!(key_high >> (w - 1) & 1))
                    return _query(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key_low, key_high, filter);
                else if (key_low >> (w - 1) & 1)
                    return _query(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key_low, key_high, filter);
                else
                    return Operation()(_query(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key_low, key_high, filter), _query(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key_low, key_high, filter));
            }
            template <typename Filter = DefaultFilter, typename Judger>
            static SizeType _max_right(size_type cur, SizeType start, SizeType lim, Tp &val, Filter &&filter, Judger &&judge) {
                if (!cur) return lim;
                node *p = _ptr(cur);
                size_type w;
                SizeType low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                if (start >= high) return lim;
                if (start <= low) {
                    auto a = Operation()(val, p->m_val);
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
            static SizeType _min_left(size_type cur, SizeType start, SizeType lim, Tp &val, Filter &&filter, Judger &&judge) {
                if (!cur) return lim;
                node *p = _ptr(cur);
                size_type w;
                SizeType low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                if (start <= low) return lim;
                if (start >= high) {
                    auto a = Operation()(p->m_val, val);
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
            static size_type _copy(size_type y) {
                if (!y) return 0;
                size_type x = _newnode(_ptr(y)->m_val, _ptr(y)->m_lca), lc = _copy(_ptr(y)->m_lc), rc = _copy(_ptr(y)->m_rc);
                _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc;
                return x;
            }
            static void _collect_all(size_type cur) {
                node *p = _ptr(cur);
                if (!p->is_leaf()) _collect_all(p->m_lc), _collect_all(p->m_rc);
                _collect(cur);
            }
            node *_root() const { return _ptr(m_root); }
            Tree() { _ptr(m_root = 0)->m_val = IdentityMapping()(); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) : Tree() { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) : Tree() { reset(first, last); }
            Tree(const tree_type &rhs) { m_root = rhs.m_root ? _copy(rhs.m_root) : 0; }
            Tree(tree_type &&rhs) noexcept { m_root = rhs.m_root, rhs.m_root = 0; }
            ~Tree() { clear(); }
            tree_type &operator=(const tree_type &rhs) {
                if (this == &rhs) return *this;
                clear();
                if (rhs.m_root) m_root = _copy(rhs.m_root);
                return *this;
            }
            tree_type &operator=(tree_type &&rhs) noexcept {
                if (this == &rhs) return *this;
                std::swap(m_root, rhs.m_root);
                return *this;
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                clear();
                if (length) m_root = _initnode(0, length, mapping);
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void clear() {
                if (m_root) _collect_all(m_root), m_root = 0;
            }
            bool empty() const { return !m_root; }
            void add(SizeType i, Tp val) { m_root = m_root ? _add(m_root, i, val) : _newnode(val, i | mask); }
            void modify(SizeType i, Tp val) { m_root = m_root ? _modify(m_root, i, val) : _newnode(val, i | mask); }
            Tp query(SizeType i) const { return m_root ? _query(m_root, i) : IdentityMapping()(); }
            Tp query(SizeType left, SizeType right) const { return m_root ? _query(m_root, left, right, {}) : IdentityMapping()(); }
            Tp query_all() const { return _root()->m_val; }
            template <typename Judge>
            SizeType max_right(SizeType left, Judge &&judge) {
                Tp val = IdentityMapping()();
                if (!judge(val)) return left - 1;
                return _max_right(m_root, left, mask, val, {}, judge) - 1;
            }
            template <typename Judge>
            SizeType min_left(SizeType right, Judge &&judge) {
                Tp val = IdentityMapping()();
                if (!judge(val)) return right + 1;
                return _min_left(m_root, right + 1, 0, val, {}, judge);
            }
            bool contains(SizeType i) const { return _contains(m_root, i); }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_root) _dfs(m_root, call);
            }
        };
        template <typename Ostream, typename Tp, typename IdentityMapping, typename Operation, typename SizeType, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Tp, IdentityMapping, Operation, SizeType, BufferType> &x) {
            out << "[";
            auto call = [&, started = false](SizeType pos, Tp val) mutable {
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
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), typename SizeType = uint64_t>
    using VectorCompressedMaxTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, Minimum>, CPTREE::ChoiceByCompare<Tp, std::less<Tp>>, SizeType, CPTREE::VectorBuffer>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), typename SizeType = uint64_t>
    using VectorCompressedMinTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, Maximum>, CPTREE::ChoiceByCompare<Tp, std::greater<Tp>>, SizeType, CPTREE::VectorBuffer>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorCompressedGcdTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, 0>, CPTREE::FpTransfer<Tp, std::gcd<Tp>>, SizeType, CPTREE::VectorBuffer>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorCompressedLcmTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, 1>, CPTREE::FpTransfer<Tp, std::lcm<Tp>>, SizeType, CPTREE::VectorBuffer>;
    template <typename Tp, Tp OneMask = Tp(-1), typename SizeType = uint64_t>
    using VectorCompressedBitAndTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, OneMask>, std::bit_and<Tp>, SizeType, CPTREE::VectorBuffer>;
    template <typename Tp, Tp ZeroMask = 0, typename SizeType = uint64_t>
    using VectorCompressedBitOrTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, ZeroMask>, std::bit_or<Tp>, SizeType, CPTREE::VectorBuffer>;
    template <typename Tp, Tp Zero = Tp(), typename SizeType = uint64_t>
    using VectorCompressedSumTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, Zero>, std::plus<Tp>, SizeType, CPTREE::VectorBuffer>;
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min(), typename SizeType = uint64_t, CPTREE::size_type BUFFER = 1 << 20>
    using StaticCompressedMaxTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, Minimum>, CPTREE::ChoiceByCompare<Tp, std::less<Tp>>, SizeType, CPTREE::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), typename SizeType = uint64_t, CPTREE::size_type BUFFER = 1 << 20>
    using StaticCompressedMinTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, Maximum>, CPTREE::ChoiceByCompare<Tp, std::greater<Tp>>, SizeType, CPTREE::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp, typename SizeType = uint64_t, CPTREE::size_type BUFFER = 1 << 20>
    using StaticCompressedGcdTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, 0>, CPTREE::FpTransfer<Tp, std::gcd<Tp>>, SizeType, CPTREE::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp, typename SizeType = uint64_t, CPTREE::size_type BUFFER = 1 << 20>
    using StaticCompressedLcmTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, 1>, CPTREE::FpTransfer<Tp, std::lcm<Tp>>, SizeType, CPTREE::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp, Tp OneMask = Tp(-1), typename SizeType = uint64_t, CPTREE::size_type BUFFER = 1 << 20>
    using StaticCompressedBitAndTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, OneMask>, std::bit_and<Tp>, SizeType, CPTREE::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp, Tp ZeroMask = 0, typename SizeType = uint64_t, CPTREE::size_type BUFFER = 1 << 20>
    using StaticCompressedBitOrTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, ZeroMask>, std::bit_or<Tp>, SizeType, CPTREE::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp, Tp Zero = Tp(), typename SizeType = uint64_t, CPTREE::size_type BUFFER = 1 << 20>
    using StaticCompressedSumTree = CPTREE::Tree<Tp, CPTREE::ConstexprIdentity<Tp, Zero>, std::plus<Tp>, SizeType, CPTREE::StaticBufferWrap<BUFFER>::template type>;
}

#endif