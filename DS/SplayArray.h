#ifndef __OY_SPLAYARRAY__
#define __OY_SPLAYARRAY__

#include <functional>

#include "MemoryPool.h"

namespace OY {
    template <typename _Tp>
    struct _SplayVoidWrap {
        _Tp m_val;
        _SplayVoidWrap() = default;
        _SplayVoidWrap(const _Tp &__val) : m_val(__val) {}
        const _Tp &val() const { return m_val; }
        template <typename _Node>
        void update(const _Node *__lc, const _Node *__rc) {}
    };
    template <typename _Tp>
    struct _SplaySumWrap {
        _Tp m_val, m_sum;
        _SplaySumWrap() = default;
        _SplaySumWrap(const _Tp &__val) : m_val(__val), m_sum(__val) {}
        const _Tp &val() const { return m_val; }
        template <typename _Node>
        void update(const _Node *__lc, const _Node *__rc) {
            if (__lc && __rc)
                m_sum = __lc->key.m_sum + m_val + __rc->key.m_sum;
            else if (__lc)
                m_sum = __lc->key.m_sum + m_val;
            else if (__rc)
                m_sum = m_val + __rc->key.m_sum;
            else
                m_sum = m_val;
        }
    };
    template <typename _Tp>
    struct _SplayMaxWrap {
        _Tp m_val, m_max;
        _SplayMaxWrap() = default;
        _SplayMaxWrap(const _Tp &__val) : m_val(__val), m_max(__val) {}
        const _Tp &val() const { return m_val; }
        template <typename _Node>
        void update(const _Node *__lc, const _Node *__rc) {
            if (__lc && __rc)
                m_max = std::max({m_val, __lc->key.m_max, __rc->key.m_max});
            else if (__lc)
                m_max = std::max(m_val, __lc->key.m_max);
            else if (__rc)
                m_max = std::max(m_val, __rc->key.m_max);
            else
                m_max = m_val;
        }
    };
    template <typename _Tp>
    struct _SplayMinWrap {
        _Tp m_val, m_min;
        _SplayMinWrap() = default;
        _SplayMinWrap(const _Tp &__val) : m_val(__val), m_min(__val) {}
        const _Tp &val() const { return m_val; }
        template <typename _Node>
        void update(const _Node *__lc, const _Node *__rc) {
            if (__lc && __rc)
                m_min = std::min({m_val, __lc->key.m_min, __rc->key.m_min});
            else if (__lc)
                m_min = std::min(m_val, __lc->key.m_min);
            else if (__rc)
                m_min = std::min(m_val, __rc->key.m_min);
            else
                m_min = m_val;
        }
    };
    template <typename _Operation>
    struct _SplayMakeWrap {
        template <typename _Tp>
        struct _SplayInternalWrap {
            _Tp m_val, m_info;
            _SplayInternalWrap() = default;
            _SplayInternalWrap(const _Tp &__val) : m_val(__val), m_info(__val) {}
            const _Tp &val() const { return m_val; }
            template <typename _Node>
            void update(const _Node *__lc, const _Node *__rc) {
                m_info = __lc ? _Operation()(__lc->key.m_info, m_val) : m_val;
                if (__rc) m_info = _Operation()(m_info, __rc->key.m_info);
            }
        };
        template <typename _Tp>
        using type = _SplayInternalWrap<_Tp>;
    };
    template <typename _Tp, typename _Wrap = _SplayVoidWrap<_Tp>>
    struct SplayArray {
        using splayarr = SplayArray<_Tp, _Wrap>;
#pragma pack(4)
        struct node : MemoryPool<node> {
            _Wrap key;
            bool reversed;
            uint32_t subtree_weight;
            node *lchild, *rchild;
            node() = default;
            node(const _Tp &_key, bool _reversed, uint32_t _subtree_weight, node *_lchild, node *_rchild) : key(_key), reversed(_reversed), subtree_weight(_subtree_weight), lchild(_lchild), rchild(_rchild) {}
            void reverse() { reversed = reversed ? false : true; }
            void push_down() {
                reverse();
                std::swap(lchild, rchild);
                if (lchild) lchild->reverse();
                if (rchild) rchild->reverse();
            }
        };
#pragma pack()
        node *m_root;
        static inline uint32_t s_state;
        static uint32_t subtree_weight(node *__p) { return __p ? __p->subtree_weight : 0; }
        static node *update(node *__p) {
            if (__p) __p->subtree_weight = 1 + subtree_weight(__p->lchild) + subtree_weight(__p->rchild);
            __p->key.update(__p->lchild, __p->rchild);
            return __p;
        }
        static node *rrotate(node *__p) {
            node *q = __p->lchild;
            __p->lchild = q->rchild;
            q->rchild = update(__p);
            return q;
        }
        static node *lrotate(node *__p) {
            node *q = __p->rchild;
            __p->rchild = q->lchild;
            q->lchild = update(__p);
            return q;
        }
        static node *rrrotate(node *__p) {
            node *q = __p->lchild, *s = q->lchild;
            __p->lchild = q->rchild;
            q->rchild = update(__p);
            q->lchild = s->rchild;
            s->rchild = update(q);
            return s;
        }
        static node *llrotate(node *__p) {
            node *q = __p->rchild, *s = q->rchild;
            __p->rchild = q->lchild;
            q->lchild = update(__p);
            q->rchild = s->lchild;
            s->lchild = update(q);
            return s;
        }
        static node *lrrotate(node *__p) {
            node *q = __p->lchild, *r = q->rchild;
            q->rchild = r->lchild;
            r->lchild = update(q);
            __p->lchild = r->rchild;
            r->rchild = update(__p);
            return r;
        }
        static node *rlrotate(node *__p) {
            node *q = __p->rchild;
            node *r = q->lchild;
            q->lchild = r->rchild;
            __p->rchild = r->lchild;
            r->rchild = update(q);
            r->lchild = update(__p);
            return r;
        }
        template <typename _Sequence = std::vector<int>>
        static _Sequence to_sequence(node *__root) {
            if (!__root) return _Sequence();
            _Sequence res;
            res.reserve(__root->subtree_weight);
            auto dfs = [&](auto self, node *cur) -> void {
                if (cur->reversed) cur->push_down();
                if (cur->lchild) self(self, cur->lchild);
                res.push_back(cur->key.val());
                if (cur->rchild) self(self, cur->rchild);
            };
            dfs(dfs, __root);
            return res;
        }
        static node *update_from_lchild(node *__cur, node *__root) {
            s_state = s_state * 2 + 1;
            if (s_state == 3) {
                s_state = 0;
                return rrrotate(__cur);
            } else if (s_state == 5) {
                s_state = 0;
                return lrrotate(__cur);
            } else if (s_state == 1 && __cur == __root)
                return rrotate(__cur);
            else
                return __cur;
        }
        static node *update_from_rchild(node *__cur, node *__root) {
            s_state = s_state * 2 + 2;
            if (s_state == 4) {
                s_state = 0;
                return rlrotate(__cur);
            } else if (s_state == 6) {
                s_state = 0;
                return llrotate(__cur);
            } else if (s_state == 2 && __cur == __root)
                return lrotate(__cur);
            else
                return __cur;
        }
        static node *splay_max(node *__cur, node *const &__root) {
            if (__cur->reversed) __cur->push_down();
            if (__cur->rchild) {
                __cur->rchild = splay_max(__cur->rchild, __root);
                s_state++;
                if (s_state == 2) {
                    s_state = 0;
                    return llrotate(__cur);
                } else if (s_state == 1 && __cur == __root)
                    return lrotate(__cur);
            } else
                s_state = 0;
            return __cur;
        }
        static node *splay_min(node *__cur, node *const &__root) {
            if (__cur->reversed) __cur->push_down();
            if (__cur->lchild) {
                __cur->lchild = splay_min(__cur->lchild, __root);
                s_state++;
                if (s_state == 2) {
                    s_state = 0;
                    return rrrotate(__cur);
                } else if (s_state == 1 && __cur == __root)
                    return rrotate(__cur);
            } else
                s_state = 0;
            return __cur;
        }
        static node *splay_kth(node *__cur, node *const &__root, uint32_t __k) {
            if (__cur->reversed) __cur->push_down();
            if (__k < subtree_weight(__cur->lchild)) {
                __cur->lchild = splay_kth(__cur->lchild, __root, __k);
                return update_from_lchild(__cur, __root);
            } else if (__k -= subtree_weight(__cur->lchild); __k) {
                __cur->rchild = splay_kth(__cur->rchild, __root, __k - 1);
                return update_from_rchild(__cur, __root);
            } else {
                s_state = 0;
                return __cur;
            }
        }
        template <typename _Iterator>
        static node *make_tree(_Iterator __first, _Iterator __last) {
            auto _make_tree = [](auto self, _Iterator first, _Iterator last) -> node * {
                if (first == last) return nullptr;
                if (first + 1 == last) return new node(*first, false, 1, nullptr, nullptr);
                _Iterator mid = first + (last - first) / 2;
                return update(new node(*mid, false, 1, self(self, first, mid), self(self, mid + 1, last)));
            };
            return _make_tree(_make_tree, __first, __last);
        }
        static node *make_tree(uint32_t __length, const _Tp &__val) {
            auto _make_tree = [&](auto self, uint32_t first, uint32_t last) -> node * {
                if (first == last) return nullptr;
                if (first + 1 == last) return new node(__val, false, 1, nullptr, nullptr);
                uint32_t mid = first + (last - first) / 2;
                return update(new node(__val, false, 1, self(self, first, mid), self(self, mid + 1, last)));
            };
            return _make_tree(_make_tree, 0, __length);
        }
        static void _delete(node *p) {
            // delete p;
        }
        static void _clear(node *__cur) {
            if (__cur->lchild) _clear(__cur->lchild);
            if (__cur->rchild) _clear(__cur->rchild);
            _delete(__cur);
        }
        static void setBufferSize(uint32_t __count) { MemoryPool<node>::_reserve(__count); }
        SplayArray() : m_root(nullptr) {}
        SplayArray(uint32_t __length, const _Tp __val = _Tp()) : m_root(make_tree(__length, __val)) {}
        template <typename _Iterator>
        SplayArray(_Iterator __first, _Iterator __last) : m_root(make_tree(__first, __last)) {}
        template <typename _Sequence, typename _Iterator = typename _Sequence::iterator>
        SplayArray(const _Sequence &__seq) : SplayArray(__seq.begin(), __seq.end()) {}
        SplayArray(std::initializer_list<_Tp> __seq) : SplayArray(__seq.begin(), __seq.end()) {}
        void clear() {
            if (m_root) _clear(m_root), m_root = nullptr;
        }
        void insert(uint32_t __pos, _Tp __key) {
            if (!__pos)
                push_front(__key);
            else if (__pos == size())
                push_back(__key);
            else {
                m_root = splay_kth(m_root, m_root, __pos);
                node *p = new node(__key, false, 1, m_root->lchild, m_root);
                m_root->lchild = nullptr;
                update(m_root);
                m_root = update(p);
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
            else {
                node *p = splay_kth(m_root, m_root, __pos - 1), *q = splay_max(__other.m_root, __other.m_root);
                q->rchild = p->rchild;
                p->rchild = update(q);
                m_root = update(p);
                __other.m_root = nullptr;
            }
        }
        void insert(uint32_t __pos, splayarr &&__other) { insert(__pos, __other); }
        void update(uint32_t __pos, _Tp __key) {
            (m_root = splay_kth(m_root, m_root, __pos))->key = __key;
            update(m_root);
        }
        void erase(uint32_t __pos) {
            if (!__pos)
                pop_front();
            else if (__pos + 1 == size())
                pop_back();
            else {
                m_root = splay_kth(m_root, m_root, __pos);
                node *p = splay_max(m_root->lchild, m_root->lchild);
                p->rchild = m_root->rchild;
                _delete(m_root);
                m_root = update(p);
            }
        }
        void erase(uint32_t __left, uint32_t __right) {
            if (!__left) {
                node *p = splay_kth(m_root, m_root, __right);
                m_root = p->rchild;
                p->rchild = nullptr;
                _clear(p);
            } else if (__right + 1 == size()) {
                node *p = splay_kth(m_root, m_root, __left);
                m_root = p->lchild;
                p->lchild = nullptr;
                _clear(p);
            } else {
                node *p = splay_kth(m_root, m_root, __right + 1);
                node *q = splay_kth(p->lchild, p->lchild, __left);
                p->lchild = q->lchild;
                m_root = update(p);
                q->lchild = nullptr;
                _clear(q);
            }
        }
        template <typename _Iterator>
        void assign(_Iterator __first, _Iterator __last) {
            clear();
            m_root = make_tree(__first, __last);
        }
        void reverse(uint32_t __left, uint32_t __right) {
            if (!__left) {
                if (__right + 1 == size())
                    m_root->reverse();
                else {
                    m_root = update(splay_kth(m_root, m_root, __right + 1));
                    m_root->lchild->reverse();
                }
            } else if (__right + 1 == size()) {
                m_root = update(splay_kth(m_root, m_root, __left - 1));
                m_root->rchild->reverse();
            } else {
                m_root = update(splay_kth(m_root, m_root, __right + 1));
                m_root->lchild = update(splay_kth(m_root->lchild, m_root->lchild, __left - 1));
                m_root->lchild->rchild->reverse();
            }
        }
        void push_front(_Tp __key) {
            if (!m_root)
                m_root = new node(__key, false, 1, nullptr, nullptr);
            else {
                m_root = splay_min(m_root, m_root);
                m_root->lchild = new node(__key, false, 1, nullptr, nullptr);
                m_root = update(m_root);
            }
        }
        void pop_front() {
            node *p = splay_min(m_root, m_root)->rchild;
            _delete(m_root);
            m_root = p;
        }
        void push_back(_Tp __key) {
            if (!m_root)
                m_root = new node(__key, false, 1, nullptr, nullptr);
            else {
                m_root = splay_max(m_root, m_root);
                m_root->rchild = new node(__key, false, 1, nullptr, nullptr);
                m_root = update(m_root);
            }
        }
        void pop_back() {
            node *p = splay_max(m_root, m_root)->lchild;
            _delete(m_root);
            m_root = p;
        }
        node *operator[](uint32_t __index) { return m_root = update(splay_kth(m_root, m_root, __index)); }
        node *at(uint32_t __index) { return (*this)[__index]; }
        node *front() { return m_root = update(splay_min(m_root, m_root)); }
        node *back() { return m_root = update(splay_max(m_root, m_root)); }
        uint32_t size() const { return subtree_weight(m_root); }
        bool empty() const { return !m_root; }
        splayarr cut(uint32_t __left, uint32_t __right) {
            splayarr sub;
            if (!__left) {
                node *p = splay_kth(m_root, m_root, __right);
                m_root = p->rchild;
                p->rchild = nullptr;
                sub.m_root = update(p);
            } else if (__right + 1 == size()) {
                node *p = splay_kth(m_root, m_root, __left);
                m_root = p->lchild;
                p->lchild = nullptr;
                sub.m_root = update(p);
            } else {
                node *p = splay_kth(m_root, m_root, __right + 1), *q = splay_kth(p->lchild, p->lchild, __left);
                p->lchild = q->lchild;
                m_root = update(p);
                q->lchild = nullptr;
                sub.m_root = update(q);
            }
            return sub;
        }
        node *sub_view(uint32_t __left, uint32_t __right) {
            if (!__left)
                return __right + 1 < size() ? update(m_root = splay_kth(m_root, m_root, __right + 1))->lchild : m_root;
            else if (__right + 1 == size())
                return update(m_root = splay_kth(m_root, m_root, __left - 1))->rchild;
            else {
                m_root = update(splay_kth(m_root, m_root, __right + 1));
                auto __cur = splay_kth(m_root->lchild, m_root->lchild, __left - 1);
                m_root->lchild = update(__cur);
                return __cur->rchild;
                return update(m_root->lchild = splay_kth(m_root->lchild, m_root->lchild, __left - 1))->rchild;
            }
        }
        void join(splayarr &__other) {
            if (__other.empty()) return;
            if (!m_root)
                m_root = __other.m_root;
            else {
                m_root = splay_max(m_root, m_root);
                m_root->rchild = __other.m_root;
                m_root = update(m_root);
            }
            __other.m_root = nullptr;
        }
        void join(splayarr &&__other) { join(__other); }
        template <typename _Sequence = std::vector<_Tp>>
        _Sequence to_sequence() const { return to_sequence<_Sequence>(m_root); }
        template <typename _Sequence = std::vector<_Tp>>
        _Sequence to_sequence(uint32_t __left, uint32_t __right) { return to_sequence<_Sequence>(sub_view(__left, __right)); }
    };
    template <typename _Tp = int>
    SplayArray() -> SplayArray<_Tp, _SplayVoidWrap<_Tp>>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    SplayArray(_Iterator, _Iterator) -> SplayArray<_Tp, _SplayVoidWrap<_Tp>>;
    template <typename _Tp = int>
    SplayArray(uint32_t, _Tp = _Tp()) -> SplayArray<_Tp, _SplayVoidWrap<_Tp>>;
    template <typename _Sequence, typename _Tp = typename _Sequence::value_type>
    SplayArray(const _Sequence &) -> SplayArray<_Tp, _SplayVoidWrap<_Tp>>;
    template <typename _Tp>
    SplayArray(std::initializer_list<_Tp>) -> SplayArray<_Tp, _SplayVoidWrap<_Tp>>;
    template <typename _Tp>
    using SplaySumArray = SplayArray<_Tp, _SplaySumWrap<_Tp>>;
    template <typename _Tp>
    using SplayMaxArray = SplayArray<_Tp, _SplayMaxWrap<_Tp>>;
    template <typename _Tp>
    using SplayMinArray = SplayArray<_Tp, _SplayMinWrap<_Tp>>;
    template <typename _Tp, typename _Operation>
    using SplayArray_ex = SplayArray<_Tp, typename _SplayMakeWrap<_Operation>::type>;
}

#endif