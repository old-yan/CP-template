/*
最后修改:
20240920
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LICHAOSEGTREEEX__
#define __OY_LICHAOSEGTREEEX__

#include <functional>
#include <limits>
#include <numeric>

#include "VectorBufferWithCollect.h"

namespace OY {
    namespace LCSEGEX {
        using size_type = uint32_t;
        template <typename Tp>
        struct BaseLine {
            Tp m_k, m_b;
            static BaseLine<Tp> make_constant_line(Tp val) { return BaseLine<Tp>(0, val); }
            BaseLine() = default;
            BaseLine(Tp k, Tp b) : m_k(k), m_b(b) {}
            void add(Tp inc) { m_b += inc; }
            Tp calc(Tp i) const { return m_k * i + m_b; }
            bool is_ascending() const { return m_k > 0; }
            bool is_horizontal() const { return m_k == 0; }
        };
        struct BaseLess {
            template <typename Tp>
            bool operator()(const Tp &x, const Tp &y) const { return x < y; }
        };
        template <typename Tp, Tp BackGroundValue>
        struct BaseJudger {
            template <typename Line>
            bool operator()(const Line &line) const { return line.is_horizontal() && std::abs(line.calc(0)) >= std::abs(BackGroundValue) / 2; }
        };
        template <typename Tp, typename Line, bool MaintainMax>
        struct Info {
            using line_type = Line;
            Line m_line;
            Tp m_max;
            void set(Tp val) { m_max = val; }
            Tp get() const { return m_max; }
            void update(Tp val) { m_max = std::max(m_max, val); }
            bool operator<(const Info &rhs) const { return m_max < rhs.m_max; }
        };
        template <typename Tp, typename Line>
        struct Info<Tp, Line, false> {
            using line_type = Line;
            Line m_line;
            Tp m_min;
            void set(Tp val) { m_min = val; }
            Tp get() const { return m_min; }
            void update(Tp val) { m_min = std::min(m_min, val); }
            bool operator<(const Info &rhs) const { return m_min > rhs.m_min; }
        };
        template <typename Node, typename Tp, bool HasInc>
        struct NodeBase {
            Tp m_inc;
        };
        template <typename Node, typename Tp>
        struct NodeBase<Node, Tp, false> {};
        template <typename Tp, typename Line, typename Info>
        struct NoLazy {
            template <typename SizeType>
            Tp get(const Line &line, SizeType i) const { return line.calc(i); }
            const Info &get(const Info &info) const { return info; }
            Tp get(Tp val) const { return val; }
            template <typename Node>
            NoLazy<Tp, Line, Info> apply(Node *) const { return {}; }
        };
        template <typename Tp, typename Line, typename Info>
        struct Lazy {
            Tp m_inc;
            template <typename SizeType>
            Tp get(Line line, SizeType i) const {
                line.add(m_inc);
                return line.calc(i);
            }
            Info get(Info info) const {
                info.m_line.add(m_inc), info.set(info.get() + m_inc);
                return info;
            }
            Tp get(Tp val) const { return val + m_inc; }
            template <typename Node>
            Lazy<Tp, Line, Info> apply(const Node *p) const { return {m_inc + p->m_inc}; }
        };
        template <typename Tp, Tp BackGroundValue, bool MaintainMax = true, bool RangeAdd = false, typename Line = BaseLine<Tp>, typename SizeType = uint64_t, typename BackGroundJudger = BaseJudger<Tp, BackGroundValue>, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
            static_assert(std::is_signed<Tp>::value, "Tp Must Be Signed Type");
        public:
            using tree_type = Tree<Tp, BackGroundValue, MaintainMax, RangeAdd, Line, SizeType, BackGroundJudger, BufferType>;
            using info_type = Info<Tp, Line, MaintainMax>;
            using lazy_type = typename std::conditional<RangeAdd, Lazy<Tp, Line, info_type>, NoLazy<Tp, Line, info_type>>::type;
            using comp_type = typename std::conditional<MaintainMax, std::less<Tp>, std::greater<Tp>>::type;
            struct node : NodeBase<node, Tp, RangeAdd> {
                info_type m_info;
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_root{};
            SizeType m_size{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static Tp _most(const Line &line, SizeType left, SizeType right) {
                if constexpr (MaintainMax)
                    return line.calc(line.is_ascending() ? right : left);
                else
                    return line.calc(line.is_ascending() ? left : right);
            }
            static size_type _newnode() {
                size_type c = buffer_type::newnode();
                _ptr(c)->m_info = {Line::make_constant_line(BackGroundValue), BackGroundValue};
                return c;
            }
            static void _collect(size_type x) {
                if constexpr (RangeAdd) _ptr(x)->m_inc = 0;
                _ptr(x)->m_lc = _ptr(x)->m_rc = 0, buffer_type::collect(x);
            }
            static void _collect_all(size_type cur) {
                if constexpr(buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(cur);
                }
            }
            static size_type _lchild(size_type cur, SizeType floor, SizeType ceil) {
                if (!_ptr(cur)->m_lc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_lc = c;
                }
                return _ptr(cur)->m_lc;
            }
            static size_type _rchild(size_type cur, SizeType floor, SizeType ceil) {
                if (!_ptr(cur)->m_rc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_rc = c;
                }
                return _ptr(cur)->m_rc;
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, Line line) {
                SizeType mid = (floor + ceil) >> 1;
                node *p = _ptr(cur);
                p->m_info.update(_most(line, floor, ceil));
                if (comp_type()(p->m_info.m_line.calc(mid), line.calc(mid))) std::swap(p->m_info.m_line, line);
                if (floor < ceil && !BackGroundJudger()(line))
                    if (comp_type()(p->m_info.m_line.calc(floor), line.calc(floor))) {
                        if constexpr (RangeAdd) line.add(-p->m_inc);
                        _add(_lchild(cur, floor, mid), floor, mid, line);
                    } else if (comp_type()(p->m_info.m_line.calc(ceil), line.calc(ceil))) {
                        if constexpr (RangeAdd) line.add(-p->m_inc);
                        _add(_rchild(cur, mid + 1, ceil), mid + 1, ceil, line);
                    }
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, Line line) {
                if (left == floor && right == ceil)
                    _add(cur, floor, ceil, line);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    node *p = _ptr(cur);
                    p->m_info.update(_most(line, left, right));
                    if constexpr (RangeAdd) line.add(-p->m_inc);
                    if (left <= mid) _add(_lchild(cur, floor, mid), floor, mid, left, std::min(mid, right), line);
                    if (right > mid) _add(_rchild(cur, mid + 1, ceil), mid + 1, ceil, std::max(mid + 1, left), right, line);
                }
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, SizeType i, Tp val) {
                node *p = _ptr(cur);
                p->m_info.update(val);
                if (floor != ceil) {
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid) {
                        if constexpr (RangeAdd) val -= p->m_inc;
                        _add(_lchild(cur, floor, mid), floor, mid, i, val);
                    } else {
                        if constexpr (RangeAdd) val -= p->m_inc;
                        _add(_rchild(cur, mid + 1, ceil), mid + 1, ceil, i, val);
                    }
                }
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, Tp inc) {
                node *p = _ptr(cur);
                if (left == floor && right == ceil)
                    p->m_info.m_line.add(inc), p->m_info.set(p->m_info.get() + inc), p->m_inc += inc;
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (!BackGroundJudger()(p->m_info.m_line)) {
                        auto line = p->m_info.m_line;
                        line.add(-p->m_inc);
                        _add(_lchild(cur, floor, mid), floor, mid, line), _add(_rchild(cur, mid + 1, ceil), mid + 1, ceil, line);
                    }
                    if (left <= mid && _ptr(cur)->m_lc) _add(_ptr(cur)->m_lc, floor, mid, left, std::min(mid, right), inc);
                    if (right > mid && _ptr(cur)->m_rc) _add(_ptr(cur)->m_rc, mid + 1, ceil, std::max(mid + 1, left), right, inc);
                    p = _ptr(cur), p->m_info = {Line::make_constant_line(BackGroundValue), BackGroundValue};
                    if (p->m_lc) p->m_info.update(p->lchild()->m_info.get() + p->m_inc);
                    if (p->m_rc) p->m_info.update(p->rchild()->m_info.get() + p->m_inc);
                }
            }
            Tp _query(size_type cur, SizeType floor, SizeType ceil, SizeType i, lazy_type lazy) const {
                node *p = _ptr(cur);
                if (floor == ceil) return lazy.get(p->m_info.get());
                SizeType mid = (floor + ceil) >> 1;
                if (i <= mid) {
                    if (!p->m_lc) return lazy.get(p->m_info.m_line, i);
                    return std::max(_query(p->m_lc, floor, mid, i, lazy.apply(p)), lazy.get(p->m_info.m_line, i), comp_type());
                } else {
                    if (!p->m_rc) return lazy.get(p->m_info.m_line, i);
                    return std::max(_query(p->m_rc, mid + 1, ceil, i, lazy.apply(p)), lazy.get(p->m_info.m_line, i), comp_type());
                }
            }
            void _query(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, lazy_type lazy, Tp &res) const {
                node *p = _ptr(cur);
                if (!comp_type()(res, lazy.get(p->m_info.get()))) return;
                if (left == floor && right == ceil)
                    res = lazy.get(p->m_info.get());
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    res = std::max(res, lazy.get(_most(p->m_info.m_line, left, right)), comp_type());
                    if (left <= mid && p->m_lc) _query(p->m_lc, floor, mid, left, std::min(mid, right), lazy.apply(p), res);
                    if (right > mid && p->m_rc) _query(p->m_rc, mid + 1, ceil, std::max(mid + 1, left), right, lazy.apply(p), res);
                }
            }
            size_type _merge(size_type x, size_type y, SizeType floor, SizeType ceil) {
                if (!x || !y) return x | y;
                node *p = _ptr(x), *q = _ptr(y);
                p->m_info.update(q->m_info.get());
                if (floor != ceil) {
                    SizeType mid = (floor + ceil) >> 1;
                    size_type lc = _merge(p->m_lc, q->m_lc, floor, mid), rc = _merge(p->m_rc, q->m_rc, mid + 1, ceil);
                    p->m_lc = lc, p->m_rc = rc, _add(x, floor, ceil, q->m_info.m_line);
                } else if (comp_type()(p->m_info.m_line.calc(floor), q->m_info.m_line.calc(floor)))
                    p->m_info.m_line = q->m_info.m_line;
                _collect(y);
                return x;
            }
        public:
            Tree() = default;
            Tree(SizeType length) { resize(length); }
            Tree(const tree_type &rhs) = delete;
            Tree(tree_type &&rhs) noexcept { std::swap(m_root, rhs.m_root), std::swap(m_size, rhs.m_size); }
            ~Tree() { clear(); }
            tree_type &operator=(const tree_type &rhs) = delete;
            tree_type &operator=(tree_type &&rhs) noexcept {
                std::swap(m_root, rhs.m_root), std::swap(m_size, rhs.m_size);
                return *this;
            }
            SizeType size() const { return m_size; }
            void clear() {
                if (m_root) _collect_all(m_root), m_root = 0, m_size = 0;
            }
            void resize(SizeType length) {
                clear();
                if (m_size = length) m_root = _newnode();
            }
            void add_line(SizeType i, const Line &line) { _add(m_root, 0, m_size - 1, i, line.calc(i)); }
            void add_line(SizeType left, SizeType right, const Line &line) { _add(m_root, 0, m_size - 1, left, right, line); }
            void add_value(SizeType left, SizeType right, Tp inc) {
                static_assert(RangeAdd, "RangeAdd Must Be True");
                _add(m_root, 0, m_size - 1, left, right, inc);
            }
            Tp query(SizeType i) const { return _query(m_root, 0, m_size - 1, i, {}); }
            Tp query(SizeType left, SizeType right) const {
                Tp res = BackGroundValue;
                _query(m_root, 0, m_size - 1, left, right, {}, res);
                return res;
            }
            void merge(tree_type &rhs) { m_root = _merge(m_root, rhs.m_root, 0, m_size - 1), rhs.m_root = rhs.m_size = 0; }
            void merge(tree_type &&rhs) { merge(rhs); }
        };
    }
    template <typename Tp, Tp BackGroundValue = std::numeric_limits<Tp>::min(), bool RangeAdd = false, typename Line = LCSEGEX::BaseLine<Tp>, typename SizeType = uint64_t, typename BackGroundJudger = LCSEGEX::BaseJudger<Tp, BackGroundValue>>
    using VectorLichaoSlopeChmaxSegTree_ex = LCSEGEX::Tree<Tp, BackGroundValue, true, RangeAdd, Line, SizeType, BackGroundJudger, VectorBufferWithCollect>;
    template <typename Tp, Tp BackGroundValue = std::numeric_limits<Tp>::max(), bool RangeAdd = false, typename Line = LCSEGEX::BaseLine<Tp>, typename SizeType = uint64_t, typename BackGroundJudger = LCSEGEX::BaseJudger<Tp, BackGroundValue>>
    using VectorLichaoSlopeChminSegTree_ex = LCSEGEX::Tree<Tp, BackGroundValue, false, RangeAdd, Line, SizeType, BackGroundJudger, VectorBufferWithCollect>;
}

#endif