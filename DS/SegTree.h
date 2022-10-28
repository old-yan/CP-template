#ifndef __OY_SEGTREE__
#define __OY_SEGTREE__

#include <cstdint>
#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Operation = std::plus<_Tp>>
    struct SegTree {
        struct _TpNode : MemoryPool<_TpNode> {
            _Tp val;
            _TpNode *lchild;
            _TpNode *rchild;
            _TpNode(_Tp _val, _TpNode *_lchild, _TpNode *_rchild) : val(_val), lchild(_lchild), rchild(_rchild) {}
        };
        _TpNode *m_root;
        uint64_t m_length;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }
        _TpNode *lchild(_TpNode *cur) {
            if (!cur->lchild)
                cur->lchild = new _TpNode(m_defaultValue, nullptr, nullptr);
            return cur->lchild;
        }
        _TpNode *rchild(_TpNode *cur) {
            if (!cur->rchild)
                cur->rchild = new _TpNode(m_defaultValue, nullptr, nullptr);
            return cur->rchild;
        }
        _TpNode *_update(_TpNode *cur) {
            cur->val = m_op(cur->lchild ? cur->lchild->val : m_defaultValue, cur->rchild ? cur->rchild->val : m_defaultValue);
            return cur;
        }
        void _clear(_TpNode *p) {
            // if (p->lchild) delete p->lchild;
            // if (p->rchild) delete p->rchild;
            // delete p;
        }
        static void setBufferSize(uint32_t __count) { MemoryPool<_TpNode>::_reserve(__count); }
        SegTree(uint64_t __length = 0, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_root(nullptr), m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__length);
        }
        template <typename _Iterator>
        SegTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_root(nullptr), m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__first, __last);
        }
        ~SegTree() {
            if (m_root) _clear(m_root);
        }
        void resize(uint64_t __length) {
            if (m_root) _clear(m_root);
            m_root = (m_length = __length) ? new _TpNode(m_defaultValue, nullptr, nullptr) : nullptr;
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            if (m_root) _clear(m_root);
            m_length = __last - __first;
            auto dfs = [&](auto self, _Iterator first, _Iterator last) -> _TpNode * {
                if (first + 1 == last)
                    return new _TpNode(*first, nullptr, nullptr);
                else
                    return _update(new _TpNode(m_defaultValue, self(self, first, first + (last - first + 1) / 2), self(self, first + (last - first + 1) / 2, last)));
            };
            m_root = dfs(dfs, __first, __last);
        }
        void update(uint64_t __i, _Tp __val) {
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) -> void {
                if (left == right)
                    cur->val = __val;
                else {
                    if (__i <= (left + right) / 2)
                        self(self, lchild(cur), left, (left + right) / 2);
                    else
                        self(self, rchild(cur), (left + right) / 2 + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        void add(uint64_t __i, _Tp __inc) {
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) -> void {
                if (left == right)
                    cur->val = m_op(cur->val, __inc);
                else {
                    if (__i <= (left + right) / 2)
                        self(self, lchild(cur), left, (left + right) / 2);
                    else
                        self(self, rchild(cur), (left + right) / 2 + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp query(uint64_t __i) const {
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) {
                if (left == right)
                    return cur->val;
                else if (__i <= (left + right) / 2)
                    return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue;
                else
                    return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue;
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp query(uint64_t __left, uint64_t __right) const {
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) {
                if (left >= __left && right <= __right)
                    return cur->val;
                else if (__right <= (left + right) / 2)
                    return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue;
                else if (__left > (left + right) / 2)
                    return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue;
                else
                    return m_op(cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue, cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue);
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp queryAll() const { return m_root->val; }
        template <typename _Judge>
        uint64_t maxRight(uint64_t __left, _Judge __judge) const {
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) {
                if (__left <= left)
                    if (_Tp a(m_op(val, cur->val)); __judge(a)) {
                        val = a;
                        return right;
                    } else if (left == right)
                        return left - 1;
                if (__left <= (left + right) / 2)
                    if (uint64_t res = self(self, cur->lchild, left, (left + right) / 2); res != (left + right) / 2) return res;
                return self(self, cur->rchild, (left + right) / 2 + 1, right);
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        template <typename _Judge>
        uint64_t minLeft(uint64_t __right, _Judge __judge) const {
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) {
                if (__right >= right)
                    if (_Tp a(m_op(cur->val, val)); __judge(a)) {
                        val = a;
                        return left;
                    } else if (left == right)
                        return right + 1;
                if (__right > (left + right) / 2)
                    if (uint64_t res = self(self, cur->rchild, (left + right) / 2 + 1, right); res != (left + right) / 2 + 1) return res;
                return self(self, cur->lchild, left, (left + right) / 2);
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        uint64_t kth(_Tp __k) const {
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right, uint64_t k) {
                if (left == right) return left;
                if (cur->lchild) {
                    if (cur->lchild->val > k)
                        return self(self, cur->lchild, left, (left + right) / 2, k);
                    else
                        return self(self, cur->rchild, (left + right) / 2 + 1, right, k - cur->lchild->val);
                } else
                    return self(self, cur->rchild, (left + right) / 2 + 1, right, k);
            };
            return dfs(dfs, m_root, 0, m_length - 1, __k);
        }
    };
    template <typename _Tp = int64_t>
    SegTree(uint64_t, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> SegTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Tp = int64_t>
    SegTree(uint64_t, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> SegTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Operation = std::plus<int64_t>, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>>
    SegTree(uint64_t = 0, _Operation = _Operation(), _Tp = _Tp()) -> SegTree<_Tp, _Operation>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    SegTree(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> SegTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    SegTree(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> SegTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Operation = std::plus<_Tp>>
    SegTree(_Iterator, _Iterator, _Operation = _Operation(), _Tp = _Tp()) -> SegTree<_Tp, _Operation>;
}

#endif