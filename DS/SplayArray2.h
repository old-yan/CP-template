#ifndef __OY_SPLAYARRAY__
#define __OY_SPLAYARRAY__

#include <algorithm>
#include <cstdint>

namespace OY {
    template <typename _Tp>
    struct _SplayVoidWrap {
        _Tp m_val;
        _SplayVoidWrap() = default;
        _SplayVoidWrap(const _Tp &__val) : m_val(__val) {}
        const _Tp &val() const { return m_val; }
        const _Tp &info() = delete;
        template <typename _NodeRef>
        void update(_NodeRef __lc, _NodeRef __rc) {}
        template <typename _NodeRef>
        void push_down(_NodeRef __lc, _NodeRef __rc) {}
    };
    template <typename _Tp>
    struct _SplaySumWrap {
        _Tp m_val, m_sum;
        _SplaySumWrap() = default;
        _SplaySumWrap(const _Tp &__val) : m_val(__val), m_sum(__val) {}
        const _Tp &val() const { return m_val; }
        const _Tp &info() const { return m_sum; }
        template <typename _NodeRef>
        void update(_NodeRef __lc, _NodeRef __rc) {
            if (__lc && __rc)
                m_sum = __lc->key.m_sum + m_val + __rc->key.m_sum;
            else if (__lc)
                m_sum = __lc->key.m_sum + m_val;
            else if (__rc)
                m_sum = m_val + __rc->key.m_sum;
            else
                m_sum = m_val;
        }
        template <typename _NodeRef>
        void push_down(_NodeRef __lc, _NodeRef __rc) {}
    };
    template <typename _Tp>
    struct _SplayMaxWrap {
        _Tp m_val, m_max;
        _SplayMaxWrap() = default;
        _SplayMaxWrap(const _Tp &__val) : m_val(__val), m_max(__val) {}
        const _Tp &val() const { return m_val; }
        const _Tp &info() const { return m_max; }
        template <typename _NodeRef>
        void update(_NodeRef __lc, _NodeRef __rc) {
            if (__lc && __rc)
                m_max = std::max({__lc->key.m_max, m_val, __rc->key.m_max});
            else if (__lc)
                m_max = std::max(__lc->key.m_max, m_val);
            else if (__rc)
                m_max = std::max(m_val, __rc->key.m_max);
            else
                m_max = m_val;
        }
        template <typename _NodeRef>
        void push_down(_NodeRef __lc, _NodeRef __rc) {}
    };
    template <typename _Tp>
    struct _SplayMinWrap {
        _Tp m_val, m_min;
        _SplayMinWrap() = default;
        _SplayMinWrap(const _Tp &__val) : m_val(__val), m_min(__val) {}
        const _Tp &val() const { return m_val; }
        const _Tp &info() const { return m_min; }
        template <typename _NodeRef>
        void update(_NodeRef __lc, _NodeRef __rc) {
            if (__lc && __rc)
                m_min = std::min({__lc->key.m_min, m_val, __rc->key.m_min});
            else if (__lc)
                m_min = std::min(__lc->key.m_min, m_val);
            else if (__rc)
                m_min = std::min(m_val, __rc->key.m_min);
            else
                m_min = m_val;
        }
        template <typename _NodeRef>
        void push_down(_NodeRef __lc, _NodeRef __rc) {}
    };
    template <typename _Tp>
    struct _SplayReverseWrap {
        _Tp m_val;
        bool m_reversed;
        _SplayReverseWrap() = default;
        _SplayReverseWrap(const _Tp &__val) : m_val(__val), m_reversed(false) {}
        const _Tp &val() const { return m_val; }
        const _Tp &info() = delete;
        void reverse() { m_reversed = m_reversed ? false : true; }
        template <typename _NodeRef>
        void update(_NodeRef __lc, _NodeRef __rc) {}
        template <typename _NodeRef>
        void push_down(_NodeRef &__lc, _NodeRef &__rc) {
            if (m_reversed) {
                m_reversed = false;
                std::swap(__lc, __rc);
                if (__lc) __lc->key.reverse();
                if (__rc) __rc->key.reverse();
            }
        }
    };
    template <typename _Tp, typename _Operation>
    struct _SplayUpdateWrap {
        _Tp m_val, m_info;
        _SplayUpdateWrap() = default;
        _SplayUpdateWrap(const _Tp &__val) : m_val(__val), m_info(__val) {}
        const _Tp &val() const { return m_val; }
        const _Tp &info() const { return m_info; }
        template <typename _NodeRef>
        void update(_NodeRef __lc, _NodeRef __rc) {
            m_info = __lc ? _Operation()(__lc->key.m_info, m_val) : m_val;
            if (__rc) m_info = _Operation()(m_info, __rc->key.m_info);
        }
        template <typename _NodeRef>
        void push_down(_NodeRef &__lc, _NodeRef &__rc) {}
    };
    template <typename _Tp, typename _Fp, typename _DefaultChange, typename _Mapping, typename _Composition>
    struct _SplayModifyWrap {
        _Tp m_val;
        _Fp m_change;
        _SplayModifyWrap() = default;
        _SplayModifyWrap(const _Tp &__val) : m_val(__val), m_change(_DefaultChange()()) {}
        const _Tp &val() const { return m_val; }
        const _Tp &info() = delete;
        void add(const _Fp &__change) { m_val = _Mapping()(__change, m_val), m_change = _Composition()(__change, m_change); }
        template <typename _NodeRef>
        void update(_NodeRef __lc, _NodeRef __rc) {}
        template <typename _NodeRef>
        void push_down(_NodeRef &__lc, _NodeRef &__rc) {
            if (__lc) __lc->key.add(m_change);
            if (__rc) __rc->key.add(m_change);
            m_change = _DefaultChange()();
        }
    };
    template <typename _Tp, typename _Wrap, uint32_t _N = 1100000>
    struct SplayArray {
        using splayarr = SplayArray<_Tp, _Wrap, _N>;
        struct node {
            struct node_ref {
                uint32_t index;
                node *operator->() const { return s_nodes + index; }
                node &operator*() { return s_nodes[index]; }
                bool operator==(const node_ref &other) const { return index == other.index; }
                explicit operator bool() const { return index; }
                operator uint32_t() const { return index; }
            };
            _Wrap key;
            uint32_t size;
            node_ref parent, child[2];
        };
        using node_ref = typename node::node_ref;
        struct nodesToUpdate {
            node_ref pointers[3]{};
            ~nodesToUpdate() {
                for (node_ref ptr : pointers)
                    if (ptr) update(ptr);
            }
            node *operator->() { return pointers[0].operator->(); }
        };
        static inline node s_nodes[_N + 1];
        static inline node_ref s_cnt{0};
        node_ref m_root;
        static node_ref update(node_ref __s) {
            if (__s) __s->size = __s->child[0]->size + __s->child[1]->size + 1;
            __s->key.update(__s->child[0], __s->child[1]);
            return __s;
        }
        static node_ref push_down(node_ref __s) {
            __s->key.push_down(__s->child[0], __s->child[1]);
            return __s;
        }
        static node_ref connect(node_ref __s, node_ref __p, uint32_t __i) {
            if (__p) __p->child[__i] = __s;
            if (__s) __s->parent = __p;
            return __p;
        }
        static node_ref set_top(node_ref __s) {
            __s->parent.index = 0;
            return __s;
        }
        static node_ref _rotate(node_ref __s, node_ref __p, uint32_t __i) { return connect(update(connect(__s->child[__i ^ 1], __p, __i)), __s, __i ^ 1); }
        static node_ref rotate(node_ref __s, node_ref __p) { return _rotate(__s, __p, __p->child[1] == __s); }
        static node_ref rotate(node_ref __s, node_ref __p, node_ref __p2) {
            uint32_t i1 = __p->child[1] == __s, i2 = __p2->child[1] == __p;
            return i1 == i2 ? _rotate(__s, _rotate(__p, __p2, i2), i1) : _rotate(_rotate(__s, __p, i1), __p2, i2);
        }
        static node_ref splay(node_ref __s) {
            node_ref p = __s->parent, p2 = p->parent, p3 = p2->parent;
            for (; p3; p = p3, p2 = p->parent, p3 = p2->parent) rotate(__s, p, p2), connect(__s, p3, p3->child[1] == p2);
            return set_top(__s = p2 ? rotate(__s, p, p2) : (p ? rotate(__s, p) : __s));
        }
        static node_ref find_min(node_ref __s) {
            while (__s->child[0]) __s = push_down(__s)->child[0];
            return __s;
        }
        static node_ref find_max(node_ref __s) {
            while (__s->child[1]) __s = push_down(__s)->child[1];
            return __s;
        }
        static node_ref find_kth(node_ref __s, uint32_t __k) {
            while (true) {
                if (push_down(__s)->child[0]->size > __k)
                    __s = __s->child[0];
                else if (__k -= __s->child[0]->size; !__k--)
                    break;
                else
                    __s = __s->child[1];
            }
            return __s;
        }
        static node_ref merge(node_ref __x, node_ref __y) { return __x ? (__y ? connect(__y, splay(find_max(set_top(__x))), 1) : __x) : __y; }
        static node_ref newnode(_Tp __val) {
            s_cnt.index++;
            s_cnt->key = __val, s_cnt->size = 1;
            return s_cnt;
        }
        template <typename _Iterator>
        static node_ref make_tree(_Iterator __first, _Iterator __last) {
            auto _make_tree = [&](auto self, _Iterator first, _Iterator last) -> node_ref {
                if (first == last) return {0};
                if (first + 1 == last) return newnode(*first);
                _Iterator mid = first + (last - first) / 2;
                return update(connect(self(self, mid + 1, last), connect(self(self, first, mid), newnode(*mid), 0), 1));
            };
            return _make_tree(_make_tree, __first, __last);
        }
        static node_ref make_tree(uint32_t __length, const _Tp &__val) {
            auto _make_tree = [&](auto self, uint32_t first, uint32_t last) -> node_ref {
                if (first == last) return {0};
                if (first + 1 == last) return newnode(__val);
                uint32_t mid = first + (last - first) / 2;
                return update(connect(self(self, mid + 1, last), connect(self(self, first, mid), newnode(__val), 0), 1));
            };
            return _make_tree(_make_tree, 0, __length);
        }
        template <typename _Sequence = std::vector<int>>
        static _Sequence to_sequence(node_ref __root) {
            if (!__root) return _Sequence();
            _Sequence res;
            res.reserve(s_nodes[__root].size);
            auto dfs = [&](auto self, node_ref cur) -> void {
                push_down(cur);
                if (cur->child[0]) self(self, cur->child[0]);
                res.push_back(cur->key.val());
                if (cur->child[1]) self(self, cur->child[1]);
            };
            dfs(dfs, __root);
            return res;
        }
        SplayArray() : m_root{0} {}
        SplayArray(uint32_t __length, const _Tp __val = _Tp()) : m_root(make_tree(__length, __val)) {}
        template <typename _Iterator>
        SplayArray(_Iterator __first, _Iterator __last) : m_root(make_tree(__first, __last)) {}
        template <typename _Sequence, typename _Iterator = typename _Sequence::iterator>
        SplayArray(const _Sequence &__seq) : SplayArray(__seq.begin(), __seq.end()) {}
        SplayArray(std::initializer_list<_Tp> __seq) : SplayArray(__seq.begin(), __seq.end()) {}
        void clear() { m_root.index = 0; }
        void insert(uint32_t __pos, _Tp __key) {
            if (!__pos)
                push_front(__key);
            else if (__pos == size())
                push_back(__key);
            else {
                find_kth(m_root, __pos);
                node_ref p = newnode(__key);
                connect(m_root->child[0], p, 0);
                m_root->child[0].index = 0;
                m_root = update(connect(update(m_root), p, 1));
            }
        }
        void insert(uint32_t __pos, splayarr &__other) {
            if (__other.empty())
                return;
            else if (!__pos) {
                __other.join(*this);
                std::swap(m_root, __other.m_root);
            } else if (__pos == size())
                join(__other);
            else
                __other.join(cut(__pos, size() - 1)), join(__other);
        }
        void insert(uint32_t __pos, splayarr &&__other) { insert(__pos, __other); }
        void update(uint32_t __pos, _Tp __key) {
            (m_root = splay(find_kth(m_root, __pos)))->key = __key;
            update(m_root);
        }
        void erase(uint32_t __pos) {
            if (!__pos)
                pop_front();
            else if (__pos + 1 == size())
                pop_back();
            else {
                node_ref p = splay(find_kth(m_root, __pos));
                m_root = update(splay(connect(p->child[1], find_max(set_top(p->child[0])), 1)));
            }
        }
        void erase(uint32_t __left, uint32_t __right) {
            if (!__left)
                m_root = set_top(splay(find_kth(m_root, __right))->child[1]);
            else if (__right + 1 == size())
                m_root = set_top(splay(find_kth(m_root, __left))->child[0]);
            else {
                node_ref p = splay(find_kth(m_root, __right + 1));
                m_root = update(connect(splay(find_kth(set_top(p->child[0]), __left))->child[0], p, 0));
            }
        }
        template <typename _Iterator>
        void assign(_Iterator __first, _Iterator __last) { m_root = make_tree(__first, __last); }
        void push_front(_Tp __key) { m_root = m_root ? update(splay(connect(newnode(__key), find_min(m_root), 0)->child[0])) : newnode(__key); }
        void pop_front() { m_root = set_top(splay(find_min(m_root))->child[1]); }
        void push_back(_Tp __key) { m_root = m_root ? update(splay(connect(newnode(__key), find_max(m_root), 1)->child[1])) : newnode(__key); }
        void pop_back() { m_root = set_top(splay(find_max(m_root))->child[0]); }
        node_ref operator[](uint32_t __index) { return m_root = update(splay(find_kth(m_root, __index))); }
        node_ref at(uint32_t __index) { return (*this)[__index]; }
        node_ref front() { return m_root = update(splay(find_min(m_root))); }
        node_ref back() { return m_root = update(splay(find_max(m_root))); }
        uint32_t size() const { return m_root->size; }
        bool empty() const { return !m_root; }
        splayarr cut(uint32_t __left, uint32_t __right) {
            splayarr sub;
            if (!__left) {
                node_ref p = splay(find_kth(m_root, __right));
                m_root = set_top(p->child[1]), sub.m_root = update(connect({0}, p, 1));
            } else if (__right + 1 == size()) {
                node_ref p = splay(find_kth(m_root, __left));
                m_root = set_top(p->child[0]), sub.m_root = update(connect({0}, p, 0));
            } else {
                node_ref p = splay(find_kth(m_root, __right + 1)), q = splay(find_kth(set_top(p->child[0]), __left));
                m_root = update(connect(q->child[0], p, 0)), sub.m_root = update(connect({0}, q, 0));
            }
            return sub;
        }
        nodesToUpdate sub_tree(uint32_t __left, uint32_t __right) {
            if (!__left)
                if (__right + 1 == size())
                    return nodesToUpdate{m_root};
                else {
                    m_root = update(splay(find_kth(m_root, __right + 1)));
                    return nodesToUpdate{m_root->child[0], m_root};
                }
            else if (__right + 1 == size()) {
                m_root = update(splay(find_kth(m_root, __left - 1)));
                return nodesToUpdate{m_root->child[1], m_root};
            } else {
                m_root = splay(find_kth(m_root, __right + 1));
                update(connect(update(splay(find_kth(set_top(m_root->child[0]), __left - 1))), m_root, 0));
                return nodesToUpdate{m_root->child[0]->child[1], m_root->child[0], m_root};
            }
        }
        node_ref sub_view(uint32_t __left, uint32_t __right) {
            if (!__left)
                return __right + 1 == size() ? m_root : (m_root = update(splay(find_kth(m_root, __right + 1))))->child[0];
            else if (__right + 1 == size())
                return (m_root = update(splay(find_kth(m_root, __left - 1))))->child[1];
            else {
                m_root = splay(find_kth(m_root, __right + 1));
                return update(connect(update(splay(find_kth(set_top(m_root->child[0]), __left - 1))), m_root, 0))->child[0]->child[1];
            }
        }
        auto query(uint32_t __left, uint32_t __right) { return sub_view(__left, __right)->key.info(); }
        void join(splayarr &__other) {
            if (__other.empty()) return;
            m_root = m_root ? update(merge(m_root, __other.m_root)) : __other.m_root;
            __other.m_root.index = 0;
        }
        void join(splayarr &&__other) { join(__other); }
        template <typename _Sequence = std::vector<_Tp>>
        _Sequence to_sequence() const { return to_sequence<_Sequence>(m_root); }
        template <typename _Sequence = std::vector<_Tp>>
        _Sequence to_sequence(uint32_t __left, uint32_t __right) { return to_sequence<_Sequence>(sub_view(__left, __right).pointers[0]); }
    };
    template <typename _Tp = int, uint32_t _N = 2000000>
    SplayArray() -> SplayArray<_Tp, _SplayVoidWrap<_Tp>, _N>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, uint32_t _N = 2000000>
    SplayArray(_Iterator, _Iterator) -> SplayArray<_Tp, _SplayVoidWrap<_Tp>, _N>;
    template <typename _Tp = int, uint32_t _N = 2000000>
    SplayArray(uint32_t, _Tp = _Tp()) -> SplayArray<_Tp, _SplayVoidWrap<_Tp>, _N>;
    template <typename _Sequence, typename _Tp = typename _Sequence::value_type, uint32_t _N = 2000000>
    SplayArray(const _Sequence &) -> SplayArray<_Tp, _SplayVoidWrap<_Tp>, _N>;
    template <typename _Tp, uint32_t _N = 2000000>
    SplayArray(std::initializer_list<_Tp>) -> SplayArray<_Tp, _SplayVoidWrap<_Tp>, _N>;
    template <typename _Tp, uint32_t _N = 2000000>
    using SplaySumArray = SplayArray<_Tp, _SplaySumWrap<_Tp>, _N>;
    template <typename _Tp, uint32_t _N = 2000000>
    using SplayMaxArray = SplayArray<_Tp, _SplayMaxWrap<_Tp>, _N>;
    template <typename _Tp, uint32_t _N = 2000000>
    using SplayMinArray = SplayArray<_Tp, _SplayMinWrap<_Tp>, _N>;
    template <typename _Tp, uint32_t _N = 2000000>
    using SplayReverseArray = SplayArray<_Tp, _SplayReverseWrap<_Tp>, _N>;
    template <typename _Tp, typename _Operation, uint32_t _N = 2000000>
    using SplayUpdateArray = SplayArray<_Tp, _SplayUpdateWrap<_Tp, _Operation>, _N>;
    template <typename _Tp, typename _Fp, typename _DefaultChange, typename _Mapping, typename _Composition, uint32_t _N = 2000000>
    using SplayModifyArray = SplayArray<_Tp, _SplayModifyWrap<_Tp, _Fp, _DefaultChange, _Mapping, _Composition>, _N>;
}

#endif